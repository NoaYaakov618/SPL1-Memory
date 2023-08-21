#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <string>
#include "Graph.h"
#include <queue>
#include "Agent.h"

class Agent;

enum TreeType{
  Cycle,
  MaxRank,
  Root
};

class Session{
public:

    Session(const std::string& path);//constructor

    Session (const Session& other); //Copy constructor

    Session (Session&& other); //Move constructor

    Session& operator =(const Session& other); //Copy assignment operator

    Session& operator =(Session&& other);//Move assignment operator

    virtual ~Session(); // destructor



    void simulate();
    void addAgent(const Agent& agent);
    void setGraph(const Graph& graph);
    Graph& getGraph();
    void add_carried_virus(Agent* virus);
    void enqueueInfected(int);
    int dequeueInfected();
    std::queue<int>& getInfectedQueue();
    Tree* bfs(int root_index);
    TreeType getTreeType() const;
    bool isFinished();
    void insertNewAgents();
    void clean();
    int getCurrCycle() const;
    std::vector<Agent*>& getAgents();
    std::vector<Agent*>& getCarriersAgents();




private:
    Graph g;
    TreeType treeType;
    std::vector<Agent*> agents;
    std::queue<int> infectedQueue;
    int curr_cycle;
    std::vector<Agent*> carriers;
};

#endif
