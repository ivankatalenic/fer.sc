#include <iostream>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <random>

struct rule_params {
	double a;
	double b;
	double c;
	double d;

	double p;
	double q;
	double r;

	rule_params operator+(const rule_params& other) const {
		return rule_params{
			.a = a + other.a,
			.b = b + other.b,
			.c = c + other.c,
			.d = d + other.d,
			.p = p + other.p,
			.q = q + other.q,
			.r = r + other.r
		};
	}
};

rule_params get_empty_gradients() {
	return {.a = 0, .b = 0, .c = 0, .d = 0, .p = 0, .q = 0, .r = 0};
}

std::vector<rule_params> get_params(const int rules_count) {
	std::vector<rule_params> ret(static_cast<std::size_t>(rules_count));

	std::random_device 					rd;
    std::mt19937 						gen(rd());
    std::uniform_real_distribution<> 	dis(-1.0, 1.0);
	for (rule_params& param : ret) {
		param.a = dis(gen);
		param.b = dis(gen);
		param.c = dis(gen);
		param.d = dis(gen);
		param.p = dis(gen);
		param.q = dis(gen);
		param.r = dis(gen);
	}

	return ret;
}

struct sample {
	double x;
	double y;

	double f;
};

double fun(double x, double y) {
	const double left_fact	{pow(x - 1, 2) + pow(y + 2, 2) - 5 * x * y + 3};
	const double right_fact	{pow(cos(x / 5), 2)};
	return left_fact * right_fact;
}

std::vector<sample> get_samples() {
	std::vector<sample> ret(81);
	for (int i{-4}; i <= 4; ++i) {
		for (int j{-4}; j <= 4; ++j) {
			const int idx{(i + 4) * 9 + (j + 4)};
			ret[idx] = {
				.x = static_cast<double>(i),
				.y = static_cast<double>(j),
				.f = fun(static_cast<double>(i), static_cast<double>(j))
			};
		}
	}
	return ret;
}

struct rule_cache {
	double mu_a;
	double mu_b;
	double alpha;
	double z;
	double dout_dalpha;
};

struct cache {
	std::vector<rule_cache> rules;

	double alpha_sum;
	double alpha_sum_squared;
	double output;
	double out_neg_diff;
};

double sigmoid(double x) {
	return 1.0 / (1.0 + exp(-x));
}

cache compute_cache(const sample& sample, const std::vector<rule_params>& params) {
	cache cache{.rules = std::vector<rule_cache>(params.size())};

	cache.alpha_sum = 0.0;
	for (std::size_t i{0ull}; i < params.size(); ++i) {
		const rule_params&	rule_param{params[i]};
		rule_cache& 		rule_cache{cache.rules[i]};

		rule_cache.mu_a 	= sigmoid(-rule_param.b * (sample.x - rule_param.a));
		rule_cache.mu_b 	= sigmoid(-rule_param.d * (sample.y - rule_param.c));
		rule_cache.alpha 	= rule_cache.mu_a * rule_cache.mu_b;
		rule_cache.z		= rule_param.p * sample.x + rule_param.q * sample.y + rule_param.r;

		cache.alpha_sum += rule_cache.alpha;
	}
	cache.alpha_sum_squared = pow(cache.alpha_sum, 2);

	cache.output = 0.0;
	for (rule_cache& rc : cache.rules) {
		rc.dout_dalpha = 0.0;
		for (const rule_cache& other_cache : cache.rules) {
			rc.dout_dalpha += other_cache.alpha * (rc.z - other_cache.z);
		}
		rc.dout_dalpha /= cache.alpha_sum_squared;

		cache.output += rc.alpha * rc.z;
	}
	cache.output /= cache.alpha_sum;
	cache.out_neg_diff = cache.output - sample.f;

	return cache;
}

std::vector<rule_params> compute_gradients(const sample& sample, const std::vector<rule_params>& params, const cache& cache) {
	std::vector<rule_params> gradients(params.size());

	for (std::size_t i{0ull}; i < params.size(); ++i) {
		rule_params& 		gradient	{gradients[i]};
		const rule_params& 	rule_param	{params[i]};
		const rule_cache& 	rule_cache	{cache.rules[i]};

		const double alpha_ratio{rule_cache.alpha / cache.alpha_sum};

		gradient.p = cache.out_neg_diff * alpha_ratio * sample.x;
		gradient.q = cache.out_neg_diff * alpha_ratio * sample.y;
		gradient.r = cache.out_neg_diff * alpha_ratio;

		gradient.a = cache.out_neg_diff * rule_cache.dout_dalpha * rule_cache.alpha * rule_param.b * (1 - rule_cache.mu_a);
		gradient.b = cache.out_neg_diff * rule_cache.dout_dalpha * rule_cache.alpha * (rule_param.a - sample.x) * (1 - rule_cache.mu_a);
		gradient.c = cache.out_neg_diff * rule_cache.dout_dalpha * rule_cache.alpha * rule_param.d * (1 - rule_cache.mu_b);
		gradient.b = cache.out_neg_diff * rule_cache.dout_dalpha * rule_cache.alpha * (rule_param.c - sample.y) * (1 - rule_cache.mu_b);
	}

	return gradients;
}

