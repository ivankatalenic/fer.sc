#include "composite_domain.hh"

#include <stdexcept>

CompositeDomain::CompositeDomain(std::initializer_list<DomainInterface*> l): values{l} {

}

int CompositeDomain::getCardinality() const {
	int card{1};

	for (const auto& domain : values) {
		card *= domain->getCardinality();
	}

	return card;
}

const DomainInterface* CompositeDomain::getComponent(int index) const {
	if (index < 0) {
		throw std::out_of_range("index must be greater than 0");
	}
	if (static_cast<std::size_t>(index) >= values.size()) {
		throw std::out_of_range("index must be less than the number of components");
	}

	return values[index];
}

int CompositeDomain::getNumberOfComponents() const {
	return values.size();
}

int CompositeDomain::indexOfElement(const DomainElement& e) const {
	const int comp_n{getNumberOfComponents()};
	if (e.getNumberOfComponents() != comp_n) {
		throw std::invalid_argument("the provided domain element has incompatible number of components");
	}

	int index{0};
	for (int i{0}; i < comp_n; ++i) {
		const DomainInterface* d{values[i]};

		const int val_index{d->indexOfElement({e.getComponentValue(i)})};
		if (val_index == DomainInterface::ELEMENT_NOT_PRESENT) {
			return DomainInterface::ELEMENT_NOT_PRESENT;
		}

		index = index * d->getCardinality() + val_index;
	}

	return index;
}

DomainElement CompositeDomain::elementForIndex(int index) const {
	if (index < 0) {
		throw std::out_of_range("index must be greater than 0");
	}
	if (index >= getCardinality()) {
		throw std::out_of_range("index must be less than the domain's cardinality");
	}

	std::vector<int> result;
	for (auto iter{values.crbegin()}; iter != values.crend(); ++iter) {
		const DomainInterface* d{*iter};
		const int card{d->getCardinality()};

		const int val_index{index % card};
		const int val{d->elementForIndex(val_index).getComponentValue(0)};
		result.insert(result.begin(), val);

		index /= card;
	}

	return DomainElement(result);
}
