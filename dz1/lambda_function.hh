#pragma once

#include "int_unary_function.hh"

#include "lamma_function.hh"

class LambdaFunction : public IntUnaryFunction {
public:
	LambdaFunction(int left, int mid, int right);

	double valueAt(int) override;
private:
	int left;
	int mid;
	int right;

	LammaFunction rise;
	LammaFunction fall;
};
