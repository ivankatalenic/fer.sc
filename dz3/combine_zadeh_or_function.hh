#pragma once

#include "int_unary_function.hh"

#include "lamma_function.hh"

class CombineZadehOrFunction : public IntUnaryFunction {
public:
	CombineZadehOrFunction(
		IntUnaryFunction const* a,
		IntUnaryFunction const* b
	);

	double valueAt(int) const override;
private:
	IntUnaryFunction const* a;
	IntUnaryFunction const* b;
};
