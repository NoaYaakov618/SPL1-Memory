//
// Created by spl211 on 09/11/2020.
//

#include <Graph.h>
#include <cstdio>

using namespace std;

void Graph::infectNode(int nodeInd) {
    infected_flags[nodeInd] = true;
}

bool Graph::isInfected(int nodeInd) {
    return infected_flags[nodeInd];
}

Graph::Graph(std::vector<std::vector<int>> matrix) : edges(move(matrix)), infected_flags() { //constructor
    for (size_t i = 0; i < edges.size(); ++i) {
        infected_flags.push_back(false);
    }
}

size_t Graph::size() {
    return infected_flags.size(); // number of nodes
}

std::vector<int> Graph::getNeighbors(int v) {
    vector<int> neighbors;
    size_t size = this->size();
    for (size_t u = 0; u < size; u++) {
        int e = edges[v][u];
        if (e)
            neighbors.push_back(u);
    }
    return neighbors;
}

void Graph::disconnectNode(int node) {
    for (size_t i = 0; i < edges.size(); i++) {
        edges[node][i] = 0;
        edges[i][node] = 0;
    }
}

std::vector<int> Graph::getInfected() const { //return all the infected nodes
    vector<int> infected;
    int j = 0;
    for (bool infected_flag : infected_flags) { // if the node is infected push his index to the infected vector
        if (infected_flag)
            infected.push_back(j);
        j++;
    }
    return infected;
}

vector<vector<int>>& Graph::getMatrix() {
    return edges;
}



