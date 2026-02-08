#ifndef __generate_orders__
#define __generate_orders__

#include "Customer.h"
#include <functional>
#include <vector>

const int MAX_BASKETS = 3;

inline void generate_orders(std::vector<Customer>& customers) {
	for (auto& customer : customers) {
		int order = rand() % MAX_BASKETS;
		customer.set_order(order);
	}
}

using Run = std::vector<int>;
using Partition = std::vector<Run>;

inline void generate_partitions(const std::vector<std::pair<int,int> >& customers, int capacity, 
						 std::vector<bool>& used, Partition& current, std::vector<Partition>& all_solutions) {
    int n = customers.size();

    int first = -1;
    for (int i = 0; i < n; ++ i) {
        if (!used[i]) {
            first = i;
            break;
        }
    }

    if (first == -1) {
        all_solutions.push_back(current);
        return;
    }

    std::vector<int> run;
    int load = 0;

    std::function<void(int)> build_run = [&](int idx)
    {
        if (load > capacity) return;

        if (!run.empty()) {
            current.push_back(run);
            generate_partitions(customers, capacity, used, current, all_solutions);
            current.pop_back();
        }

        for (int i = idx; i < n; ++ i) {
            if (used[i]) continue;

            int baskets = customers[i].second;
            if (load + baskets > capacity) continue;

            used[i] = true;
            run.push_back(customers[i].first);
            load += baskets;

            build_run(i + 1);

            load -= baskets;
            run.pop_back();
            used[i] = false;
        }
    };

    used[first] = true;
    run.push_back(customers[first].first);
    load = customers[first].second;

    build_run(first + 1);

    used[first] = false;
}

inline std::vector<Partition>
generate_unique_delivery_plans(const std::vector<Customer>& customers, int capacity) {
	std::vector<std::pair<int,int> > active;
	for (const auto& c : customers) {
		if (c.get_order() > 0) {
			active.push_back({c.get_id(), c.get_order()});
		}
	}

	std::vector<bool> used(active.size(), false);
	Partition current;
	std::vector<Partition> solutions;

	generate_partitions(active, capacity, used, current, solutions);
	return solutions;
}


#endif