void adjust_params(
	std::vector<rule_params>&		params,
	const std::vector<rule_params>&	gradients,
	const double					learning_rate
) {
	for (std::size_t i{0ll}; i < params.size(); ++i) {
		rule_params& 		rule_param	{params[i]};
		const rule_params& 	gradient	{gradients[i]};

		rule_param.a += -gradient.a * learning_rate;
		rule_param.b += -gradient.b * learning_rate;
		rule_param.c += -gradient.c * learning_rate;
		rule_param.d += -gradient.d * learning_rate;
		
		rule_param.p += -gradient.p * learning_rate;
		rule_param.q += -gradient.q * learning_rate;
		rule_param.r += -gradient.r * learning_rate;
	}
}

std::vector<sample> compute_values(const std::vector<sample>& samples, const std::vector<rule_params>& params) {
	std::vector<sample> ret(samples.size());
	for (std::size_t i{0ull}; i < samples.size(); i++) {
		const sample& s{samples[i]};
		sample& value{ret[i]};
		value = s;

		const cache c{compute_cache(s, params)};
		value.f = c.output;
	}
	return ret;
}

void print_values(const std::vector<sample>& values) {
	std::cout << '[';
	for (std::size_t i{0ull}; i < values.size(); i++) {
		const sample& s{values[i]};
		
		std::cout << '[' << s.x << ',' << s.y << ',' << s.f << ']';
		if (i != values.size() - 1) {
			std::cout << ',';
		}
	}
	std::cout << ']' << std::endl;
}

void add_gradients(std::vector<rule_params>& dest, const std::vector<rule_params>& src) {
	for (std::size_t i{0ull}; i < dest.size(); i++) {
		dest[i] = dest[i] + src[i];
	}
}

void print_gradients(const rule_params& grad) {
	std::cout << '('
	<< grad.a << ',' << grad.b << ',' << grad.c << ',' << grad.d << ','
	<< grad.p << ',' << grad.q << ',' << grad.r
	<< ')'
	<< std::endl;
}

int main(int argc, char* argv[]) {
	if (argc != 4) {
		std::cerr << "Usage: ./program <num_of_rules> <num_of_iterations> <learning_rate>" << std::endl;
		return -1;
	}
	
	const int 		rules_count		{atoi(argv[1])};
	const int 		iteration_count	{atoi(argv[2])};
	const double 	learning_rate	{atof(argv[3])};

	{
		if (rules_count < 1) 		throw std::invalid_argument("the number of rules has to be greater than 1");
		if (iteration_count < 1)	throw std::invalid_argument("the number of iterations has to be greater than 1");
		if (!(learning_rate > 0))	throw std::invalid_argument("the learning rate has to be greater than 0");
	}

	// Generate initial parameters
	std::vector<rule_params> params{get_params(rules_count)};
	// Retrieve training samples
	const std::vector<sample> samples{get_samples()};

	for (int i{0}; i < iteration_count; ++i) {
		double error{0.0};

		std::vector<rule_params> gradients(params.size(), get_empty_gradients());
		for (const sample& sample : samples) {
			const cache 					cache			{compute_cache(sample, params)};
			const std::vector<rule_params> 	sample_gradients{compute_gradients(sample, params, cache)};
			add_gradients(gradients, sample_gradients);

			const double sample_error{pow(sample.f - cache.output, 2)};
			error += sample_error;
		}
		adjust_params(params, gradients, learning_rate);

		error /= samples.size();

		std::cerr << "Iteration: " << i << ", Error: " << error << std::endl;
	}

	print_values(compute_values(samples, params));

	return 0;
}

int main_stochastic(int argc, char* argv[]) {
	if (argc != 4) {
		std::cerr << "Usage: ./program <num_of_rules> <num_of_iterations> <learning_rate>" << std::endl;
		return -1;
	}
	
	const int 		rules_count		{atoi(argv[1])};
	const int 		iteration_count	{atoi(argv[2])};
	const double 	learning_rate	{atof(argv[3])};

	{
		if (rules_count < 1) 		throw std::invalid_argument("the number of rules has to be greater than 1");
		if (iteration_count < 1)	throw std::invalid_argument("the number of iterations has to be greater than 1");
		if (!(learning_rate > 0))	throw std::invalid_argument("the learning rate has to be greater than 0");
	}

	// Generate initial parameters
	std::vector<rule_params> params{get_params(rules_count)};
	// Retrieve training samples
	const std::vector<sample>      samples{get_samples()};

	for (int i{0}; i < iteration_count; ++i) {
		double error{0.0};

		for (const sample& sample : samples) {
			const cache cache{compute_cache(sample, params)};
			// Compute gradients
			const std::vector<rule_params> gradients{compute_gradients(sample, params, cache)};
			// Adjust parameters
			adjust_params(params, gradients, learning_rate);

			const double sample_error{pow(sample.f - cache.output, 2)};
			error += sample_error;
		}

		error /= samples.size();

		std::cerr << "Iteration: " << i << ", Error: " << error << std::endl;
	}

	print_values(compute_values(samples, params));

	return 0;
}
