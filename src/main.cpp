#include <iostream>
#include <vector>
#include <chrono>

#include "Robot.h"
#include "Graph.h"
#include "Store.h"
#include "Customer.h"
#include "generate_graph.hpp"
#include "generate_orders.hpp"

int main() {
	constexpr int NUM_CUSTOMERS = 10;
	constexpr int STORE_ID = 0;
	constexpr int ROBOT_CAPACITY = 3;

	Store store(STORE_ID);

	std::vector<Customer> customers;
	for (int i = 1; i <= NUM_CUSTOMERS; ++ i) {
		Customer c("Customer_" + std::to_string(i), i);
		customers.push_back(c);
	}

	Graph graph = Graph(NUM_CUSTOMERS + 1);
	generate_graph(graph, STORE_ID, NUM_CUSTOMERS);
	graph.printGraph();

	generate_orders(customers);

	std::cout << "\n--- Customer Orders ---\n";
	for (const auto& c : customers) {
		std::cout << c.get_name() << " ordered "
				  << c.get_order() << " baskets\n";
	}

	double best_distance = std::numeric_limits<double>::infinity();
	std::vector<std::vector<int> > optimal_run;

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

	Robot robot(ROBOT_CAPACITY, customers, store, graph);

	robot.execute_delivery_plan();

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "\nTotal execution time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

	return 0;
}

