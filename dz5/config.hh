#pragma once

#include <vector>

enum Training {
	STOCHASTIC = 1,
	GROUP = 2,
	MINI_BATCHES = 3,
};

struct Config {
	Training type;
	std::vector<int> architecture;
};
