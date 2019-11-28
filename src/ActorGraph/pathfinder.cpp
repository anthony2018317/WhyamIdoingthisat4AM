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

#include "ActorGraph.hpp"

// argv positions
#define MOVIE_FILE 0
#define UW 1
#define ACTOR_FILE 2
#define OUTPUT_FILE 3

// Delimiters in outputing paths
#define ACT_MOV "--"
#define MOV_ACT "-->"
#define LEFT_BRACE "("
#define RIGHT_BRACE ")"
#define LEFT_MOV_BRACE "["
#define RIGHT_MOV_BRACE "]"
#define NAME_YEAR "#@"

using namespace std;

void clearEdges(vector<Edge*> edges);

/**
 * Finds the closese unweighted path between actor1 and actor2
 * Params:
 * - graph: the graph of all actors
 * - actor1: the name of first actor
 * - actor2: the name of second actor
 * Returns: a vector of edges representing the path from actor1 to actor2
 */
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
 * Parses test file to extract actor pairs within it
 * Params:
 * - testfile: name of testfile
 * Returns: a vector of pairs containing actor pairs
 */
vector<pair<string, string>> parsePathFile(string testFile) {
    // Initialize the file stream
    ifstream infile(testFile);
    vector<pair<string, string>> actorList;
    bool have_header = false;

    while (!infile.eof()) {
        string line;

        // Gets the next line in file
        if (!getline(infile, line)) {
            break;
        }

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        stringstream ss(line);
        pair<string, string> actors;
        int pos = 1;

        while (ss) {
            string str;

            // get the next string before hitting a tab character and put it in
            // 'str'
            if (!getline(ss, str, '\t')) {
                break;
            }
            if (pos == 1) {  // first actor
                actors.first = str;
            } else {  // second actor
                actors.second = str;
            }
            pos++;
        }
        actorList.push_back(actors);
    }
    infile.close();
    return actorList;
}

/**
 * Runs pathfinder to find the shortest path between two actors
 * Params:
 *  - argc: the number of command line arguments (should be 4)
 *  - argv: contents of the command line arguments: should be movie file name
 *          u or w for weighted/unweighted, file name of file containing actor
 *          pairs, and output file name Returns: 0 if success, otherwise 1
 */
int main(int argc, char* argv[]) {
    ActorGraph graph;
    graph.loadFromFile(argv[MOVIE_FILE], argv[UW]);
    vector<pair<string, string>> actors = parsePathFile(argv[ACTOR_FILE]);
    ofstream output;
    output.open(argv[OUTPUT_FILE]);
    output << endl;
    for (int test = 0; test < actors.size(); test++) {
        // Runs pathfinder on all actor pairs in file
        vector<Edge*> path =
            findUnweightedPath(graph, actors[test].first, actors[test].second);
        for (int edge = 0; edge < path.size(); edge++) {
            if (edge == 0) {  // If printing first actor in path
                output << LEFT_BRACE << path[edge]->getSource()->getName()
                       << RIGHT_BRACE;  // prints first actor
            }
            output << ACT_MOV;  // prints first delimiter
            output << LEFT_MOV_BRACE << path[edge]->getName().name << NAME_YEAR
                   << path[edge]->getName().year
                   << RIGHT_MOV_BRACE;  // prints movie and year
            output << MOV_ACT;          // prints second delimiter
            output << LEFT_BRACE << path[edge]->getDest()->getName()
                   << RIGHT_BRACE;  // prints second actor
        }
        output << endl;  // prints new line
    }
    output.close();
}
