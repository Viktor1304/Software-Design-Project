#ifndef __generate_graph__ 
#define __generate_graph__

#include "Graph.h"
#include <algorithm>
#include <random>
#include <set>

inline void generate_graph(Graph& graph, int store_id, int num_customers) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis_w(50.0, 1000.0);
	std::uniform_int_distribution<> dis_p(1, num_customers - 1);
	std::uniform_int_distribution<> dis_connected(1, num_customers);

	int direct_connections = dis_p(gen);

	std::vector<int> customers(num_customers);
	for (int i = 0; i < num_customers; ++ i) {
		customers[i] = i + 1;
	}

	std::shuffle(customers.begin(), customers.end(), gen);

	std::set<int> connected_to_store;

	for (int i = 0; i < direct_connections; ++ i) {
		int customer_id = customers[i];
		double weight = dis_w(gen);
		graph.addEdge(store_id, customer_id, weight);
		connected_to_store.insert(customer_id);
	}

	while (!graph.connected()) {
		int u = dis_connected(gen);
		int v = dis_connected(gen);

		if (u != v) {
			auto neighbors_u = graph.get_neighbors(u);
			bool already_connected = false;

			for (const auto& neighbor : neighbors_u) {
				if (neighbor.first == v) {
					already_connected = true;
					break;
				}
			}

			if (!already_connected) {
				double weight = dis_w(gen);
				graph.addEdge(u, v, weight);
			}
		}
	}
}

#endif
