/**
 * File Header
 * Author: Anthony Yang
 * Email: a5yang@ucsd.edu
 * Sources: None
 * Description: This file contains the implementation for pathfinder, which
 * finds the shortest path between two actors
 */

#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "ActorGraph.hpp"

// argv positions
#define MOVIE_FILE 0
#define UW 1
#define ACTOR_FILE 2
#define OUTPUT_FILE 3

using namespace std;

vector<Edge*> findUnweightedPath(ActorGraph& graph, string actor1,
                                 string actor2) {
    Node* start = graph.getActorNode(actor1);
    Node* end = graph.getActorNode(actor2);
    queue<Node*> bfs;
    bfs.push(start);
    vector<Edge*> checked;
    while (!bfs.empty()) {
        // Runs BFS to find nearest path using a queue
        Node* current = bfs.front();
        bfs.pop();
        if (current == end) {
            clearEdges(
                checked);  // clears all edges so they are reset to unchecked
            return current->getPath();
        }
        vector<Edge*> neighborEdges = current->getEdges();
        for (int edge = 0; edge < neighborEdges.size(); edge++) {
            // Pushes neighbors onto queue
            Node* neighbor = neighborEdges[edge]->getDest();
            if (neighbor == current) {
                // Skips directed edges pointed back to itself from neighbor
                neighborEdges[edge]->check();
                checked.push_back(
                    neighborEdges[edge]);  // Store these in array so they can
                                           // be reset after path found
                continue;
            }
            if (neighborEdges[edge]->isChecked()) {
                // Edge will result in a cycle back to a checked node
                continue;
            }
            bfs.push(neighbor);
            vector<Edge*> path = neighbor->getPath();  // updates neighbor's
                                                       // path
            path.push_back(neighborEdges[edge]);
            neighbor->setPath(path);
        }
    }
    clearEdges(checked);
    return {};  // Path not found
}

/**
 * Resets all edges so that they are no longer checked; called after path is
 * found
 * Params:
 * - edges: the vector containing all edges that need to be reset
 * Returns: void
 */
void clearEdges(vector<Edge*> edges) {
    for (int edge = 0; edge < edges.size(); edge++) {
        edges[edge]->unCheck();
    }
}

/**
 * Runs pathfinder to find the shortest path between two actors
 * Params:
 *  - argc: the number of command line arguments (should be 4)
 *  - argv: contents of the command line arguments, should be movie file name
 *          u or w for weighted/unweighted, file name of file containing actor
 *          pairs, and output file name Returns: 0 if success, otherwise 1
 */
int main(int argc, char* argv[]) {
    ActorGraph graph;
    graph.loadFromFile(argv[MOVIE_FILE], argv[UW]);
}
