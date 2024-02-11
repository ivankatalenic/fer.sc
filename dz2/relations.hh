#pragma once

#include "fuzzy_set_interface.hh"

namespace Relations {
	bool isUxU(FuzzySetInterface* relation);
	bool isReflexive(FuzzySetInterface* relation);
	bool isSymmetric(FuzzySetInterface* relation);
	bool isMaxMinTransitive(FuzzySetInterface* relation);
	bool isFuzzyEquivalence(FuzzySetInterface* relation);
	
	FuzzySetInterface* compositionOfBinaryRelations(FuzzySetInterface* r1, FuzzySetInterface* r2);
};
