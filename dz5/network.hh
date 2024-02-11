#pragma once

#include "matrix.hh"

#include <vector>
#include <stdexcept>
#include <cmath>
#include <random>
#include <iomanip>
#include <algorithm>

#include <iostream>

struct Layer {
	Matrix weights;
	Matrix bias;
};

class Network {
public:
	Network(const std::vector<int>& arch);

	void fit(
		const std::vector<Matrix>& samples,
		const std::vector<Matrix>& labels,
		const double               learning_rate,
		const int                  epoch_max,
		int                        batch_size = 0
	);
	double loss(
		const std::vector<Matrix>& samples,
		const std::vector<Matrix>& labels
	);
	Matrix predict(const Matrix& input) const;
private:
	const std::vector<int> arch;
	std::vector<Layer>     layers;

	static std::vector<Layer> create_layers(const std::vector<int>& arch);
	static void               initialize_layers(std::vector<Layer>& layers);

	static Matrix                           compute_activation(const Matrix &v);
	static std::vector<std::vector<double>> compute_values(const std::vector<Layer>& layers, const Matrix& input);
	static std::vector<std::vector<double>> compute_errors(
		const std::vector<Layer> layers,
		const std::vector<std::vector<double>>& values,
		const Matrix& output
	);

	static void initialize_weights(Matrix& weights);
};
