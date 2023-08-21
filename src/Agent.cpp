#include "Agent.h"
#include "Session.h"
#include "Graph.h"
using namespace std;
//region Virus
Virus::Virus(int nodeInd) : Agent(), nodeInd(nodeInd) { //constructor
}

Virus *Virus::clone() const {
    return new Virus(*this);
}
bool isNodeListedAsAgent(int node, vector<Agent*>& vector){
    for (Agent* agent : vector)
        if (agent->getNode()==node)
            return true;
    return false;
}
void Virus::act(Session &session) {
    Graph& g = session.getGraph();
    if(!g.isInfected(this->getNode())) {
        g.infectNode(this->getNode());
        session.enqueueInfected(this->getNode()); // push to the end of the infected Q
    }

    g.infectNode(this->getNode());
    std::vector<int> neighbors = g.getNeighbors(this->getNode());//return the neighbors of the current virus
    for (int neighbor : neighbors) {
        if (!g.isInfected(neighbor) && !isNodeListedAsAgent(neighbor, session.getAgents()) &&
        !isNodeListedAsAgent(neighbor,session.getCarriersAgents()) ) { //check if the neighbor is not already infected
            session.add_carried_virus(new Virus(neighbor));
            break;
        }
    }
}


int Virus::getNode() const {
    return nodeInd;
}


//endregion

//region ContactTracer
ContactTracer::ContactTracer() : Agent() { //constructor

}

ContactTracer *ContactTracer::clone() const {
    return new ContactTracer(*this);
}

void ContactTracer::act(Session &session) {
    std::queue<int>& infectedQueue = session.getInfectedQueue();

    if (!infectedQueue.empty()) {
        int infected = infectedQueue.front(); // take out tha first element in the Q
        infectedQueue.pop(); // remove the first element
        Graph& g = session.getGraph();
        Tree *tree = session.bfs(infected);
        int target = tree->traceTree();
        g.disconnectNode(target);
        delete tree;
    }
}

int ContactTracer::getNode() const {
    return -1;
}



//endregion


Agent::Agent() {

}
