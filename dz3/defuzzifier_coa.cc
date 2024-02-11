#include "defuzzifier_coa.hh"

#include "floating_point.hh"

int DefuzzifierCOA::defuzzy(FuzzySetInterface* fs) const {
	DomainInterface* d{fs->getDomain()};
	if (d->getNumberOfComponents() != 1) {
		throw std::invalid_argument("can't defuzzy the fuzzy set with more than one domain component");
	}
	if (d->getCardinality() == 0) {
		throw std::invalid_argument("can't defuzzy the fuzzy set with no elements");
	}

	double sum_upper{0};
	double sum_lower{0};
	for (int i{0}; i < d->getCardinality(); ++i) {
		const DomainElement& e{d->elementForIndex(i)};

		sum_upper += e.getComponentValue(0) * fs->getValueAt(e);
		sum_lower += fs->getValueAt(e);
	}

	if (FloatingPoint::isEqual(sum_lower, 0)) {
		return 0;
	}

	return sum_upper / sum_lower;
}
