#ifndef PROBLEM_H
#define PROBLEM_H
#include "Node.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>

using namespace std;

class Problem
{
private:
    vector<int> initialState;
    vector<int> goalState = {1, 2, 3, 4, 5, 6, 7, 8, 0};
    vector<vector<int>> visitedStates;
    int node_count = 0;

public:
    Problem(vector<int> puzzle)
    { // constructor
        this->initialState = puzzle;
    }

    vector<int> getInitialState()
    {
        return initialState;
    }

    int getNodeCount()
    {
        return node_count;
    }

    vector<int> moveUp(int index, vector<int> &currState)
    {
        vector<int> newState = currState;
        swap(newState[index], newState[index - 3]);
        return newState;
    }

    vector<int> moveDown(int index, vector<int> &currState)
    {
        vector<int> newState = currState;
        swap(newState[index], newState[index + 3]);
        return newState;
    }

    vector<int> moveLeft(int index, vector<int> &currState)
    {
        vector<int> newState = currState;
        swap(newState[index], newState[index - 1]);
        return newState;
    }

    vector<int> moveRight(int index, vector<int> &currState)
    {
        vector<int> newState = currState;
        swap(newState[index], newState[index + 1]);
        return newState;
    }

    int findBlank(vector<int> state)
    {
        for (int i = 0; i < state.size(); i++)
        {
            if (state.at(i) == 0)
            {
                return i;
            }
        }
        return 0;
    }

    bool goal_test(vector<int> state)
    {
        return goalState == state;
    }

    bool visited(vector<int> state)
    {
        for (int i = 0; i < visitedStates.size(); i++)
        {
            if (visitedStates.at(i) == state)
            {
                return true;
            }
        }
        visitedStates.push_back(state);
        return false;
    }

    vector<Node *> operators(Node *input_node)
    {
        node_count++;
        vector<Node *> operator_list;
        vector<int> curr_state = input_node->get_state();
        int zero_index = findBlank(curr_state);

        // Move up allowed
        if (zero_index > 2)
        {
            Node *up_node = new Node(moveUp(zero_index, curr_state));
            up_node->set_gCost(input_node->get_gCost() + 1);
            if (!visited(up_node->get_state()))
            {
                operator_list.push_back(up_node);
            }
        }

        // Move down allowed
        if (zero_index < 6)
        {
            Node *down_node = new Node(moveDown(zero_index, curr_state));
            down_node->set_gCost(input_node->get_gCost() + 1);
            if (!visited(down_node->get_state()))
            {
                operator_list.push_back(down_node);
            }
        }

        // Move left allowed
        if (zero_index % 3 > 0)
        {
            Node *left_node = new Node(moveLeft(zero_index, curr_state));
            left_node->set_gCost(input_node->get_gCost() + 1);
            if (!visited(left_node->get_state()))
            {
                operator_list.push_back(left_node);
            }
        }

        // Move right allowed
        if (zero_index % 3 < 2)
        {
            Node *right_node = new Node(moveRight(zero_index, curr_state));
            right_node->set_gCost(input_node->get_gCost() + 1);
            if (!visited(right_node->get_state()))
            {
                operator_list.push_back(right_node);
            }
        }

        return operator_list;
    }
};
#endif
