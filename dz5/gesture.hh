#pragma once

#include "position.hh"

#include <string>
#include <vector>

struct Gesture {
	std::string label;
	std::vector<Position> positions;
};
