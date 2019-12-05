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

priority_queue<Edge*, vector<Edge*>, EdgeComp> findEdges(ActorGraph& graph) {
    priority_queue<Edge*, vector<Edge*>, EdgeComp> edges;

    queue<Node*> nodes;
    Node* start = graph.getActorNode(ACTOR);
    nodes.push(start);
    unordered_map<Node*, int> finished;
    finished[start] = 1;
    // unordered_map<Node*, int> checked;
    while (!nodes.empty()) {
        Node* current = nodes.front();
        // cout << current->getName() << endl;
        nodes.pop();
        // checked[current] = 1;
        vector<Edge*> neighbors = current->getEdges();
        for (int edge = 0; edge < neighbors.size(); edge++) {
            // for each neighboring edge, add edges to edgeList
            edges.push(neighbors[edge]);
            if (finished.find(neighbors[edge]->getDest()) == finished.end()) {
                // Node not in list
                /*cout << "source: " << neighbors[edge]->getSource()->getName()
                     << " ";
                cout << "dest: " << neighbors[edge]->getDest()->getName()
                     << " ";
                cout << "movie: " << neighbors[edge]->getName().title << " "
                     << endl;*/
                nodes.push(neighbors[edge]->getDest());
                finished[neighbors[edge]->getDest()] = 1;
            }

            /*if (checked.find(neighbors[edge]->getDest()) != checked.end()) {
                // Excludes all directed edges pointing back to a checked node
                edges.push(neighbors[edge]);
                nodes.push(neighbors[edge]->getDest());
            }*/
        }
    }
    return edges;
}

vector<Edge*> findShortestPath(
    priority_queue<Edge*, vector<Edge*>, EdgeComp>& edges) {
    unordered_map<Node*, unordered_set<Node*>> uptrees;
    vector<Edge*> thePath;
    // cout << edges.size() << endl;
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
                // cout << "skipped" << endl;
                edge2 = nullptr;
            }
        }

        Node* source = edge1->getSource();
        Node* dest = edge1->getDest();
        /*cout << "source: " << source->getName() << " ";
        cout << "dest: " << dest->getName() << " ";
        cout << "movie: " << edge1->getName().title << " " << endl;*/
        if (source->getSentinel() != nullptr &&
            dest->getSentinel() != nullptr) {
            // Both Nodes are in uptrees
            if (source->getSentinel() == dest->getSentinel()) {
                // In same uptree
                // cout << "case 1" << endl;
                continue;
            } else {
                // In different uptrees

                unordered_set<Node*> uptreeSource =
                    uptrees[source->getSentinel()];
                unordered_set<Node*> uptreeDest =
                    uptrees[source->getSentinel()];
                if (uptreeSource.size() > uptreeDest.size()) {
                    // source's uptree is bigger
                    // cout << "case 2" << endl;
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
                    // cout << "case 3" << endl;
                    // cout << uptreeSource.size() << endl;
                    // cout << uptreeDest.size() << endl;

                    /*if (edge2 == nullptr) {
                        // edge2 not the reverse of edge1
                        edge2 = dest->findEdge(source->getName());
                        if (dest == nullptr) {
                            continue;
                        }
                    }*/
                    thePath.push_back(edge1);
                    dest->setPrev(edge1);
                    // Make dest's sentinel have a pointer to base of source's
                    // uptree
                    // dest->getSentinel()->setBase(
                    //    source->getSentinel()->getBase());
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
            // cout << "case 4" << endl;
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
            // cout << "case 5" << endl;

            source->setSentinel(dest->getSentinel());
            dest->setPrev(edge1);
            thePath.push_back(edge1);
            // dest->getSentinel()->setBase(source);
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
        // dest and source are not in uptrees
        // cout << "case 6" << endl;
        dest->setPrev(edge1);
        /*if (edge2 == nullptr) {
            // edge2 not the reverse of edge1
            edge2 = dest->findEdge(source->getName());
            if (dest == nullptr) {
                continue;
            }
        }
        source->setPrev(edge2);*/

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
    // vector<Edge*> path;
    // cout << "reached end" << endl;
    /*while (current != nullptr) {
        // Goes from base (beginning) to sentinel (end)
        cout << "current: " << current->getName() << endl;
        cout << "sentinel: " << current->getSentinel()->getName() << endl;
        path.insert(path.begin(), current->getPrev());
        current = current->getPrev()->getSource();
    }
    return path;*/
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
    // cout << "made it past findedges" << endl;
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
