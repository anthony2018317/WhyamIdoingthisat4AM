/**
 * File Header
 * Author: Anthony Yang
 * Email: a5yang@ucsd.edu
 * Sources: None
 * Description: This file contains the implementation for movie traveler, which
 * calculates the minimum path to visit all actors.
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

#include "ActorGraph.hpp"

using namespace std;

// Command line argument positions
#define MOVIEFILE 1
#define OUTFILE 2

// File header for output file
#define HEADER "(actor)<--[movie#@year]-->(actor)"

// Delimiters for output
#define OPENING "("
#define CLOSING ")"
#define MOVIE_YEAR "#@"
#define ACTORMOVIE ")<--["
#define MOVIEACTOR "]-->("

#define FIRSTLINE "#NODE CONNECTED: "
#define SECONDLINE "#EDGE CHOSEN: "
#define THIRDLINE "TOTAL EDGE WEIGHTS: "

// Random actor to find first node to add all edges
#define ACTOR "Kevin Bacon"

/**
 * This method finds all edges in a particular graph
 * Params:
 *  - graph: a refeence of the graph containing all actor nodes
 * Returns: a priority queue ranking all edges in terms of edge weights
 */
priority_queue<Edge*, vector<Edge*>, EdgeComp> findEdges(ActorGraph& graph) {
    priority_queue<Edge*, vector<Edge*>, EdgeComp> edges;

    queue<Node*> nodes;
    Node* start = graph.getActorNode(ACTOR);
    nodes.push(start);
    unordered_map<Node*, int> finished;
    finished[start] = 1;

    while (!nodes.empty()) {
        Node* current = nodes.front();

        nodes.pop();

        vector<Edge*> neighbors = current->getEdges();
        for (int edge = 0; edge < neighbors.size(); edge++) {
            // for each neighboring edge, add edges to edgeList
            edges.push(neighbors[edge]);
            if (finished.find(neighbors[edge]->getDest()) == finished.end()) {
                // Node not in list
                nodes.push(neighbors[edge]->getDest());
                finished[neighbors[edge]->getDest()] = 1;
            }
        }
    }
    return edges;
}

/**
 * This method finds the shortest path given a list of edges that comprise the
 * graph
 * Params:
 *  - edges: the priority queue of edges, ranked by which higher priority is
 *          lower weight
 * Returns: A vector of edges representing the MST
 */
