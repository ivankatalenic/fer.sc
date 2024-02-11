#pragma once

#include "gesture.hh"
#include "position.hh"
#include "config.hh"

#include <vector>
#include <istream>

namespace Import {
	std::vector<Gesture> gestures(std::istream& in);
	Gesture              gesture(std::istream& in);
	std::vector<Gesture> gestures_from_file(const std::string& filename);	
	
	std::vector<Position> positions(std::istream& in);
	Position              position(std::istream& in);

	Config           config(std::istream& in);
	Config           config_from_file(const std::string& filename);
	std::vector<int> config_arch_from_string(const std::string& str);
}
