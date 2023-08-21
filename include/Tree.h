#ifndef TREE_H_
#define TREE_H_

#include <vector>

class Session;

class Tree{
public:
    Tree(int rootLabel); //constructor
    Tree(const Tree& other); //copy constructor
    Tree(Tree && other); //move constructor
    virtual ~Tree(); //destructor
    Tree& operator = (const Tree& other); //copy assigment operator
    Tree& operator =(Tree&& other);  //Move assignment operator

    virtual int traceTree()=0;
    virtual Tree *clone() const = 0;
    void clean();
    void addChild(const Tree& child);
    static Tree* createTree(const Session& session, int rootLabel);
    int getNode() const;
    std::vector <Tree*> getChildren();
    Tree* getChild(int u);

private:
    int node;
    std::vector<Tree*> children;

};


class CycleTree: public Tree{
public:
    CycleTree(int rootLabel, int currCycle);
    virtual int traceTree();
    virtual CycleTree *clone() const;
private:
    int currCycle;
};

class MaxRankTree: public Tree{
public:
    MaxRankTree(int rootLabel);
    virtual int traceTree();
    virtual MaxRankTree *clone() const;
};

class RootTree: public Tree{
public:
    RootTree(int rootLabel);
    virtual int traceTree();
    virtual RootTree *clone() const;
};

#endif
