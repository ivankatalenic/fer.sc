#pragma once

#include "domain_element.hh"
#include "domain_interface.hh"

class DomainBuilder {
public:
	static DomainInterface* intRange(int first, int last);
	static DomainInterface* combine(DomainInterface* a, DomainInterface* b);
};
