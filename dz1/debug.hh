#pragma once

#include "domain_interface.hh"
#include "fuzzy_set_interface.hh"
#include "domain_element.hh"

#include <string>
#include <iostream>

namespace Debug {
	void print(DomainInterface* d, const std::string& desc) {
		if (!desc.empty()) {
			std::cout << desc << std::endl;
		}

		for (int i{0}; i < d->getCardinality(); ++i) {
			std::cout << "Domain element: " << d->elementForIndex(i) << std::endl;
		}

		std::cout << "Domain cardinality: " << d->getCardinality() << std::endl << std::endl;
	}
	void printFuzzy(FuzzySetInterface* f, const std::string& desc) {
		if (!desc.empty()) {
			std::cout << desc << std::endl;
		}

		const DomainInterface* d{f->getDomain()};
		for (int i{0}; i < d->getCardinality(); ++i) {
			const DomainElement& e{d->elementForIndex(i)};
			std::cout << "d[" << e << "] = " << f->getValueAt(e) << std::endl;
		}
	}
};
