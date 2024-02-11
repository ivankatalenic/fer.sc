#include "relations.hh"

#include "domain_interface.hh"
#include "floating_point.hh"
#include "domain_builder.hh"
#include "mutable_fuzzy_set.hh"

#include <stdexcept>
#include <algorithm>

bool Relations::isUxU(FuzzySetInterface* relation) {
	if (relation == nullptr) {
		throw std::invalid_argument("the relation is null");
	}

	DomainInterface* domain{relation->getDomain()};
	if (domain == nullptr) {
		throw std::invalid_argument("the domain is null");
	}

	if (domain->getNumberOfComponents() != 2) {
		return false;
	}

	const DomainInterface* first{domain->getComponent(0)};
	const DomainInterface* second{domain->getComponent(1)};

	if (first == nullptr || second == nullptr) {
		throw std::invalid_argument("the relation subdomain is null");
	}

	return *first == *second;
}

bool Relations::isReflexive(FuzzySetInterface* relation) {
	if (!Relations::isUxU(relation)) {
		throw std::invalid_argument("the relation isn't of U*U kind");
	}

	DomainInterface* domain{relation->getDomain()};
	const DomainInterface* u{domain->getComponent(0)};

	for (int i{0}; i < u->getCardinality(); ++i) {
		const DomainElement& e{u->elementForIndex(i)};
		const int            e_int{e.getComponentValue(0)};

		const double mi{relation->getValueAt({e_int, e_int})};

		if (!FloatingPoint::isEqual(mi, 1.0)) {
			return false;
		}
	}

	return true;
}

bool Relations::isSymmetric(FuzzySetInterface* relation) {
	if (!Relations::isUxU(relation)) {
		throw std::invalid_argument("the relation isn't of U*U kind");
	}

	DomainInterface* domain{relation->getDomain()};
	const DomainInterface* u{domain->getComponent(0)};
	const int card{u->getCardinality()};

	for (int i{0}; i < card - 1; ++i) {
		const DomainElement& first{u->elementForIndex(i)};
		const int            first_int{first.getComponentValue(0)};
		
		for (int j{i + 1}; j < card; ++j) {
			const DomainElement& second{u->elementForIndex(j)};
			const int            second_int{second.getComponentValue(0)};

			const double mi_norm{relation->getValueAt({first_int, second_int})};
			const double mi_inv {relation->getValueAt({second_int, first_int})};
			
			if (!FloatingPoint::isEqual(mi_norm, mi_inv)) {
				return false;
			}
		}
	}

	return true;
}

bool Relations::isMaxMinTransitive(FuzzySetInterface* relation) {
	if (!Relations::isUxU(relation)) {
		throw std::invalid_argument("the relation isn't of U*U kind");
	}

	DomainInterface* domain{relation->getDomain()};
	const DomainInterface* u{domain->getComponent(0)};
	const int card{u->getCardinality()};

	for (int i{0}; i < card; ++i) {
		const DomainElement& x{u->elementForIndex(i)};
		const int            x_int{x.getComponentValue(0)};

		for (int j{0}; j < card; ++j) {
			const DomainElement& z{u->elementForIndex(j)};
			const int            z_int{z.getComponentValue(0)};

			double max{0.0};

			for (int k{0}; k < card; ++k) {
				const DomainElement& y{u->elementForIndex(k)};
				const int            y_int{y.getComponentValue(0)};

				const double mi_xy{relation->getValueAt({x_int, y_int})};
				const double mi_yz{relation->getValueAt({y_int, z_int})};

				const double min{std::min(mi_xy, mi_yz)};

				if (min > max) {
					max = min;
				}
			}

			const double mi_xz{relation->getValueAt({x_int, z_int})};
			if (mi_xz < max) {
				return false;
			}
		}
	}

	return true;
}

bool Relations::isFuzzyEquivalence(FuzzySetInterface* relation) {
	return isReflexive(relation) && isSymmetric(relation) && isMaxMinTransitive(relation);
}

FuzzySetInterface* Relations::compositionOfBinaryRelations(FuzzySetInterface* r1, FuzzySetInterface* r2) {
	if (r1 == nullptr) {
		throw std::invalid_argument("the relation r1 is null");
	}
	if (r2 == nullptr) {
		throw std::invalid_argument("the relation r2 is null");
	}

	DomainInterface* d1{r1->getDomain()};
	if (d1 == nullptr) {
		throw std::invalid_argument("the domain of r1 is null");
	}
	DomainInterface* d2{r2->getDomain()};
	if (d2 == nullptr) {
		throw std::invalid_argument("the domain of r2 is null");
	}

	if (d1->getNumberOfComponents() != 2) {
		throw std::invalid_argument("the relation r1 isn't binary relation");
	}
	if (d2->getNumberOfComponents() != 2) {
		throw std::invalid_argument("the relation r2 isn't binary relation");
	}

	const DomainInterface* X{d1->getComponent(0)};
	if (X == nullptr) {
		throw std::invalid_argument("the first component of r1 domain is null");
	}
	if (X->getNumberOfComponents() != 1) {
		throw std::invalid_argument("the X domain doesn't have one component");
	}
	const DomainInterface* Y{d1->getComponent(1)};
	if (Y == nullptr) {
		throw std::invalid_argument("the second component of r1 domain is null");
	}
	if (Y->getNumberOfComponents() != 1) {
		throw std::invalid_argument("the Y domain doesn't have one component");
	}
	const DomainInterface* Y_other{d2->getComponent(0)};
	if (Y_other == nullptr) {
		throw std::invalid_argument("the first component of r2 domain is null");
	}
	if (Y_other->getNumberOfComponents() != 1) {
		throw std::invalid_argument("the Y_other domain doesn't have one component");
	}
	const DomainInterface* Z{d2->getComponent(1)};
	if (Z == nullptr) {
		throw std::invalid_argument("the second component of r2 domain is null");
	}
	if (Z->getNumberOfComponents() != 1) {
		throw std::invalid_argument("the Z domain doesn't have one component");
	}

	if (!(*Y == *Y_other)) {
		throw std::invalid_argument("the middle composition domains are not equal");
	}

	DomainInterface* resultDomain{
		DomainBuilder::combine(
			const_cast<DomainInterface*>(X),
			const_cast<DomainInterface*>(Z)
		)
	};
	MutableFuzzySet* result{new MutableFuzzySet(resultDomain)};

	for (int i{0}; i < X->getCardinality(); ++i) {
		const DomainElement& x{X->elementForIndex(i)};
		const int            x_int{x.getComponentValue(0)};

		for (int j{0}; j < Z->getCardinality(); ++j) {
			const DomainElement& z{Z->elementForIndex(j)};
			const int            z_int{z.getComponentValue(0)};

			double max{0.0};

			for (int k{0}; k < Y->getCardinality(); ++k) {
				const DomainElement& y{Y->elementForIndex(k)};
				const int            y_int{y.getComponentValue(0)};

				const double mi_xy{r1->getValueAt({x_int, y_int})};
				const double mi_yz{r2->getValueAt({y_int, z_int})};

				const double min{std::min(mi_xy, mi_yz)};

				if (min > max) {
					max = min;
				}
			}

			result->set({x_int, z_int}, max);
		}
	}

	return result;
}
