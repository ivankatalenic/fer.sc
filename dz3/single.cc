#include "defuzzifier.hh"
#include "defuzzifier_coa.hh"
#include "fuzzy_system.hh"
#include "fuzzy_system_min.hh"
#include "fuzzy_system_product.hh"
#include "rules.hh"

#include <cstdio> 
#include <cstring>
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[]) {
	if (argc != 8) {
		throw std::length_error("the number of expected arguments is 7");
	}

	const int rule_idx{atoi(argv[1])};

	const int left			{atoi(argv[2])};
	const int right			{atoi(argv[3])};
	const int left_angled	{atoi(argv[4])};
	const int right_angled	{atoi(argv[5])};
	const int speed			{atoi(argv[6])};
	const int direction		{atoi(argv[7])};

	Defuzzifier* def{new DefuzzifierCOA()};
	FuzzySystem* fs_omega{new FuzzySystemProduct(def, {Rules::get_for_omega()[rule_idx]})};

	const int omega{fs_omega->infer(left, right, left_angled, right_angled, speed, direction)};
	std::cout << "omega: " << omega << std::endl;

	return 0;
}
