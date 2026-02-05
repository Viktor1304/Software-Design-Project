#ifndef __generate_orders__
#define __generate_orders__

#include "Customer.h"
#include <vector>

const int MAX_BASKETS = 3;

void generate_orders(std::vector<Customer>& customers) {
	for (auto& customer : customers) {
		int order = rand() % MAX_BASKETS;
		customer.set_order(order);
	}
}

std::vector<std::vector<int> > group_customers(const std::vector<Customer>& customers) {
	std::vector<std::vector<int> > runs;
	std::vector<int> current_run;
	int baskets = 0;

	for (const auto& customer : customers) {
		if (customer.get_order() == 0) {
			continue;
		}

		if (baskets + customer.get_order() > MAX_BASKETS) {
			runs.push_back(current_run);
			current_run.clear();
			baskets = 0;
		}

		current_run.push_back(customer.get_id());
		baskets += customer.get_order();
	}

	if (!current_run.empty()) {
		runs.push_back(current_run);
	}

	return runs;
}

#endif
