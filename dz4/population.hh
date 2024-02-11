#pragma once

#include "individual.hh"

#include <vector>

template<class T>
class Population {
public:
	Population(std::vector<Individual<T>> initial_pop);

	Individual<T> find_best() const;
private:
	std::vector<Individual<T>> individuals;
};


