#pragma once
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

class Graph {
private:
	int num_nodes;
	std::unordered_map<int, std::vector<std::pair<int, double> > > adjList;

public:
	Graph(int num_nodes) : num_nodes(num_nodes) {}

	void addEdge(int u, int v, double weight) {
		adjList[u].push_back(std::make_pair(v, weight));
		adjList[v].push_back(std::make_pair(u, weight));
	}

	const std::vector<std::pair<int, double> >& getNeighbors(int u) const {
		if (adjList.find(u) == adjList.end()) {
			static const std::vector<std::pair<int, double> > empty;
			return empty;
		}
		return adjList.at(u);
	}

	const std::unordered_map<int, std::vector<std::pair<int, double> > >& getAdjList() const {
		return adjList;
	}
	
	void dfs(int node, bool* visited) const {
		visited[node] = true;
		for (const auto& neighbor : adjList.at(node)) {
			int next_node = neighbor.first;

			if (!visited[next_node]) {
				dfs(next_node, visited);
			}
		}
	}

	const bool connected() const {
		int start = adjList.begin()->first;
		bool visited[this->num_nodes] = {false};
		dfs(start, visited);

		for (int i = 0; i < this->num_nodes; ++ i) {
			if (!visited[i]) {
				return false;
			}
		}
		return true;
	}

	void printGraph() const {
		for (const auto& pair : adjList) {
			int u = pair.first;
			const auto& neighbors = pair.second;
			std::cout << "Node " << u << ": ";
			for (const auto& neighbor : neighbors) {
				std::cout << "(Node " << neighbor.first << ", Weight " << neighbor.second << ") ";
			}
			std::cout << std::endl;
		}
	}
};
