#include <iostream>
#include <vector>

#include "Graph.h"
#include "Store.h"
#include "Customer.h"
#include "tsp.hpp"
#include "generate_graph.hpp"
#include "generate_orders.hpp"

int main() {
    constexpr int NUM_CUSTOMERS = 10;
    constexpr int STORE_ID = 0;

    Store store(STORE_ID);

    std::vector<Customer> customers;
    for (int i = 1; i <= NUM_CUSTOMERS; ++i) {
		Customer c("Customer_" + std::to_string(i), i);
		customers.emplace_back(c);
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

    auto runs = group_customers(customers);

    std::cout << "\nTotal delivery runs: " << runs.size() << "\n";

    for (size_t r = 0; r < runs.size(); ++r) {
        std::cout << "\n============================\n";
        std::cout << "Delivery Run " << r + 1 << "\n";

        DistMatrix dist_matrix = build_metric_matrix(graph, STORE_ID, runs[r]);

        std::vector<int> tsp_tour;
        double tour_distance = tsp(dist_matrix, STORE_ID, runs[r], tsp_tour);

        std::cout << "TSP stop order: Store → ";
        for (int id : tsp_tour)
            std::cout << id << " → ";
        std::cout << "Store\n";

        std::cout << "Optimal tour distance: " << tour_distance << " km\n";

        ParentMatrix parent_mat;
        std::vector<int> nodes = runs[r];
        nodes.push_back(STORE_ID);

        for (int node : nodes) {
            auto sp = dijkstra(graph, node);
            parent_mat[node] = sp.parent;
        }

        std::vector<int> full_route;
        int prev = STORE_ID;

        for (int stop : tsp_tour) {
            auto segment = reconstruct_path(prev, stop, parent_mat);
            if (!full_route.empty())
                segment.erase(segment.begin());
            full_route.insert(full_route.end(), segment.begin(), segment.end());
            prev = stop;
        }

        auto back = reconstruct_path(prev, STORE_ID, parent_mat);
        back.erase(back.begin());
        full_route.insert(full_route.end(), back.begin(), back.end());

        std::cout << "Full navigation path: ";
        for (int node : full_route)
            std::cout << node << " ";
        std::cout << "\n";
    }

    return 0;
}

