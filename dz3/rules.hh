#pragma once

#include "int_unary_function.hh"

#include <vector>
#include <array>

namespace Rules {
	std::vector<std::array<IntUnaryFunction const*, 7>> get_for_accel();
	std::vector<std::array<IntUnaryFunction const*, 7>> get_for_omega();
}
