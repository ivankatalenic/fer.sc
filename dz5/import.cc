#include "import.hh"

#include <istream>
#include <sstream>
#include <fstream>
#include <exception>
#include <limits>

std::vector<Gesture> Import::gestures(std::istream& in) {
	std::vector<Gesture> ret;

	while (true) {
		Gesture g;
		try {
			g = Import::gesture(in);
		} catch (const std::exception& e) {
			break;
		}
		ret.push_back(g);
	}

	return ret;
}

Gesture Import::gesture(std::istream& in) {
	Gesture g;
	
	std::stringbuf buf_label;
	in.get(buf_label, ':');
	if (!in) {
		throw std::runtime_error("can't read a gesture's label from the input stream");
	}
	g.label = buf_label.str();
	in.ignore(1);
	
	std::stringbuf buf_positions;
	in.get(buf_positions, '\n');
	if (!in) {
		throw std::runtime_error("can't read a gesture's positions from the input stream");
	}
	in.ignore(1);
	std::istringstream positions_stream(buf_positions.str());
	g.positions = Import::positions(positions_stream);

	return g;
}

std::vector<Gesture> Import::gestures_from_file(const std::string& filename) {
	std::ifstream file_stream(filename);
	if (!file_stream) {
		throw std::runtime_error("can't open a file with gestures");
	}
	return gestures(file_stream);
}

std::vector<Position> Import::positions(std::istream& in) {
	std::vector<Position> ret;

	while (true) {
		Position p;

		try {
			p = Import::position(in);
		} catch (const std::exception& e) {
			break;
		}
		
		ret.push_back(p);
		in.ignore(1, ';');
	}

	return ret;
}

Position Import::position(std::istream& in) {
	Position p;

	in >> p.x;
	in.ignore(1, ',');
	in >> p.y;
	if (!in) {
		throw std::runtime_error("can't import a position");
	}

	return p;
}

std::vector<int> config_arch(std::istream& in) {
	std::vector<int> arch;
	while (true) {
		int count{-1};
		in >> count;
		if (!in) {
			break;
		}
		arch.push_back(count);
		
		in.ignore(1);
		if (!in) {
			break;
		}
	}
	return arch;
}

std::vector<int> Import::config_arch_from_string(const std::string& str) {
	std::istringstream ss(str);
	return config_arch(ss);
}

Config Import::config(std::istream& in) {
	Config c;

	in.ignore(std::numeric_limits<std::streamsize>::max(), ':');

	int type{-1};
	in >> type;
	if (!in) {
		throw std::runtime_error("can't read the network's training type");
	}

	c.type = static_cast<Training>(type);
	if (type < 1 || type > 3) {
		throw std::runtime_error("invalid training type");
	}

	in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	in.ignore(std::numeric_limits<std::streamsize>::max(), ':');

	std::stringbuf buf_arch;
	in.get(buf_arch, '\n');
	if (!in) {
		throw std::runtime_error("can't read the network's architecture");
	}

	std::istringstream arch_stream(buf_arch.str());
	c.architecture = config_arch(arch_stream);

	return c;
}

Config Import::config_from_file(const std::string& filename) {
	std::ifstream file_stream(filename);
	if (!file_stream) {
		throw std::runtime_error("can't open a config file");
	}
	return config(file_stream);
}
