#pragma once

#include "fuzzy_unary_function.hh"
#include "fuzzy_binary_function.hh"

#include <stdexcept>

double min(double a, double b) {
	if (a <= b) {
		return a;
	}
	return b;
}

double max(double a, double b) {
	if (a >= b) {
		return a;
	}
	return b;
}

class ZadehNot : public FuzzyUnaryFunction {
public:
	double valueAt(double v) const override {
		return 1.0 - v;
	};
};

class ZadehAnd : public FuzzyBinaryFunction {
public:
	double valueAt(double v1, double v2) const override {
		return min(v1, v2);
	};
};

class ZadehOr : public FuzzyBinaryFunction {
public:
	double valueAt(double v1, double v2) const override {
		return max(v1, v2);
	};
};

class HamacherTNorm : public FuzzyBinaryFunction {
public:
	HamacherTNorm(double par): p{par} {
		if (p < 0) {
			throw std::invalid_argument("the parameter must be greater than 0");
		}
	}
	double valueAt(double a, double b) const override {
		const double ab{a * b};

		return ab / (p + (1.0 - p) * (a + b - ab));
	};
private:
	double p;
};

class HamacherSNorm : public FuzzyBinaryFunction {
public:
	HamacherSNorm(double par): p{par} {
		if (p < 0) {
			throw std::invalid_argument("the parameter must be greater than 0");
		}
	}
	double valueAt(double a, double b) const override {
		const double ab{a * b};

		return (a + b - (2.0 - p) * ab) / (1.0 - (1.0 - p) * ab);
	};
private:
	double p;
};
