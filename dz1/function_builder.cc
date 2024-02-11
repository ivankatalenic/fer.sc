#include "function_builder.hh"

#include "lamma_function.hh"
#include "lambda_function.hh"

IntUnaryFunction* FunctionBuilder::lFunction(int max, int min) {
	return new LammaFunction(max, min, false);
}

IntUnaryFunction* FunctionBuilder::gammaFunction(int min, int max) {
	return new LammaFunction(min, max, true);
}

IntUnaryFunction* FunctionBuilder::lambdaFunction(int left, int mid, int right) {
	return new LambdaFunction(left, mid, right);
}
