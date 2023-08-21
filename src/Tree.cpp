//
// Created by spl211 on 09/11/2020.
//

#include "Session.h"
#include "Tree.h"

using namespace std;

Tree *Tree::createTree(const Session &session, int rootLabel) {
    TreeType type = session.getTreeType();
    Tree *tree;
    if (type == Cycle) {
        tree = new CycleTree(rootLabel, session.getCurrCycle());
    }
    if (type == Root) {
        tree = new RootTree(rootLabel);
    }
    if (type == MaxRank) {
        tree = new MaxRankTree(rootLabel);
    }
    return tree;


}

int Tree::getNode() const {
    return node;
}

void Tree::addChild(const Tree &child) {
    Tree *copy = child.clone();
    children.push_back(copy);
}

std::vector<Tree *> Tree::getChildren() {
    return children;
}

Tree::Tree(int rootLabel) : node(rootLabel), children(){ //constructor

}

Tree::Tree(const Tree &other): node(other.node), children(){//copy constructor
    for (Tree* tree:other.children)  // deep copy for the children
        this->addChild(*tree);
}

Tree::Tree(Tree &&other) : node(other.node), children(move(other.children)) {//move constructor

}

Tree::~Tree() { //destructor
    clean();
}

void Tree::clean() {
    for(auto &tree : children){ //deep clean for children
        delete(tree);
    }
    children.clear(); // delete pointers

}

Tree &Tree::operator=(const Tree &other) { //copy assigment operator
    if(this == &other)
        return *this;
    clean();
    for (auto tree:other.children)
        this->addChild(*tree); //copy all the children
    node = other.node;
    return *this;
}



Tree &Tree::operator=(Tree &&other) { //move assigment operator
    if(this == &other)
        return *this;

    children.swap(other.children);
    node = other.node;
    return *this;

}

Tree* Tree::getChild(int u) {
    for (Tree *child: children) {
        if (child->node == u)
            return child;
    }
    return nullptr;
}



RootTree::RootTree(int rootLabel) : Tree(rootLabel) { //constructor
}

int RootTree::traceTree() {
    return getNode();

}

RootTree *RootTree::clone() const {
    return new RootTree(*this);
}


CycleTree::CycleTree(int rootLabel, int currCycle) : Tree(rootLabel), currCycle(currCycle) { //constructor
}

CycleTree *CycleTree::clone() const {
    return new CycleTree(*this);
}
int CycleTree::traceTree() {
    int c = currCycle;
    Tree *tree = this;
    while (c > 0 && !tree->getChildren().empty()) {
        tree = tree->getChildren()[0]; // take the left-most child
        c--;
    }
    return tree->getNode(); // return the node of the tree in this cycle
}


MaxRankTree::MaxRankTree(int rootLabel) : Tree(rootLabel) { //constructor

}

MaxRankTree *MaxRankTree::clone() const {
    return new MaxRankTree(*this);
}


int MaxRankTree::traceTree() { //return the node with the max-rank
    queue<Tree *> q;
    q.push(this); //copy the pointers of this tree to the Q
    Tree *maxRank = this; // the first node (*tree)
    while (!q.empty()) { //while we have more children to check
        Tree *curr = q.front();
        q.pop(); //remove the current pointer
        for (Tree *child : curr->getChildren()) // for every child of current
            q.push(child); //push his pointer
        if (curr->getChildren().size() > maxRank->getChildren().size()) // found new maxRank
            maxRank = curr;
    }
    return maxRank->getNode();
}



