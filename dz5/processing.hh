#pragma once

#include "position.hh"

#include <vector>

namespace Processing {
	std::vector<Position> process_positions(
		const std::vector<Position>& positions,
		const int repre_cnt
	);
}
