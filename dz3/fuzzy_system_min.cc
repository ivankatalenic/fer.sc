#include "fuzzy_system_min.hh"

#include <stdexcept>
#include <algorithm>

FuzzySystemMin::FuzzySystemMin(
	const Defuzzifier* d,
	std::vector<std::array<IntUnaryFunction const*, 7>> r
):
	df(d), rules(r) {
	if (df == nullptr) {
		throw std::invalid_argument("the provided fuzzifier is null");
	}
	if (rules.size() == 0) {
		throw std::invalid_argument("there are no rules provided");
	}
}

struct ResultFunc : public IntUnaryFunction {
	std::vector<std::array<IntUnaryFunction const*, 7>> rules;
	std::array<int, 6> args;

	ResultFunc(
		std::vector<std::array<IntUnaryFunction const*, 7>> r,
		std::array<int, 6> a
	): rules{r}, args{a} {};

	double valueAt(int y) const override {
		double max{0};
		for (const auto& rule : rules) {
			const double val{
				std::min({
					rule[0]->valueAt(args[0]),
					rule[1]->valueAt(args[1]),
					rule[2]->valueAt(args[2]),
					rule[3]->valueAt(args[3]),
					rule[4]->valueAt(args[4]),
					rule[5]->valueAt(args[5]),
					rule[6]->valueAt(y)
				})
			};
			
			if (val > max) {
				max = val;
			}
		}
		return max;
	}

};

int FuzzySystemMin::infer(
	const int left,
	const int right,
	const int left_angled,
	const int right_angled,
	const int speed,
	const int direction
) const {
	// Create a new fuzzy set
	DomainInterface* domain{new SimpleDomain(-400, 400)};
	IntUnaryFunction* func{new ResultFunc{rules, {left, right, left_angled, right_angled, speed, direction}}};
	FuzzySetInterface* result{new CalculatedFuzzySet(domain, func)};

	// Defuzzy the set
	return df->defuzzy(result);
}
