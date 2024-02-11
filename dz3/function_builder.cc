#include "function_builder.hh"

#include "lamma_function.hh"
#include "lambda_function.hh"
#include "constant_function.hh"
#include "combine_zadeh_or_function.hh"

IntUnaryFunction const* FunctionBuilder::lFunction(int max, int min) {
	return new LammaFunction(max, min, false);
}

IntUnaryFunction const* FunctionBuilder::gammaFunction(int min, int max) {
	return new LammaFunction(min, max, true);
}

IntUnaryFunction const* FunctionBuilder::lambdaFunction(int left, int mid, int right) {
	return new LambdaFunction(left, mid, right);
}

IntUnaryFunction const* FunctionBuilder::constantFunction() {
	return new ConstantFunction();
}

IntUnaryFunction const* FunctionBuilder::combineZadehOr(
	IntUnaryFunction const* a,
	IntUnaryFunction const* b
) {
	return new CombineZadehOrFunction(a, b);
}
