#pragma once

#include <string>

class Node {
private:
	std::string name;
	int id;

public:
	Node(std::string name, int id) : name(name), id(id) {}

	std::string get_name() const {
		return name;
	}

	int get_id() const {
		return id;
	}
};
