#include "domain_builder.hh"

#include "simple_domain.hh"
#include "composite_domain.hh"

DomainInterface* DomainBuilder::intRange(int first, int last) {
	return new SimpleDomain(first, last);
}

DomainInterface* DomainBuilder::combine(DomainInterface* a, DomainInterface* b) {
	return new CompositeDomain{a, b};
}
