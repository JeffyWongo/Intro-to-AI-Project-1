#include "Problem.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <sstream>

using namespace std;

vector<int> getUserPuzzle(vector<int> puzzle, string &userInput)
{ // take rows and fill puzzle
    getline(cin, userInput);
    stringstream ss(userInput);
    int temp_int;
    while (ss >> temp_int)
    {
        puzzle.push_back(temp_int);
    }
    return puzzle;
}

struct CompareNode
{
    bool operator()(Node *&a, Node *&b)
    {
        return a->get_fCost() > b->get_fCost();
    }
};

void printState(Node *node)
{
    for (int i = 0; i < node->get_state().size(); i += 3)
    {
        cout << node->get_state().at(i) << " ";
        if (i + 1 < node->get_state().size())
        {
            cout << node->get_state().at(i + 1) << " ";
        }
        if (i + 2 < node->get_state().size())
        {
            cout << node->get_state().at(i + 2) << " ";
        }
        cout << endl;
    }
}

int nodeDifference(vector<int> state)
{
    vector<int> goalState = {1, 2, 3, 4, 5, 6, 7, 8, 0};
    int h = 0;
    for (int i = 0; i < state.size(); i++)
    {
        if (goalState.at(i) != state.at(i))
        {
            h++;
        }
    }
    return h;
}

double findEuclid(vector<int> state)
{ // distance from current to correct,
    double EucDistance = 0;
    for (int i = 0; i < state.size(); i++)
    {                      //* if current position is not correct position, find correct position and take sqrt((x_2 - x_1)^2 + (y^2 - y^1)^2) *//
        if (state[i] == 0) // hit blank spot, continue loop
            continue;
        if (state[i] != 0)
        {
            int puzSize = sqrt(state.size());
            int x1 = i / puzSize;                                   // goal row (0-2), index 0 accounted for?
            int y1 = i % puzSize;                                   // goal column (0-2)
            int x2 = (state[i] - 1) / puzSize;                      // curr row (0-2)
            int y2 = (state[i] - 1) % puzSize;                      // curr column (0-2) 
            EucDistance += sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)); // formula to calculate Euclid using sqrt(goal state - current state)^2, if tile is correct should return 0.
        }
    }
    return EucDistance;
}

priority_queue<Node *, vector<Node *>, CompareNode> uniform(priority_queue<Node *, vector<Node *>, CompareNode> nodes, vector<Node *> new_nodes)
{
    for (auto i = new_nodes.begin(); i != new_nodes.end(); i++)
    {
        Node *node = *i;
        node->set_hCost(0);
        node->set_fCost(node->get_gCost() + 0);
        nodes.push(node);
    }
    return nodes;
}

priority_queue<Node *, vector<Node *>, CompareNode> a_star_misplaced(priority_queue<Node *, vector<Node *>, CompareNode> nodes, vector<Node *> new_nodes)
{
    for (auto i = new_nodes.begin(); i != new_nodes.end(); i++)
    {
        Node *node = *i;
        int difference = nodeDifference(node->get_state());
        node->set_hCost(difference);
        node->set_fCost(node->get_gCost() + difference); // f = g + h
        nodes.push(node);
    }
    return nodes;
}

priority_queue<Node *, vector<Node *>, CompareNode> aStarEuclidean(priority_queue<Node *, vector<Node *>, CompareNode> nodes, vector<Node *> new_nodes)
{
    for (auto i = new_nodes.begin(); i != new_nodes.end(); i++)
    {
        Node *node = *i;
        double euclid = findEuclid(node->get_state());
        node->set_hCost(euclid);
        node->set_fCost(node->get_gCost() + euclid);
        nodes.push(node);
    }
    return nodes;
}

Node *graph_search(Problem problem, int choice)
{
    priority_queue<Node *, vector<Node *>, CompareNode> nodes;
    Node *initial_node = new Node(problem.getInitialState());
    initial_node->set_gCost(0);
    initial_node->set_hCost(0);
    initial_node->set_fCost(0);
    nodes.push(initial_node);
    cout << "Expanding state" << endl;
    printState(initial_node);

    int maxQueueSize = nodes.size();
    while (true)
    {
        if (nodes.empty())
        {
            cout << "Failed to find any possible solutions" << endl;
            cout << "The search algorithm expanded a total of "
                 << problem.getNodeCount() << " nodes." << endl;
            cout << "The maximum number of nodes in the queue at any one time: "
                 << maxQueueSize << "." << endl;
            return nullptr;
        }
        Node *node = nodes.top();
        nodes.pop();
        if (problem.goal_test(node->get_state()))
        {
            cout << "Goal!!" << endl;
            cout << "To solve this problem the search algorithm expanded a total of "
                 << problem.getNodeCount() << " nodes." << endl; // implement getnodecount
            cout << "The maximum number of nodes in the queue at any one time: "
                 << maxQueueSize << "." << endl; // implement maxqueuesize
            cout << "The depth of the goal node was " << node->get_gCost() << endl;
            return node;
        }

        cout << "The best state to expand with a g(n) = " << node->get_gCost() << " and h(n) = " << node->get_hCost() << " is ..." << endl;
        printState(node);
        cout << "Expanding this node..." << endl << endl;
        // implement search choice
        if (choice == 1)
        {
            nodes = uniform(nodes, problem.operators(node));
        }

        else if (choice == 2)
        {
            nodes = a_star_misplaced(nodes, problem.operators(node));
        }

        else if (choice == 3)
        {
            nodes = aStarEuclidean(nodes, problem.operators(node));
        }

        int sizeAttempt = nodes.size();
        if (sizeAttempt > maxQueueSize)
        {
            maxQueueSize = sizeAttempt;
        }
    }
}

int main()
{
    vector<int> puzzleBoard;
    string userInput;
    bool makingPuzzle = true;
    while (makingPuzzle)
    {
        cout << "Welcome to Jeffrey Hwang's (862093523) 8-Puzzle Solver.\n Type '1' to use a default puzzle, or '2' to enter your own puzzle. " << endl;
        cin >> userInput;
        if (userInput == "1")
        {
            puzzleBoard = {1, 2, 3,
                           4, 8, 0,
                           7, 6, 5};
            makingPuzzle = false; // exit loop
        }
        else if (userInput == "2")
        {
            cin.ignore();
            cout << "Enter your puzzle, use a zero to represent the blank." << endl;
            cout << "Enter the first row, use space between numbers: " << endl;
            puzzleBoard = getUserPuzzle(puzzleBoard, userInput);
            cout << "Enter the second row, use space between numbers: " << endl;
            puzzleBoard = getUserPuzzle(puzzleBoard, userInput);
            cout << "Enter the third row, use space between numbers: " << endl;
            puzzleBoard = getUserPuzzle(puzzleBoard, userInput);
            makingPuzzle = false; // exit loop
        }
        else
        {
            cout << "Invalid input! Enter only '1' or '2'! " << endl;
        }
    }

    int algoChoice;

    cout << "Enter your choice of algorithm:\n (1) Uniform Cost Search\n (2) A* with the Misplaced Tile heuristic\n (3) A* with the Euclidean distance heuristic\n"
         << endl;
    cin >> algoChoice;

    while (algoChoice < 1 || algoChoice > 3)
    { // do we need this?
        cout << "Error invalid choice!" << endl;
        cin >> algoChoice;
    }

    Problem problem(puzzleBoard);

    Node *node = graph_search(problem, algoChoice);

    return 0;
}