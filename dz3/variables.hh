#pragma once

#include "int_unary_function.hh"

namespace Var {
	extern IntUnaryFunction const* distance_close;
	extern IntUnaryFunction const* distance_medium;
	extern IntUnaryFunction const* distance_far;
	extern IntUnaryFunction const* speed_small;
	extern IntUnaryFunction const* speed_medium;
	extern IntUnaryFunction const* speed_big;
	extern IntUnaryFunction const* direction_right;
	extern IntUnaryFunction const* direction_wrong;
	extern IntUnaryFunction const* accel_nb;
	extern IntUnaryFunction const* accel_ns;
	extern IntUnaryFunction const* accel_zo;
	extern IntUnaryFunction const* accel_ps;
	extern IntUnaryFunction const* accel_pb;
	extern IntUnaryFunction const* omega_nb;
	extern IntUnaryFunction const* omega_ns;
	extern IntUnaryFunction const* omega_zo;
	extern IntUnaryFunction const* omega_ps;
	extern IntUnaryFunction const* omega_pb;
	extern IntUnaryFunction const* ignore;
}
