/**
 * File Header
 * Author: Anthony Yang
 * Email: a5yang@ucsd.edu
 * Sources: None
 * Description: This file contains the implementation for link predictor, which
 * predicts which actors are most likely to be together in the next movie.
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>

#include "ActorGraph.hpp"

// command line argument positions
#define MOVIE_FILE 1
#define ACTOR_FILE 2
#define COLLAB_FILE 3
#define UNCOLLAB_FILE 4

// Top rank 4 is printed
#define TOP_RANK 4

// Tabs between actors
#define ACTOR_DELIM '\t'

bool compare(pair<string, int> left, pair<string, int> right);

/**
 * Predicts the top four actors that are most likely to be in the next movie
 * with actor out of those who already collaborated with the actor
 * Params:
 * - graph: the graph of all actors
 * - actorName: name of actor to be predicted
 * Returns: top four actors in next movie (out of collaborators)
 */
vector<string> predictCollaborate(ActorGraph& graph, string actorName) {
    Node* actor = graph.getActorNode(actorName);
    vector<Edge*> edges = actor->getEdges();
    unordered_map<Node*, int> collaborators;
    // Finds all collaborators and number of movies both are in
    for (int edge = 0; edge < edges.size(); edge++) {
        if (collaborators.find(edges[edge]->getDest()) == collaborators.end()) {
            // collaborator not in map
            collaborators.insert({edges[edge]->getDest(), 1});
            continue;
        }
        collaborators[edges[edge]->getDest()]++;
    }
    vector<pair<string, int>> closest;

    // Loops through each collaborator in order to calculate their priority
    for (auto iter = collaborators.begin(); iter != collaborators.end();
         iter++) {
        vector<Edge*> neighbors = iter->first->getEdges();
        unordered_map<Node*, int> neighborMap;
        // sorts through all outgoing edges of neighbor node to find number of
        // edges between each neighbor and current actor
        for (int edge = 0; edge < neighbors.size(); edge++) {
            if (collaborators.find(neighbors[edge]->getDest()) ==
                    collaborators.end() ||
                neighbors[edge]->getDest() == iter->first) {
                // neighbor not a collaborator or itself
                continue;
            }
            if (neighborMap.find(neighbors[edge]->getDest()) ==
                neighborMap.end()) {
                // edge between collaborator and neighbor not in list
                // cout << "make it 1" << endl;
                neighborMap[neighbors[edge]->getDest()] = 1;

            } else {
                // cout << "why adding" << endl;
                neighborMap[neighbors[edge]->getDest()]++;
            }
        }
        int priority = 0;
        // Goes through neighbors to calculate priority
        for (auto iter2 = neighborMap.begin(); iter2 != neighborMap.end();
             iter2++) {
            // numEdges from actor to collab * numEdges from collab to current
            priority += (collaborators[iter2->first] * iter2->second);
        }
        pair<string, int> collaborator;
        collaborator.first = iter->first->getName();  // Name of collaborator
        collaborator.second = priority;  // priority of collaborator
        closest.push_back(collaborator);
    }
    // Sort vector from largest to smallest priority
    sort(closest.begin(), closest.end(), compare);
    vector<string> top;
    for (int rank = 0; rank < closest.size(); rank++) {
        if (rank == TOP_RANK) {
            break;  // 4 actors already in list
        }
        top.push_back(closest[rank].first);
    }
    return top;
}

bool compare(pair<string, int> left, pair<string, int> right) {
    if (left.second != right.second) {
        return left.second > right.second;
    } else {
        return left.first < right.first;
    }
}

/**
 * Predicts the top four actors that are most likely to be in the next movie
 * with actor out of those who haven't collaborated with the actor
 * Params:
 * - graph: the graph of all actors
 * - actorName: name of actor to be predicted
 * Returns: top four actors in next movie (out of non-collaborators)
 */
