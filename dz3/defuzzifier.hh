#pragma once

#include "fuzzy_set_interface.hh"

class Defuzzifier {
public:
	virtual int defuzzy(FuzzySetInterface* fs) const = 0;

	virtual ~Defuzzifier() {};
};
