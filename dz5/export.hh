#pragma once

#include "gesture.hh"
#include "position.hh"

#include <ostream>
#include <string>
#include <vector>

namespace Export {
	void gestures(std::ostream& out, const std::vector<Gesture>& gestures);
	void gesture(std::ostream& out, const Gesture& g);
	void position(std::ostream& out, const Position& p);
	void save_gestures(const std::string& filename, const std::vector<Gesture> gestures);
}
