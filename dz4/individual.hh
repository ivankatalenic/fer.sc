#pragma once

#include <array>

template<class T>
class Individual {
public:
	Individual() = default;
	Individual(T chromosome, double fitness = 0.0);

	T		get_chromosome() const;
	double	get_fitness() const;

	void set_fitness(const double f);
private:
	T      chromosome;
	double fitness;
};

template<class T>
Individual<T>::Individual(T chromosome, double fitness): chromosome{chromosome}, fitness{fitness} {}

template<class T>
T Individual<T>::get_chromosome() const {
	return chromosome;
}

template<class T>
double Individual<T>::get_fitness() const {
	return fitness;
}

template<class T>
void Individual<T>::set_fitness(const double f) {
	fitness = f;
}
