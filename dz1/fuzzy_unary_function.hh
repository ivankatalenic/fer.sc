#pragma once

class FuzzyUnaryFunction {
public:
	virtual double valueAt(double) const = 0;

	virtual ~FuzzyUnaryFunction() {};
};
