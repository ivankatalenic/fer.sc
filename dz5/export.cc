#include "export.hh"

#include "gesture.hh"
#include "position.hh"

#include <vector>
#include <ostream>
#include <fstream>

void Export::gestures(std::ostream& out, const std::vector<Gesture>& gestures) {
	for (const Gesture& g : gestures) {
		gesture(out, g);
		out << '\n';
	}
}

void Export::gesture(std::ostream& out, const Gesture& g) {
	out << g.label << ':';
	for (const Position& p : g.positions) {
		position(out, p);
		out << ";";
	}
}

void Export::position(std::ostream& out, const Position& p) {
	out << p.x << "," << p.y;
}

void Export::save_gestures(const std::string& filename, const std::vector<Gesture> g) {
	std::ofstream file_stream(filename);
	gestures(file_stream, g);
}
