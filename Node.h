#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <vector>
using namespace std;

class Node
{
private:
    double gCost;
    double hCost;
    double fCost;
    vector<int> state;

public:
    Node(vector<int> state)
    {
        this->state = state;
    }

    vector<int> get_state()
    {
        return state;
    }

    void set_gCost(double gCost)
    {
        this->gCost = gCost;
    }

    double get_gCost()
    {
        return gCost;
    }

    void set_hCost(double hCost)
    {
        this->hCost = hCost;
    }

    double get_hCost()
    {
        return hCost;
    }

    void set_fCost(double fCost)
    {
        this->fCost = fCost;
    }

    double get_fCost()
    {
        return fCost;
    }
};

#endif