vector<Edge*> findShortestPath(
    priority_queue<Edge*, vector<Edge*>, EdgeComp>& edges) {
    unordered_map<Node*, unordered_set<Node*>> uptrees;
    vector<Edge*> thePath;

    while (!edges.empty()) {
        Edge* edge1 = edges.top();

        edges.pop();
        Edge* edge2 = nullptr;
        if (!edges.empty()) {
            edge2 = edges.top();
            if (edge2->getName() == edge1->getName() &&
                edge2->getDest() == edge1->getSource()) {
                // Also pops from pq the equivalent edge pointing in opposite
                // direction
                edges.pop();
            } else {
                edge2 = nullptr;
            }
        }

        Node* source = edge1->getSource();
        Node* dest = edge1->getDest();
        if (source->getSentinel() != nullptr &&
            dest->getSentinel() != nullptr) {
            // Both Nodes are in uptrees
            if (source->getSentinel() == dest->getSentinel()) {
                // In same uptree
                continue;
            } else {
                // In different uptrees

                unordered_set<Node*> uptreeSource =
                    uptrees[source->getSentinel()];
                unordered_set<Node*> uptreeDest =
                    uptrees[source->getSentinel()];
                if (uptreeSource.size() > uptreeDest.size()) {
                    // source's uptree is bigger
                    thePath.push_back(edge1);
                    dest->setPrev(edge1);
                    uptrees.erase(dest->getSentinel());
                    // Makes source's sentinel have a pointer to base of dest's
                    // uptree
                    source->getSentinel()->setBase(
                        dest->getSentinel()->getBase());
                    for (auto iter = uptreeDest.begin();
                         iter != uptreeDest.end(); iter++) {
                        Node* current = *iter;
                        uptreeSource.insert(current);
                        current->setSentinel(source->getSentinel());
                    }
                    uptrees[source->getSentinel()] = uptreeSource;

                } else {
                    // source's uptree is smaller
                    thePath.push_back(edge1);
                    dest->setPrev(edge1);
                    // Make dest's sentinel have a pointer to base of source's
                    // uptree
                    uptrees.erase(source->getSentinel());
                    for (auto iter = uptreeSource.begin();
                         iter != uptreeSource.end(); iter++) {
                        Node* current = *iter;
                        uptreeDest.insert(current);
                        current->setSentinel(dest->getSentinel());
                    }
                    uptrees[dest->getSentinel()] = uptreeDest;
                }
                continue;
            }
        }
        if (source->getSentinel() != nullptr) {
            // source is in an uptree but dest is not
            unordered_set<Node*> uptree = uptrees[source->getSentinel()];
            uptree.insert(dest);
            thePath.push_back(edge1);
            uptrees[source->getSentinel()] = uptree;
            dest->setSentinel(source->getSentinel());
            if (source != source->getSentinel()->getBase()) {
                // Not a base
                if (edge2 == nullptr) {
                    // edge2 not the reverse of edge1
                    edge2 = dest->findEdge(source->getName());
                    if (dest == nullptr) {
                        continue;
                    }
                }
                dest->setPrev(edge2);
                source->getSentinel()->setBase(dest);

            } else {
                dest->setPrev(edge1);
                source->getSentinel()->setBase(dest);
            }

            continue;
        }
        if (dest->getSentinel() != nullptr) {
            // dest is in an uptree but source is not

            source->setSentinel(dest->getSentinel());
            dest->setPrev(edge1);
            thePath.push_back(edge1);
            unordered_set<Node*> uptree = uptrees[dest->getSentinel()];
            uptree.insert(source);
            if (dest != dest->getSentinel()->getBase()) {
                // Not a base
                if (edge2 == nullptr) {
                    // edge2 not the reverse of edge1
                    edge2 = dest->findEdge(source->getName());
                    if (dest == nullptr) {
                        continue;
                    }
                }
                dest->setPrev(edge2);
                dest->getSentinel()->setBase(source);
            } else {
                dest->setPrev(edge1);
                dest->getSentinel()->setBase(source);
            }

            uptrees[dest->getSentinel()] = uptree;
            continue;
        }

        dest->setPrev(edge1);

        source->setSentinel(source);
        dest->setSentinel(source);
        source->setBase(dest);
        unordered_set<Node*> uptree = {source, dest};
        uptrees[source] = uptree;
        thePath.push_back(edge1);
    }
    Node* current = uptrees.begin()->first->getBase();
    if (current == nullptr) {
        return {};
    }

    return thePath;
}

/**
 * Runs movie traveler to find the optimal path to visit all actors
 * Params:
 *  - argc: the number of command line arguments (should be 2)
 *  - argv: contents of the command line arguments: should be the name of
 *          all actors and movies they have played in, and the second
 *          argument should be the output file path
 * Returns: 0 if success, otherwise 1
 */
int main(int argc, char* argv[]) {
    ActorGraph graph;
    graph.loadFromFile(argv[MOVIEFILE], true);
    priority_queue<Edge*, vector<Edge*>, EdgeComp> edges = findEdges(graph);

    vector<Edge*> shortest = findShortestPath(edges);
    ofstream os;
    os.open(argv[OUTFILE]);

    // prints to file
    os << HEADER << endl;
    int weight = 0;
    for (int edge = 0; edge < shortest.size(); edge++) {
        // prints each edge
        os << OPENING << shortest[edge]->getSource()->getName() << ACTORMOVIE
           << shortest[edge]->getName().name << MOVIE_YEAR
           << shortest[edge]->getName().year << MOVIEACTOR
           << shortest[edge]->getDest()->getName() << CLOSING << endl;
        weight += shortest[edge]->getWeight();
    }

    os << FIRSTLINE << (shortest.size() + 1) << endl;
    os << SECONDLINE << (shortest.size()) << endl;
    os << THIRDLINE << weight << endl;

    graph.clear();
    os.close();
    return EXIT_SUCCESS;
}
