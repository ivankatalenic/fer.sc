#pragma once

#include "int_unary_function.hh"

class FunctionBuilder {
public:
	static IntUnaryFunction* lFunction(int max, int min);
	static IntUnaryFunction* gammaFunction(int min, int max);
	static IntUnaryFunction* lambdaFunction(int left, int mid, int right);
};
