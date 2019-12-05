/**
 * File Header
 * Author: Anthony Yang
 * Email: a5yang@ucsd.edu
 * Sources: None
 * Description: This file contains the implementation for the Node object, which
 * represents an actor in the graph of actors to be created from a list of
 * movies.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "ActorGraph.hpp"

#include "Edge.hpp"
// #include "Movie.hpp"
// #include "Node.hpp"

// using namespace std;

/**
 * Constructs a new Node with the name of the actor
 * Params:
 *  - actor_name: the name of the actor
 */
Node::Node(string actor_name) {
    name = actor_name;
    prev = nullptr;
    pathWeight = MAX_WEIGHT;
    sentinel = nullptr;
    base = nullptr;
}

/**
 * Adds an edge between this actor and another actor, given the movie name
 * and a pointer to the other actor
 * Params:
 * - other: a pointer to the Node containing the other actor
 * - movie: the name of the movie that joins the two actors
 * Returns: void
 */
void Node::addEdge(Node* other, Movie movie) {
    Edge* edge = new Edge(this, other, movie);
    edges.push_back(edge);
}

void Node::check() { done = true; }

int Node::getPathWeight() { return pathWeight; }

void Node::uncheck() {
    done = false;
    prev = nullptr;
    base = nullptr;
    sentinel = nullptr;
    pathWeight = MAX_WEIGHT;
}

bool Node::isDone() { return done; }

/**
 * Adds an edge between this actor and another actor, given an existing
 * pointer to an edge
 * Params:
 * - newEdge: the new connecting edge between this actor node and another
 * Returns: void
 */
void Node::addEdge(Edge* newEdge) { edges.push_back(newEdge); }

void Node::setSentinel(Node* sent) { sentinel = sent; }

Node* Node::getSentinel() { return sentinel; }

/**
 * Sets the path from some other node to this node
 * Params:
 * - thePath: a vector containing nodes from some actor to this actor
 * Returns: void
 */
void Node::setPrev(Edge* thePrev) {
    if (thePrev == nullptr) {
        // cout << "being set to nullptr: " << name << endl;
        return;
    }
    prev = thePrev;
}

void Node::setPrev(Edge* thePrev, int weight) {
    if (thePrev == nullptr) {
        return;
    }
    prev = thePrev;
    pathWeight = weight + thePrev->getWeight();  // Calculates total weight
}

void Node::setStart() {
    prev = nullptr;
    pathWeight = 0;
}

void Node::setBase(Node* theBase) { base = theBase; }

Node* Node::getBase() { return base; }

/**
 * Returns the path from some actor to this actor
 * Params: None
 * Returns: a vector containing a list of nodes that are traversed to get to
 * this node
 */
Edge* Node::getPrev() { return prev; }

/**
 * Returns a vector of all other actors who starred in same movie as this actor
 * Params: None
 * Returns: the vector of all other actors in same movies
 */
vector<Edge*> Node::getEdges() { return edges; }

/**
 * Returns the name of the actor contained by this node
 * Params: none
 * Returns: name of actor
 */
string Node::getName() { return name; }

/**
 * Returns the edge pair connecting the node of this actor and the other
 * actor given other actor's name
 * Params:
 * - actor: the name of other actor
 * Returns: the pair containing the edge between this actor and the other
 * actor, or empty pair if no movie links them both
 */
Edge* Node::findEdge(string actor) {
    for (int edge = 0; edge < edges.size(); edge++) {
        if (edges[edge]->getDest()->getName() ==
            actor) {  // Actor found in edges list
            return edges[edge];
        }
    }
    return nullptr;
}

/**
 * Destructor for Node, frees all edges
 * Params: None
 */
Node::~Node() {
    for (int edge = 0; edge < edges.size(); edge++) {
        delete edges[edge];
    }
}
