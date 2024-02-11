#include "individual.hh"

#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include <stdexcept>
#include <random>
#include <fstream>
#include <algorithm>
#include <limits>
#include <string>

using Candidate = Individual<std::array<double, 5>>;

double transfer_func(double x, double y, const std::array<double, 5>& params) {
	const double& beta0{params[0]};
	const double& beta1{params[1]};
	const double& beta2{params[2]};
	const double& beta3{params[3]};
	const double& beta4{params[4]};

	const double first{std::sin(beta0 + beta1 * x)};
	const double up{beta2 * std::cos(x * (beta3 + y))};
	const double x_min_beta4{x - beta4};
	const double down{1 + std::exp(x_min_beta4 * x_min_beta4)};
	const double second{up / down};
	return first + second;
}

double loss(const std::vector<std::array<double, 3>>& dataset, const std::array<double, 5>& chromosome) {
	double square_sum{0.0};
	for (std::size_t i{0}; i < dataset.size(); ++i) {
		const auto& record{dataset[i]};

		const double& x{record[0]};
		const double& y{record[1]};
		
		const double& actual{record[2]};
		const double predicted{transfer_func(x, y, chromosome)};
		
		const double diff{predicted - actual};

		square_sum += diff * diff;
	}
	return square_sum / dataset.size();
}

std::vector<std::array<double, 3>> dataset_load(int selector) {
	if (selector < 1 || selector > 2) {
		throw std::invalid_argument("the dataset selector should be 1 or 2");
	}

	std::string filename = "dataset" + std::to_string(selector) + ".txt";

	std::ifstream file_stream(filename);
	if (!file_stream.is_open()) {
		throw std::runtime_error("can't open the dataset file");
	}

	double x, y, f;
	std::vector<std::array<double, 3>> dataset;
	while (file_stream >> x >> y >> f) {
		dataset.push_back({x, y, f});
	}

	return dataset;
}

