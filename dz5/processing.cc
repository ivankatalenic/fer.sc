#include "processing.hh"

#include <stdexcept>
#include <cmath>

static Position positions_average(const std::vector<Position>& positions) {
	const std::size_t size{positions.size()};
	if (size < 1) {
		throw std::invalid_argument("the positions size can't be zero while calculating average");
	}

	Position sum{0.0, 0.0};
	for (const Position& pos : positions) {
		sum.x += pos.x;
		sum.y += pos.y;
	}
	return {sum.x / size, sum.y / size};
}

static std::vector<Position> positions_zero(const std::vector<Position>& positions) {
	const auto& avg{positions_average(positions)};
	std::vector<Position> ret;
	for (const Position& pos : positions) {
		ret.push_back({pos.x - avg.x, pos.y - avg.y});
	}
	return ret;
}

static std::vector<Position> positions_normalize(const std::vector<Position>& positions) {
	double max{1e-9};
	for (const Position& pos : positions) {
		double x{std::abs(pos.x)};
		double y{std::abs(pos.y)};

		if (x > max) {
			max = x;
		}
		if (y > max) {
			max = y;
		}
	}

	std::vector<Position> ret;
	for (const Position& pos : positions) {
		ret.push_back({pos.x / max, pos.y / max});
	}
	return ret;
}

static double distance(const Position& a, const Position& b) {
	return std::hypot(a.x - b.x, a.y - b.y);
}

static double positions_distance(const std::vector<Position>& positions) {
	const std::size_t size{positions.size()};
	if (size < 2) {
		return 0;
	}
	double dist{0};
	for (std::size_t i{0}; i < size - 1; ++i) {
		dist += distance(positions[i], positions[i + 1]);
	}
	return dist;
}

static Position find_point(
	const std::vector<Position>& positions,
	double dist
) {
	if (positions.size() < 2) {
		throw std::invalid_argument("there aren't enough of the provided positions");
	}
	if (dist < 0) {
		throw std::invalid_argument("the provided distance can't be negative");
	}
	const double dist_avail{positions_distance(positions)};
	if (dist - dist_avail > 1e-6) {
		throw std::invalid_argument("the provided distance exceeds the available distance");
	}

	double dist_curr{0};
	const int size{static_cast<int>(positions.size())};
	int i{1};
	for (; i < size && dist_curr < dist; ++i) {
		dist_curr += distance(positions[i], positions[i - 1]);
	}
	--i;

	if (i == 0) {
		return positions[0];
	}

	const Position& prev{positions[i - 1]};
	const Position& next{positions[i]};

	const double dist_step{distance(next, prev)};
	const double dist_remainder{dist - (dist_curr - dist_step)};
	const double ratio{dist_remainder / dist_step};

	Position pos_interp{(1 - ratio) * prev.x + ratio * next.x, (1 - ratio) * prev.y + ratio * next.y};
	return pos_interp;
}

static std::vector<Position> positions_represent(
	const std::vector<Position>& positions,
	const int repre_cnt
) {
	const double length{positions_distance(positions)};
	const double step{length / (repre_cnt - 1)};

	std::vector<Position> ret;
	for (int i{0}; i < repre_cnt; ++i) {
		const double dist{i * step};
		const auto& point{find_point(positions, dist)};

		ret.push_back(point);
	}
	return ret;
}

std::vector<Position> Processing::process_positions(
	const std::vector<Position>& positions,
	const int repre_cnt
) {
	const auto& positions_zeroed{positions_zero(positions)};
	const auto& positions_normalized{positions_normalize(positions_zeroed)};
	return positions_represent(positions_normalized, repre_cnt);
}
