#pragma once

#include "Node.h"

class Store : public Node {
public:
	Store(int id): Node("Store", id) {}
};