vector<string> predictNew(ActorGraph& graph, string actorName) {
    Node* actor = graph.getActorNode(actorName);
    vector<Edge*> edges = actor->getEdges();
    unordered_map<Node*, int> collaborators;
    // Finds all collaborators and number of movies both are in
    for (int edge = 0; edge < edges.size(); edge++) {
        if (collaborators.find(edges[edge]->getDest()) == collaborators.end()) {
            // collaborator not in map
            collaborators.insert({edges[edge]->getDest(), 1});
            continue;
        }
        collaborators[edges[edge]->getDest()]++;
    }
    unordered_map<Node*, int>
        noncollabs;  // one degree of separation apart from actor

    // Loops through each collaborator to find all actors that are one degree
    // apart
    for (auto iter = collaborators.begin(); iter != collaborators.end();
         iter++) {
        vector<Edge*> neighbors = iter->first->getEdges();
        // Loops through each edge to create list of non-collaborators
        for (int edge = 0; edge < neighbors.size(); edge++) {
            if (collaborators.find(neighbors[edge]->getDest()) ==
                    collaborators.end() &&
                neighbors[edge]->getDest() != actor) {
                // if the edge from the collaborator is not another collaborator
                // nor the original actor
                if (noncollabs.find(neighbors[edge]->getDest()) ==
                    noncollabs.end()) {
                    // noncollaborator not in hashmap yet
                    noncollabs[neighbors[edge]->getDest()] = 0;
                }
            }
        }
    }

    // Loops through each noncollaborator to calculate its priority
    for (auto iter = noncollabs.begin(); iter != noncollabs.end(); iter++) {
        vector<Edge*> neighbors = iter->first->getEdges();
        unordered_map<Node*, int> priorities;
        // Loops through each edge to find number of edges shared with a
        // collaborator
        for (int edge = 0; edge < neighbors.size(); edge++) {
            if (collaborators.find(neighbors[edge]->getDest()) !=
                collaborators.end()) {
                // Edge is with a collaborator
                if (priorities.find(neighbors[edge]->getDest()) ==
                    priorities.end()) {
                    // collaborator not in list
                    priorities[neighbors[edge]->getDest()] = 1;
                } else {
                    priorities[neighbors[edge]->getDest()]++;
                }
            }
        }
        // Calculates total priority of noncollaborator
        int priority = 0;
        for (auto iter2 = priorities.begin(); iter2 != priorities.end();
             iter2++) {
            // For each collab, find product between actor and collab and collab
            // and curr
            priority +=
                (collaborators.find(iter2->first)->second * iter2->second);
        }
        iter->second = priority;
    }

    vector<pair<string, int>> allNoncollabs;
    for (auto iter = noncollabs.begin(); iter != noncollabs.end(); iter++) {
        pair<string, int> noncollab;
        noncollab.first = iter->first->getName();
        noncollab.second = iter->second;
        allNoncollabs.push_back(noncollab);
    }

    std::sort(allNoncollabs.begin(), allNoncollabs.end(), compare);

    vector<string> top;
    for (int rank = 0; rank < allNoncollabs.size(); rank++) {
        if (rank == TOP_RANK) {
            break;  // 4 actors already added
        }
        top.push_back(allNoncollabs[rank].first);
    }
    return top;
}

/**
 * Parses test file to extract all actors in it
 * Params:
 * - actorFile: name of file containing actors
 * Returns: a vector of containing names of actors
 */
vector<string> parseFile(string actorFile) {
    ifstream infile(actorFile);
    vector<string> actors;
    bool have_header = false;

    while (!infile.eof()) {
        // retrieves each actor in file
        string line;
        if (!getline(infile, line)) {
            break;
        }
        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }
        actors.push_back(line);
    }
    infile.close();
    return actors;
}

/**
 * Runs link predictor to find the which actors most likely to be in next
 * movie
 * Params:
 *  - argc: the number of command line arguments (should be 4)
 *  - argv: contents of the command line arguments: should be name of file
 *          containing movie vasts, name of file containing actors algorithm
 *          is to be run on, output file name of actors current actor
 *          collaborated with, and output file name of actors who current actor
 *          haven't collaborated with pairs Returns: 0 if success, otherwise 1
 */
int main(int argc, char* argv[]) {
    ActorGraph graph;
    graph.loadFromFile(argv[MOVIE_FILE], false);
    vector<string> actors = parseFile(argv[ACTOR_FILE]);

    // Outputs to file
    ofstream outputCollab;
    ofstream outputUncollab;
    outputCollab.open(argv[COLLAB_FILE]);
    outputUncollab.open(argv[UNCOLLAB_FILE]);
    outputCollab << "Actor1,Actor2,Actor3,Actor4" << endl;
    outputUncollab << "Actor1,Actor2,Actor3,Actor4" << endl;
    for (int actor = 0; actor < actors.size(); actor++) {
        vector<string> collabs = predictCollaborate(graph, actors[actor]);
        vector<string> uncollabs = predictNew(graph, actors[actor]);
        // Outputs collaborators
        for (int collab = 0; collab < collabs.size(); collab++) {
            outputCollab << collabs[collab];
            outputCollab << ACTOR_DELIM;  // prints tab
        }
        outputCollab << endl;
        // Outputs new collaborators
        for (int uncollab = 0; uncollab < uncollabs.size(); uncollab++) {
            outputUncollab << uncollabs[uncollab];
            outputUncollab << ACTOR_DELIM;  // prints tab
        }
        outputUncollab << endl;
    }
    graph.clear();
    outputCollab.close();
    outputUncollab.close();
    return EXIT_SUCCESS;
}