#include <fstream>
#include <string>
#include <iostream>
#include "Session.h"
#include "Agent.h"
#include "Graph.h"
#include "Tree.h"
#include "json.hpp"


using json = nlohmann::json;
using namespace std;



Session::Session(const std::string &path) : g(vector<vector<int>>()), treeType(), agents(), infectedQueue(),curr_cycle(0),
                                            carriers()   { //Extracting information from json
    std::ifstream jsonRead(path);
    json j = json::parse(jsonRead);
    g = Graph(j["graph"]);
    map<string, TreeType> treeTypeMap = {{"M", MaxRank},
                                         {"C", Cycle},
                                         {"R", Root}};
    string alias = j["tree"];
    treeType = treeTypeMap[alias];
    vector<pair<string, int>> agents_pairs = j["agents"];
    for (const auto &p : agents_pairs) {
        if (p.first == "C")
            carriers.push_back(new ContactTracer());
        else {
            carriers.push_back(new Virus(p.second));
        }
    }

}

void Session::clean() { // call from the destructor
    for (auto* agent : agents)
        delete (agent); //remove data from the heap
    agents.erase(agents.begin(), agents.end()); // clear pointers
    for (auto* currier:carriers)
        delete (currier); //remove from the heap
    carriers.erase(carriers.begin(), carriers.end()); //clear pointers

}




Session::Session(const Session &other) : g(other.g), treeType(other.treeType), agents(),
                                         infectedQueue(other.infectedQueue), curr_cycle(other.curr_cycle),
                                         carriers() { //copy constructor
    for (auto agent : other.agents)  // deep copy for agents
        this->agents.push_back(agent->clone()); //because we dont know if this agent is a virus/contact tracer we need to call clone
    for (auto currier : other.carriers)  // deep copy for carriers
        this->carriers.push_back(currier->clone());
}


Session::Session(Session &&other) : g(move(other.g)), treeType(other.treeType),
                                    agents(move(other.agents)),
                                    infectedQueue(move(other.infectedQueue)), curr_cycle(other.curr_cycle),
                                    carriers(move(other.carriers)) {} //Move constructor



Session &Session::operator=(const Session &other) { // copy assigment operator
    if (this == &other) // check assignment to self
        return *this;

    clean(); //clean my data from the heap

    g = other.g;
    for (auto agent:other.agents) //deep copy for agents
        this->agents.push_back(agent->clone());
    for (auto currier:other.carriers) //deep copy for agents
        this->carriers.push_back(currier->clone());
    treeType = other.treeType;
    infectedQueue = other.infectedQueue;
    curr_cycle = other.curr_cycle;
    return *this;
}

Session &Session::operator=(Session &&other) { //Move assignment operator
    if (this == &other)
        return *this;

    g = move(other.g);
    treeType = other.treeType;
    agents.swap(other.agents); // moves the ownership of its pointers from other to this
    infectedQueue.swap(other.infectedQueue);
    curr_cycle = other.curr_cycle;
    carriers.swap(other.carriers);
    return *this;
}
void Session::simulate() {
    while (!isFinished()) {
        insertNewAgents(); //new agents that added in the last iteration/in the initialization

        for (auto &elem:agents) { //by reference no need to copy
            elem->act(*this); //access to data in the heap
        }


        curr_cycle++;
    }

    nlohmann::json json_file; //json output


    json_file["infected"] = this->g.getInfected();
    json_file["graph"] = g.getMatrix();


    std::ofstream o("./output.json");
    o << json_file << endl;
}



void Session::addAgent(const Agent &agent) {
    this->agents.push_back(agent.clone());
}

void Session::setGraph(const Graph &graph) {
    g = graph;
}

TreeType Session::getTreeType() const {
    return treeType;
}

enum Colors {
    Unknown = 0,
    Visited = 1,
};

Tree* Session::bfs(int root_index) {
    Graph graph(g); //call copy constructor of graph
    vector<int> colored_v;
    size_t size = g.size();// number of nodes
    for (size_t i = 0; i < size; ++i) //initialized nodes to unknown
        colored_v.push_back(Unknown);
    queue<Tree*> q;

    Tree* tree1 = Tree::createTree(*this, root_index); //normal BFS:
    q.push(tree1);
    colored_v[root_index] = Visited;
    Tree* tree = q.front(); //read without remove the first data
    while (!q.empty()) {
        Tree* v = q.front();
        q.pop(); //remove the first data from the Q
        int tmp_node = v->getNode(); //check the current node
        vector<int> neighbors = g.getNeighbors(tmp_node); //take his the neighbors
        for (int u :neighbors) {
            if (colored_v[u] != Visited) {
                colored_v[u] = Visited;
                Tree* t = Tree::createTree(*this, u); //creat tree with the current neighbor
                v->addChild(*t); //creat the children of the temp node
                delete (t); // addChild is clone so delete this memory
                q.push(v->getChild(u));
            }
        }
    }

    return tree;
}

void Session::enqueueInfected(int j) {
    infectedQueue.push(j);
};

int Session::dequeueInfected() {
    if (!infectedQueue.empty()) { //if queue !isEmpty extract the first element
        int first = infectedQueue.front();
        infectedQueue.pop();

        return first;
    }
    return -1; //must return int, -1 is not a valid number of node
}

Session::~Session() { //destuctor
    clean();
}

Graph &Session::getGraph() {
    return g;
}

void Session::insertNewAgents() { //insert new (yellow) virus to agent in the end of the iteration
    for (auto agent:carriers) {
        agents.push_back(agent);
    }
    carriers.clear(); //removing all the agents pointers from carries without destroying them


}

void Session::add_carried_virus(Agent* virus) {
    carriers.push_back(virus);
}

queue<int> &Session::getInfectedQueue() {
    return infectedQueue;
}

bool Session::isFinished() { //if in the end of the iteration we didn't add new virus so all the conditions to end are exist
    return carriers.empty();
}

int Session::getCurrCycle() const {
    return curr_cycle;
}

std::vector<Agent*> &Session::getAgents() {
    return agents;
}

std::vector<Agent*> &Session::getCarriersAgents() {
    return carriers;
}





