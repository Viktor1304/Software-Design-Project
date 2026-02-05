#pragma once
#include <string>

class Node {
private:
	std::string name;
	int id;

public:
	Node(std::string name, int id) : name(name), id(id) {}

	std::string getName() const {
		return name;
	}

	int getId() const {
		return id;
	}
};
