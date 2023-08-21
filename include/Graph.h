#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include "Tree.h"

class Graph{
public:
    explicit Graph(std::vector<std::vector<int>> matrix);

    void infectNode(int nodeInd);
    bool isInfected(int nodeInd);
    void disconnectNode(int node);

    std::vector<int> getNeighbors(int v);
    std::vector<int> getInfected() const;
    std::vector<std::vector<int>>& getMatrix();
    size_t size();
private:
    std::vector<std::vector<int>> edges;
    std::vector<bool> infected_flags;

};

#endif
