#ifndef AGENT_H_
#define AGENT_H_

#include <vector>
#include "Session.h"

class Agent {
public:
    Agent() ;

    virtual void act(Session &session) = 0;

    virtual Agent *clone() const = 0;

    virtual ~Agent() = default;

    virtual int getNode() const =0;
};

class ContactTracer : public Agent {
public:
    ContactTracer();

    virtual void act(Session &session);

    virtual ContactTracer *clone() const;

    int getNode() const;
};


class Virus : public Agent {
public:
    Virus(int nodeInd);

    virtual void act(Session &session);

    virtual Virus *clone() const;

    int getNode() const;

private:
    const int nodeInd;
};

#endif
