#pragma once

#include "int_unary_function.hh"

class LammaFunction : public IntUnaryFunction {
public:
	LammaFunction(int left, int right, bool isRising);

	double valueAt(int) const override;
private:
	int left;
	int right;
	bool isRising;
};
