#ifndef SELECTEDSPHERENODE_H
#define SELECTEDSPHERENODE_H

#include "spherenode.h"

class SelectedSphereNode
{
public:
    SelectedSphereNode() : node(0), dist(-1) {}
    SelectedSphereNode(SphereNode* anode) : node(anode), dist(-1) {}


    SphereNode* getNode() const { return node; }
    float getDist() const { return dist; }

    void setInvalid() { node = 0; }
    void setNode(SphereNode* anode) { node = anode; }
    void setDist(float d) { dist = d; }

    bool valid() const { return node != 0; }

private:
    SphereNode* node;
    float dist;
};

#endif // SELECTEDSPHERENODE_H
