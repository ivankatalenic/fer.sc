#pragma once

class IntUnaryFunction {
public:
	virtual double valueAt(int) const = 0;

	virtual ~IntUnaryFunction() {};
};
