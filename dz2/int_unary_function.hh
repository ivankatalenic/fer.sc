#pragma once

class IntUnaryFunction {
public:
	virtual double valueAt(int) = 0;

	virtual ~IntUnaryFunction() {};
};
