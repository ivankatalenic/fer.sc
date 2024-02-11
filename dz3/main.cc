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

int main() {

	Defuzzifier* def{new DefuzzifierCOA()};
	FuzzySystem* fs_accel{new FuzzySystemProduct(def, Rules::get_for_accel())};
	FuzzySystem* fs_omega{new FuzzySystemProduct(def, Rules::get_for_omega())};

	while (true) {
		char buff[1000];
		fgets(buff, 1000, stdin);
		if(buff[0] == 'K') {
			break;
		}

		int left, right, left_angled, right_angled, speed, direction;
		sscanf(
			buff,
			"%d %d %d %d %d %d",
			&left,
			&right,
			&left_angled,
			&right_angled,
			&speed,
			&direction
		);
		
		const int accel{fs_accel->infer(left, right, left_angled, right_angled, speed, direction)};
		std::cerr << "accel: " << accel << '\n';
		const int omega{fs_omega->infer(left, right, left_angled, right_angled, speed, direction)};
		std::cerr << "omega: " << omega << std::endl;
		
		fprintf(stdout, "%d %d\n", accel, omega);
		fflush(stdout);
	}

	return 0;
}
