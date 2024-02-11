#include "network.hh"

void Network::initialize_weights(Matrix& weights) {
	std::random_device       rd;
	std::mt19937             gen(rd());
	std::normal_distribution d(0.0, 1.0);

	for (int i{0}; i < weights.getRows(); ++i) {
		for (int j{0}; j < weights.getColumns(); ++j) {
			weights[i][j] = d(gen);
		}
	}
}

std::vector<Layer> Network::create_layers(const std::vector<int>& arch) {
	std::vector<Layer> ret;
	for (int i{1}; i < static_cast<int>(arch.size()); ++i) {
		const int nodes{arch[i]};
		const int nodes_last{arch[i - 1]};
		
		Matrix weights(nodes, nodes_last);
		Matrix bias(nodes, 1);
		
		Layer layer{weights, bias};
		
		ret.push_back(layer);
	}
	return ret;
}

void Network::initialize_layers(std::vector<Layer>& layers) {
	for (Layer& layer : layers) {
		initialize_weights(layer.weights);
	}
}

Network::Network(const std::vector<int>& a): arch(a), layers(create_layers(arch)) {
	if (arch.size() < 1) {
		throw std::invalid_argument("the network's architecture can't be undefined");
	}
	initialize_layers(layers);
}

double func_sigmoid(double x) {
	return 1.0 / (1.0 + std::exp(-x));
}

Matrix Network::compute_activation(const Matrix& v) {
	Matrix ret(v);
	for (int i{0}; i < ret.getRows(); ++i) {
		ret[i][0] = func_sigmoid(ret[i][0]);
	}
	return ret;
}

Matrix Network::predict(const Matrix& input) const {
	if (input.getColumns() != 1) {
		throw std::invalid_argument("the input isn't a vector");
	}
	if (input.getRows() != arch[0]) {
		throw std::invalid_argument("the input's size don't match the network's architecture");
	}

	if (arch.size() == 1) {
		return compute_activation(input);
	}

	Matrix v(input);
	for (const Layer& layer : layers) {
		v = compute_activation(layer.weights * v + layer.bias);
	}

	return v;
}

std::vector<double> map_to_vector(const Matrix& vector) {
	std::vector<double> ret;
	for (int i{0}; i < vector.getRows(); ++i) {
		ret.push_back(vector[i][0]);
	}
	return ret;
}

std::vector<std::vector<double>> Network::compute_values(const std::vector<Layer>& layers, const Matrix& input) {
	std::vector<std::vector<double>> ret;
	if (layers.size() == 0) {
		std::cout << "layers size = 0\n" << std::flush;
		return ret;
	}
	if (input.getColumns() != 1) {
		throw std::invalid_argument("the input isn't a vector");
	}
	if (input.getRows() != layers[0].weights.getColumns()) {
		throw std::invalid_argument("the input's size don't match the network's architecture");
	}

	ret.push_back(map_to_vector(input));

	Matrix v(input);
	for (const Layer& layer : layers) {
		v = compute_activation(layer.weights * v + layer.bias);
		ret.push_back(map_to_vector(v));
	}

	return ret;
}

std::vector<std::vector<double>> Network::compute_errors(
	const std::vector<Layer>                layers,
	const std::vector<std::vector<double>>& values,
	const Matrix&                           output
) {
	std::vector<double>              labels{map_to_vector(output)};
	std::vector<std::vector<double>> errors;
	
	const int last_index{static_cast<int>(layers.size() - 1)};
	{
		std::vector<double> layer_errors;
		const int           nodes{layers[last_index].weights.getRows()};
		for (int i{0}; i < nodes; ++i) {
			const double value{values[last_index + 1][i]};
			const double label{labels[i]};
			const double error{value * (1 - value) * (label - value)};

			layer_errors.push_back(error);
		}
		errors.push_back(layer_errors);
	}

	for (int l{static_cast<int>(layers.size() - 1)}; l > 0; --l) {
		std::vector<double> layer_errors;
		const int           nodes{layers[l - 1].weights.getRows()};
		for (int n{0}; n < nodes; ++n) {
			const double value{values[l][n]};
			double       sum{0};

			const int nodes_next{layers[l].weights.getRows()};
			const int index_layer_prev{static_cast<int>(errors.size() - 1)};
			for (int m{0}; m < nodes_next; ++m) {
				sum += errors[index_layer_prev][m] * layers[l].weights[m][n];
			}

			const double error{value * (1 - value) * sum};

			layer_errors.push_back(error);
		}
		errors.push_back(layer_errors);
	}

	// Empty vector to correct indices for other error vectors.
	errors.push_back(std::vector<double>());

	// Reorder error vectors
	std::reverse(errors.begin(), errors.end());

	return errors;
}

