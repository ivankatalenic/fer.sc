#pragma once

#include "fuzzy_system.hh"
#include "defuzzifier.hh"
#include "calculated_fuzzy_set.hh"
#include "simple_domain.hh"
#include "int_unary_function.hh"

#include <vector>
#include <array>

class FuzzySystemProduct : public FuzzySystem {
public:
	FuzzySystemProduct(
		const Defuzzifier* df,
		std::vector<std::array<IntUnaryFunction const*, 7>> rules
	);

	int infer(
		const int left,
		const int right,
		const int left_angled,
		const int right_angled,
		const int speed,
		const int direction
	) const override;
private:
	const Defuzzifier* df;
	std::vector<std::array<IntUnaryFunction const*, 7>> rules;
};