std::vector<std::size_t> selection_tournament(
	int                                       k,
	const std::vector<Candidate>& population
) {
	if (k < 0) {
		throw std::invalid_argument("tournament selection with a negative K is not allowed");
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<std::size_t> dis(0, population.size() - 1);

	std::vector<std::size_t> tournament;
	for (int i{0}; i < k; ++i) {
		const std::size_t index{dis(gen)};
		tournament.push_back(index);
	}

	std::sort(
		tournament.begin(),
		tournament.end(),
		[&](std::size_t a, std::size_t b) -> bool {
			return population[a].get_fitness() > population[b].get_fitness();
		}
	);

	return tournament;
}

Candidate crossbreed(const Candidate& parent0, const Candidate& parent1) {
	std::array<double, 5> child_chomosome;
	for (std::size_t i{0}; i < 5; ++i) {
		child_chomosome[i] = (parent0.get_chromosome()[i] + parent1.get_chromosome()[i]) / 2.0;
	}
	return Candidate(child_chomosome);
}

Candidate mutate(
	const Candidate& candidate,
	const double                 prob,
	const double                 deviation
) {
	if (prob < 0.0 || prob > 1.0) {
		throw std::invalid_argument("the probability is not in range [0, 1]");
	}
	if (deviation < 0.0) {
		throw std::invalid_argument("the deviation can't be negative");
	}

	std::random_device               rd;
	std::mt19937                     gen(rd());
	std::normal_distribution<double> dist_dev(0.0, deviation);
	std::bernoulli_distribution      dist_mut(prob);

	std::array<double, 5> mutated_chromosome;
	for (std::size_t i{0}; i < 5; ++i) {
		mutated_chromosome[i] = candidate.get_chromosome()[i];
		if (!dist_mut(gen)) {
			// The gene wasn't selected for the mutation
			continue;
		}
		// Mutating the selected gene by adding a slight deviation from the previous value.
		mutated_chromosome[i] += dist_dev(gen);
	}

	return Candidate(mutated_chromosome);
}

std::vector<Candidate> create_population(int size) {
	if (size < 0) {
		throw std::invalid_argument("the population size can't be negative");
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dis(-4.0, 4.0);

	std::vector<Candidate> ret;
	for (int i{0}; i < size; ++i) {
		const Candidate candidate({dis(gen), dis(gen), dis(gen), dis(gen), dis(gen)});
		ret.push_back(candidate);
	}

	return ret;
}

Candidate find_best(const std::vector<Candidate>& pop) {
	if (pop.size() < 1) {
		throw std::invalid_argument("can't find the best chromosome in an empty population");
	}

	std::size_t best_index{0};
	for (std::size_t i{1}; i < pop.size(); ++i) {
		if (pop[i].get_fitness() > pop[best_index].get_fitness()) {
			best_index = i;
		}
	}

	return pop[best_index];
}

void evaluate_population(std::vector<Candidate>& pop, const std::vector<std::array<double, 3>>& dataset) {
	double loss_max{std::numeric_limits<double>::min()};
	for (auto& candidate : pop) {
		const double loss_candidate{loss(dataset, candidate.get_chromosome())};
		if (loss_candidate > loss_max) {
			loss_max = loss_candidate;
		}

		candidate.set_fitness(-loss_candidate);
	}

	// Adjust the fitness to be greater than or equal to FITNESS_MIN.
	constexpr double FITNESS_MIN{10.0};
	for (auto& candidate : pop) {
		candidate.set_fitness(candidate.get_fitness() + loss_max + FITNESS_MIN);
	}
}

std::vector<Candidate> proportional_selection(const std::vector<Candidate>& pop, int count) {
	if (pop.size() < 1) {
		throw std::invalid_argument("can't proportionally select from an empty population");
	}
	if (count < 1) {
		throw std::invalid_argument("can't proportionally select less than 1 candidates");
	}

	double sum{0.0};
	for (const auto& candidate : pop) {
		sum += candidate.get_fitness();
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dis(0, sum);

	std::vector<Candidate> selected;
	for (int n{0}; n < count; ++n) {
		const double selected_sum{dis(gen)};

		std::size_t selected_idx{0};
		for (double partial_sum{pop[0].get_fitness()}; partial_sum < selected_sum;) {
			partial_sum += pop[++selected_idx].get_fitness();
		}

		selected.push_back(pop[selected_idx]);
	}

	return selected;
}

Candidate generational_genetic_algorithm(
	const int                                 pop_size,
	const int                                 iterations,
	const bool                                is_elitism,
	const double                              mutation_prob,
	const double                              mutation_dev,
	const std::vector<std::array<double, 3>>& dataset,
	const bool print_iter
) {
	if (pop_size < 1) {
		throw std::invalid_argument("the population size can't be less than 1");
	}
	if (iterations < 1) {
		throw std::invalid_argument("the iteration count can't be less than 1");
	}
	if (mutation_prob < 0.0 || mutation_prob > 1.0) {
		throw std::invalid_argument("the mutation probability is not in range [0, 1]");
	}
	if (mutation_dev < 0.0) {
		throw std::invalid_argument("the mutation deviation can't be negative");
	}
	if (dataset.size() < 1) {
		throw std::invalid_argument("the dataset can't be empty");
	}

	// Generate the initial population
	std::vector<Candidate> pop{create_population(pop_size)};
	evaluate_population(pop, dataset);

	// Loop while the iterations are not exhausted:
	for (int i{0}; i < iterations; ++i) {
		std::vector<Candidate> new_pop;
		// If the elitism is on, keep the best individual from the previous generation
		if (is_elitism) {
			new_pop.push_back(find_best(pop));
		}
		// Generate a new population:
		while (new_pop.size() < pop.size()) {
			// Select two parents
			const auto& parents{proportional_selection(pop, 2)};
			// Crossbreed them
			Candidate child{crossbreed(parents[0], parents[1])};
			// Mutate the child
			child = mutate(child, mutation_prob, mutation_dev);
			// Add the child to the new population
			new_pop.push_back(child);
		}
		// Evaluate the new population
		evaluate_population(new_pop, dataset);
		// Replace the old population with the new one
		pop = std::move(new_pop);

		// Evaluate a loss.
		const double current_fitness{find_best(pop).get_fitness()};

		if (print_iter) {
			std::cout.width(12);
			std::cout << "Iter: " << i << "\t\t" << "Fitness: " << current_fitness << std::endl;
		}
	}
	// Return the best individual from the population
	
	return find_best(pop);
}

Candidate eliminational_genetic_algorithm(
	const int                                 pop_size,
	const int                                 iterations,
	const double                              mutation_prob,
	const double                              mutation_dev,
	const int tournament_k,
	const std::vector<std::array<double, 3>>& dataset,
	const bool print_iter
) {
	if (pop_size < 1) {
		throw std::invalid_argument("the population size can't be less than 1");
	}
	if (iterations < 1) {
		throw std::invalid_argument("the iteration count can't be less than 1");
	}
	if (mutation_prob < 0.0 || mutation_prob > 1.0) {
		throw std::invalid_argument("the mutation probability is not in range [0, 1]");
	}
	if (mutation_dev < 0.0) {
		throw std::invalid_argument("the mutation deviation can't be negative");
	}
	if (dataset.size() < 1) {
		throw std::invalid_argument("the dataset can't be empty");
	}

	// Generate the initial population
	std::vector<Candidate> pop{create_population(pop_size)};
	evaluate_population(pop, dataset);

	// Loop while the iterations are not exhausted:
	for (int i{0}; i < iterations; ++i) {
		// Modify the population
		for (std::size_t j{0}; j < pop.size(); ++j) {
			const auto tour{selection_tournament(tournament_k, pop)};
			
			const std::size_t parent1_idx{tour[0]};
			const std::size_t parent2_idx{tour[1]};
			const std::size_t parent3_idx{tour[2]};

			Candidate child{crossbreed(pop[parent1_idx], pop[parent2_idx])};
			child = mutate(child, mutation_prob, mutation_dev);

			pop[parent3_idx] = child;
		}
		
		// Evaluate the new population
		evaluate_population(pop, dataset);

		const double current_fitness{find_best(pop).get_fitness()};

		if (print_iter) {
			std::cout.width(12);
			std::cout << "Iter: " << i << "\t\t" << "Fitness: " << current_fitness << std::endl;
		}
	}
	
	return find_best(pop);
}

struct config {
	int pop_size;
	int iteration_count;
	bool is_elitism;
	int tournament_size;
	double mutation_prob;
	double mutation_dev;
	int dataset_sel;
	std::string algorithm;
	bool print_iter;
};

void print_config(const config& c) {
	std::cout << "config:" << std::endl;
	std::cout << "\tpop_size (-p=): " << c.pop_size << std::endl;
	std::cout << "\titeration_count (-i=): " << c.iteration_count << std::endl;
	std::cout << "\tis_elitism (-e=): " << std::boolalpha << c.is_elitism << std::endl;
	std::cout << "\ttournament_size (-t=): " << c.tournament_size << std::endl;
	std::cout << "\tmutation_prob (-mp=): " << c.mutation_prob << std::endl;
	std::cout << "\tmutation_dev (-md=): " << c.mutation_dev << std::endl;
	std::cout << "\tdataset_sel (-d=): " << c.dataset_sel << std::endl;
	std::cout << "\talgorithm (-a=): " << c.algorithm << std::endl;
	std::cout << "\tprint_iter (-pi=): " << std::boolalpha << c.print_iter << std::endl;
}

config parse_config(int argc, char* argv[]) {
	config cfg{
		.pop_size = 100,
		.iteration_count = 300,
		.is_elitism = true,
		.tournament_size = 5,
		.mutation_prob = 0.5,
		.mutation_dev = 1.2,
		.dataset_sel = 2,
		.algorithm = "gen",
		.print_iter = false
	};
	if (argc <= 1) {
		return cfg;
	}
	for (int i{1}; i < argc; ++i) {
		std::string arg(argv[i]);

		const auto idx{arg.find_first_of('=')};
		if (idx == std::string::npos) {
			// There is not equals sign
			continue;
		}
		std::string_view selector(argv[i], idx);
		if (selector.length() == 0) {
			// There is not selector
			continue;
		}
		std::string value(argv[i] + idx + 1);
		if (value.length() == 0) {
			// There is not value
			continue;
		}

		if (selector == "-p") {
			cfg.pop_size = std::stoi(value);
		} else if (selector == "-i") {
			cfg.iteration_count = std::stoi(value);
		} else if (selector == "-e") {
			if (value == "true") {
				cfg.is_elitism = true;
			} else if (value == "false") {
				cfg.is_elitism = false;
			}
		} else if (selector == "-t") {
			cfg.tournament_size = std::stoi(value);
		} else if (selector == "-mp") {
			cfg.mutation_prob = std::stod(value);
		} else if (selector == "-md") {
			cfg.mutation_dev = std::stod(value);
		} else if (selector == "-d") {
			cfg.dataset_sel = std::stoi(value);
		} else if (selector == "-a") {
			cfg.algorithm = value;
		} else if (selector == "-pi") {
			if (value == "true") {
				cfg.print_iter = true;
			} else if (value == "false") {
				cfg.print_iter = false;
			}
		}
	}
	return cfg;
}

void print_candidate(const Candidate& c) {
	std::cout << "Candidate: ";
	for (int i{0}; i < 5; ++i) {
		std::cout << c.get_chromosome()[i] << ", ";
	}
	std::cout << std::endl;
}

int main(int argc, char* argv[]) {
	const config c{parse_config(argc, argv)};
	print_config(c);

	const auto& dataset{dataset_load(c.dataset_sel)};

	Candidate best;
	if (c.algorithm == "gen") {
		best = generational_genetic_algorithm(
			c.pop_size, c.iteration_count, c.is_elitism, c.mutation_prob, c.mutation_dev, dataset, c.print_iter
		);
	} else if (c.algorithm == "elim") {
		best = eliminational_genetic_algorithm(
			c.pop_size, c.iteration_count, c.mutation_prob, c.mutation_dev, c.tournament_size, dataset, c.print_iter
		);
	} else {
		throw std::invalid_argument("unrecognized algorithm, allowed values are: \"gen\", \"elem\"");
	}

	print_candidate(best);

	std::cout << "Loss: " << loss(dataset, best.get_chromosome()) << std::endl;

	return EXIT_SUCCESS;
}
