#pragma once
#include "Node.h"
#include <cassert>

class Customer : public Node {
private:
	int baskets_ordered = 0;
public:
	Customer(std::string name, int id) : Node(name, id) {
		assert(name != "Store" && "Customer name cannot be 'Store'");
		assert(id >= 0 && "Customer ID must be non-negative");
	}

	void set_order(int baskets) {
		assert(baskets >= 0 && "Number of baskets ordered must be non-negative");
		assert(baskets < 3 && "Customer cannot order more than 2 baskets");

		baskets_ordered = baskets;
	}

	std::string get_name() const {
		return Node::get_name();
	}

	int get_order() const {
		return baskets_ordered;
	}
};
