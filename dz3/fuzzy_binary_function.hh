#pragma once

class FuzzyBinaryFunction {
public:
	virtual double valueAt(double, double) const = 0;

	virtual ~FuzzyBinaryFunction() {};
};