double Network::loss(
	const std::vector<Matrix>& samples,
	const std::vector<Matrix>& labels
) {
	if (samples.size() != labels.size()) {
		throw std::invalid_argument("the number of the provided samples don't match the number of the provided lables");
	}
	
	double sum{0};
	const int sample_size{static_cast<int>(samples.size())};
	for (int i{0}; i < sample_size; ++i) {
		const Matrix& sample{samples[i]};
		const Matrix& label{labels[i]};
		const Matrix& prediction{predict(sample)};

		if (label.getRows() != prediction.getRows()) {
			throw std::invalid_argument("the prediction's vector size doesn't match the label's vector size");
		}
		for (int j{0}; j < static_cast<int>(label.getRows()); ++j) {
			const double diff{label[j][0] - prediction[j][0]};
			sum += diff * diff;
		}
	}

	return sum / sample_size;
}

void Network::fit(
	const std::vector<Matrix>& samples,
	const std::vector<Matrix>& labels,
	const double               learning_rate,
	const int                  epoch_max,
	int                        batch_size
) {
	if (samples.size() == 0) {
		throw std::invalid_argument("there are no samples");
	}
	if (samples.size() != labels.size()) {
		throw std::invalid_argument("the number of the provided samples don't match the number of the provided lables");
	}
	if (batch_size < 0) {
		throw std::invalid_argument("the batch size can't be negative");
	}
	if (batch_size == 0) {
		batch_size = static_cast<int>(samples.size());
	}

	std::cout.width(12);
	for (int epoch{0}; epoch < epoch_max; ++epoch) {

		const double l{loss(samples, labels)};
		std::cout << "epoch: " << epoch << ", loss: " << l << '\n' << std::flush;

		for (int b{0}; b < static_cast<int>(samples.size()); b += batch_size) {
			// Create a new batch
			auto start_sample{samples.begin() + b};
			auto start_label{labels.begin() + b};
			auto end_sample{start_sample + batch_size};
			auto end_label{start_label + batch_size};
			if (end_sample > samples.end()) {
				end_sample = samples.end();
				end_label = labels.end();
			}
			const std::vector<Matrix> batch_samples(start_sample, end_sample);
			const std::vector<Matrix> batch_labels(start_label, end_label);

			// Create the weight delta structure
			std::vector<Layer> weight_delta{create_layers(arch)};

			for (int s{0}; s < static_cast<int>(batch_samples.size()); ++s) {
				// Compute values for each neuron
				std::vector<std::vector<double>> values{compute_values(layers, batch_samples[s])};
				// Compute errors for each neuron
				std::vector<std::vector<double>> errors{compute_errors(layers, values, batch_labels[s])};

				// Add to the weight delta
				for (int l{1}; l < static_cast<int>(arch.size()); ++l) {
					Layer& layer{weight_delta[l - 1]};

					// Adjust weights
					for (int i{0}; i < layer.weights.getRows(); ++i) {
						for (int j{0}; j < layer.weights.getColumns(); ++j) {
							const double error{errors[l][i]};
							const double value{values[l - 1][j]};

							layer.weights[i][j] = layer.weights[i][j] + (error * value);
						}
					}

					// Adjust biases
					for (int i{0}; i < layer.bias.getRows(); ++i) {
						const double error{errors[l][i]};
						layer.bias[i][0] = layer.bias[i][0] + error;
					}
				}
			}

			// For each weight, add the weight delta to the particular weight
			for (int l{1}; l < static_cast<int>(arch.size()); ++l) {
				Layer& layer{layers[l - 1]};
				Layer& delta{weight_delta[l - 1]};
				for (int i{0}; i < layer.weights.getRows(); ++i) {
					for (int j{0}; j < layer.weights.getColumns(); ++j) {
						layer.weights[i][j] = layer.weights[i][j] + learning_rate * delta.weights[i][j];
					}
				}
				for (int i{0}; i < layer.bias.getRows(); ++i) {
					layer.bias[i][0] = layer.bias[i][0] + learning_rate * delta.bias[i][0];
				}

			}

		}

	}

}
