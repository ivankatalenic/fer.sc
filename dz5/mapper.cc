#include "mapper.hh"

#include <stdexcept>

Matrix Mapper::map_label(const std::string& label) {
	if (label == "Alpha") {
		return Matrix{{1}, {0}, {0}, {0}, {0}};
	}
	if (label == "Beta") {
		return Matrix{{0}, {1}, {0}, {0}, {0}};
	}
	if (label == "Gamma") {
		return Matrix{{0}, {0}, {1}, {0}, {0}};
	}
	if (label == "Delta") {
		return Matrix{{0}, {0}, {0}, {1}, {0}};
	}
	if (label == "Epsilon") {
		return Matrix{{0}, {0}, {0}, {0}, {1}};
	}
	throw std::invalid_argument("the label is invalid");
	return Matrix(5, 1);
}

std::string Mapper::map_to_label(const Matrix& m) {
	int    best_index{0};
	double best_value{m[best_index][0]};
	for (int i{1}; i < m.getRows(); ++i) {
		const double value{m[i][0]};
		if (value > best_value) {
			best_index = i;
			best_value = value;
		}
	}

	switch (best_index) {
		case 0:
			return "Alpha";
		case 1:
			return "Beta";
		case 2:
			return "Gamma";
		case 3:
			return "Delta";
		case 4:
			return "Epsilon";
	}
	return "Unknown";
}

Matrix Mapper::map_positions(const std::vector<Position>& positions) {
	const int len{static_cast<int>(positions.size())};
	Matrix ret(2 * len, 1);
	for (int i{0}; i < len; ++i) {
		ret[2 * i][0]     = positions[i].x;
		ret[2 * i + 1][0] = positions[i].y;
	}
	return ret;
}

std::pair<std::vector<Matrix>,std::vector<Matrix>> Mapper::map_gestures(const std::vector<Gesture>& gestures) {
	std::vector<Matrix> samples;
	std::vector<Matrix> labels;
	for (const Gesture& gesture : gestures) {
		samples.push_back(map_positions(gesture.positions));
		labels.push_back(map_label(gesture.label));
	}
	return std::make_pair(samples, labels);
}
