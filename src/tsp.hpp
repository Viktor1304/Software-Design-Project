#ifndef __tsp__ 
#define __tsp__

#include "Graph.h"
#include <algorithm>
#include <limits>
#include <queue>
#include <unordered_map>
#include <vector>

using ParentMatrix = std::unordered_map<int, std::unordered_map<int, int>>;

struct SPResult {
    std::unordered_map<int, double> dist;
    std::unordered_map<int, int> parent;
};

SPResult dijkstra(const Graph& g, int source) {
	SPResult res;
    const double INF = std::numeric_limits<double>::infinity();

    for (const auto& [node, _] : g.get_adj_list())
        res.dist[node] = INF;

    res.dist[source] = 0;

    using P = std::pair<double, int>;
    std::priority_queue<P, std::vector<P>, std::greater<>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > res.dist[u]) continue;

        for (const auto& e : g.get_neighbors(u)) {
            double nd = d + e.second;
            if (nd < res.dist[e.first]) {
                res.dist[e.first] = nd;
                res.parent[e.first] = u;
                pq.push({nd, e.first});
            }
        }
    }
    return res;
}

std::unordered_map<int, double>
dijkstra_all(const Graph& g, int source)
{
    std::unordered_map<int, double> dist;
    const double INF = std::numeric_limits<double>::infinity();

    for (const auto& [node, _] : g.get_adj_list())
        dist[node] = INF;

    dist[source] = 0;

    using P = std::pair<double, int>;
    std::priority_queue<P, std::vector<P>, std::greater<>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;

        for (const auto& e : g.get_neighbors(u)) {
            double nd = d + e.second;
            if (nd < dist[e.first]) {
                dist[e.first] = nd;
                pq.push({nd, e.first});
            }
        }
    }
    return dist;
}

using DistMatrix = std::unordered_map<int, std::unordered_map<int, double> >;
DistMatrix build_metric_matrix(const Graph& g,
                             int storeId,
                             const std::vector<int>& customers)
{
    DistMatrix matrix;

    std::vector<int> nodes = customers;
    nodes.push_back(storeId);

    for (int node : nodes) {
        auto sp = dijkstra_all(g, node);
        for (int other : nodes) {
            matrix[node][other] = sp[other];  // shortest path
        }
    }
    return matrix;
}

double tsp(const DistMatrix& dist, int store_id, const std::vector<int>& run_customers, std::vector<int>& route) {
    int n = run_customers.size();
    if (n == 0) return 0.0;

    std::vector<int> id = run_customers;

    int FULL = (1 << n);
    const double INF = std::numeric_limits<double>::infinity();

    std::vector<std::vector<double>> dp(FULL, std::vector<double>(n, INF));
    std::vector<std::vector<int>> parent(FULL, std::vector<int>(n, -1));

    for (int i = 0; i < n; ++i) {
        if (dist.count(store_id) && dist.at(store_id).count(id[i]))
            dp[1 << i][i] = dist.at(store_id).at(id[i]);
    }

    for (int mask = 0; mask < FULL; ++ mask) {
        for (int u = 0; u < n; ++ u) {
            if (!(mask & (1 << u))) continue;
            if (dp[mask][u] == INF) continue;

            for (int v = 0; v < n; ++ v) {
                if (mask & (1 << v)) continue;

                if (!dist.count(id[u]) || !dist.at(id[u]).count(id[v]))
                    continue;

                int next_mask = mask | (1 << v);
                double new_dist = dp[mask][u] + dist.at(id[u]).at(id[v]);

                if (new_dist < dp[next_mask][v]) {
                    dp[next_mask][v] = new_dist;
                    parent[next_mask][v] = u;
                }
            }
        }
    }

    double best = INF;
    int last = -1;

    for (int i = 0; i < n; ++i) {
        if (!dist.count(id[i]) || !dist.at(id[i]).count(store_id))
            continue;

        double total = dp[FULL - 1][i] + dist.at(id[i]).at(store_id);
        if (total < best) {
            best = total;
            last = i;
        }
    }

    int mask = FULL - 1;
    std::vector<int> reverse_tour;

    while (last != -1) {
        reverse_tour.push_back(id[last]);
        int p = parent[mask][last];
        mask ^= (1 << last);
        last = p;
    }

    std::reverse(reverse_tour.begin(), reverse_tour.end());
    route = reverse_tour;

    return best;
}

std::vector<int> reconstruct_path(int start, int end,
                                 const ParentMatrix& parents)
{
    std::vector<int> path;
    int curr = end;
    path.push_back(curr);

    while (curr != start) {
        curr = parents.at(start).at(curr);
        path.push_back(curr);
    }

    std::reverse(path.begin(), path.end());
    return path;
}

#endif
