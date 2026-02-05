#ifndef __execute_plan__
#define __execute_plan__


#include "Graph.h"
#include "tsp.hpp"


void execute_plan(const Graph& graph, const DistMatrix& dist_matrix, const std::vector<std::vector<int> >& runs) {
	for (size_t i = 0; i < runs.size(); ++ i) {
		std::vector<int> route;
		double distance = tsp(dist_matrix, 0, runs[i], route);

		std::cout << "Run " << i + 1 << ": ";
		std::cout << "Route: Store -> ";

		for (auto c : route) {
			std::cout << "Customer " << c << " -> ";
		}

		std::cout << "Store | Total Distance: " << distance << '\n';
	}

}

void print_full_route(int store_id,
                    const std::vector<int>& tour,
                    const ParentMatrix& parents)
{
    std::vector<int> full_route;

    int prev = store_id;

    for (int stop : tour) {
        auto segment = reconstruct_path(prev, stop, parents);

        // Avoid duplicating nodes
        if (!full_route.empty())
            segment.erase(segment.begin());

        full_route.insert(full_route.end(), segment.begin(), segment.end());
        prev = stop;
    }

    // Return to store
    auto back = reconstruct_path(prev, store_id, parents);
    back.erase(back.begin());
    full_route.insert(full_route.end(), back.begin(), back.end());

    std::cout << "Full route (all nodes visited): ";
    for (int n : full_route)
        std::cout << n << " ";
    std::cout << "\n";
}


#endif
