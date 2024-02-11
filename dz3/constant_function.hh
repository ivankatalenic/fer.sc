#pragma once

#include "int_unary_function.hh"

class ConstantFunction : public IntUnaryFunction {
public:
	ConstantFunction() = default;

	double valueAt(int) const override;
};
