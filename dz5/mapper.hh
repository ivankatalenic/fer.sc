#pragma once

#include "matrix.hh"
#include "position.hh"
#include "gesture.hh"

#include <string>
#include <vector>
#include <utility>

namespace Mapper {
	Matrix      map_label(const std::string& label);
	std::string map_to_label(const Matrix& m);
	Matrix      map_positions(const std::vector<Position>& positions);
	
	std::pair<std::vector<Matrix>,std::vector<Matrix>> map_gestures(const std::vector<Gesture>& gestures);
}
