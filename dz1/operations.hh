#pragma once

#include "fuzzy_set_interface.hh"
#include "fuzzy_unary_function.hh"
#include "fuzzy_binary_function.hh"

namespace Operations {
	FuzzySetInterface* unaryOperation(FuzzySetInterface* s, FuzzyUnaryFunction* f);
	FuzzySetInterface* binaryOperation(FuzzySetInterface* s1, FuzzySetInterface* s2, FuzzyBinaryFunction* f);
	
	FuzzyUnaryFunction* zadehNot();
	FuzzyBinaryFunction* zadehAnd();
	FuzzyBinaryFunction* zadehOr();

	FuzzyBinaryFunction* hamacherTNorm(double v);
	FuzzyBinaryFunction* hamacherSNorm(double v);
};
