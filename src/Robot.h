#pragma once

#include "Customer.h"
#include <iostream>
#include <limits>
#include <vector>
#include "generate_orders.hpp"
#include "Graph.h"
#include "tsp.hpp"
#include "Store.h"

using Run = std::vector<int>;

class Robot {
private:
	int max_capacity;
	std::vector<Customer> customers;
	Store store;
	Graph graph;

public:
	Robot(int capacity, std::vector<Customer> customers, Store store, Graph graph) : 
		max_capacity(capacity), customers(customers), store(store), graph(graph) {
		assert(capacity > 0 && "Robot capacity must be greater than zero");
	}

	std::vector<Customer>& get_customers() {
		return this->customers;
	}

	struct OptimalRun {
		std::vector<Run> runs;
		double total_distance;
	};
	OptimalRun generate_optimal_run() {
		std::vector<Run> optimal_run;

		std::vector<std::vector<Run> > unique_plans;
		unique_plans = generate_unique_delivery_plans(this->customers, this->max_capacity);

		std::cout << "\nGenerated " << unique_plans.size() << " unique delivery plans.\n"; 

		double best_distance = std::numeric_limits<double>::max();
		for (auto runs : unique_plans) {
			double run_distance = 0.0;

			for (size_t i = 0; i < runs.size(); ++ i) {
				DistMatrix dist_matrix = build_metric_matrix(this->graph, this->store.get_id(), runs[i]);

				std::vector<int> tsp_tour;
				double tour_distance = tsp(dist_matrix, this->store.get_id(), runs[i], tsp_tour);
				run_distance += tour_distance;
			}

			if (run_distance < best_distance) {
				best_distance = run_distance;
				optimal_run = runs;
			}
		}

		return OptimalRun{optimal_run, best_distance};
	}

	void execute_delivery_plan() {
		OptimalRun optimal = this->generate_optimal_run();
		std::vector<Run> optimal_run = optimal.runs;
		double best_distance = optimal.total_distance;

		std::cout << "\nTotal delivery runs: " << optimal_run.size() << "\n";

		for (size_t r = 0; r < optimal_run.size(); ++ r) {
			std::cout << "\n=========================\n";
			std::cout << "Delivery Run " << r + 1 << ":\n";

			DistMatrix dist_matrix = build_metric_matrix(this->graph, this->store.get_id(), optimal_run[r]);

			std::vector<int> tsp_tour;
			double tour_distance = tsp(dist_matrix, this->store.get_id(), optimal_run[r], tsp_tour);

			std::cout << "TSP stop order: Store - ";
			for (size_t id : tsp_tour) {
				std::cout << id << " - ";
			}
			std::cout << "Store\n";

			std::cout << "Optimal tour distance: " << tour_distance << "\n";

			ParentMatrix parent_matrix;
			Run nodes = optimal_run[r];
			nodes.push_back(this->store.get_id());

			for (size_t node : nodes) {
				auto sp = dijkstra(this->graph, node);
				parent_matrix[node] = sp.parent;
			}

			Run full_route;
			int prev = this->store.get_id();

			for (size_t stop : tsp_tour) {
				auto segment = reconstruct_path(prev, stop, parent_matrix);

				if (!full_route.empty()) {
					segment.erase(segment.begin());
				}
				full_route.insert(full_route.end(), segment.begin(), segment.end());
				prev = stop;
			}

			auto back = reconstruct_path(prev, this->store.get_id(), parent_matrix);
			back.erase(back.begin());
			full_route.insert(full_route.end(), back.begin(), back.end());

			std::cout << "Full navigation route: ";
			for (size_t node : full_route) {
				std::cout << node << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\nOptimal total distance: " << best_distance << "\n";
	}
};
