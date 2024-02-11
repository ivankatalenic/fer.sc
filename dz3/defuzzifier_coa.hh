#pragma once

#include "defuzzifier.hh"

class DefuzzifierCOA : public Defuzzifier {
public:
	int defuzzy(FuzzySetInterface* fs) const override;
};
