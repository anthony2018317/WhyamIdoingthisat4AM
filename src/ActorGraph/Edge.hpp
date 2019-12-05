#ifndef EDGE_HPP
#define EDGE_HPP

// #include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Movie.hpp"
// #include "Node.hpp"

#define MAX_WEIGHT 0x7FFFFFFF  // Maximum weight

using namespace std;
class Node;

/**
 * This class represents a connection between two actors (represented as nodes)
 * when they star in a movie together. It contains pointers to the two actor
 * nodes, the sting of the movie, and the weight of the edge (number of movies
 * they aired in togerher).
 */
class Edge {
  private:
    Node* src;     // Node the edge is being directed from
    Node* dest;    // Node the edge is being directed to
    bool checked;  // True if destination node has already been checked
    Movie name;    // Name of movie
    int weight;    // Weight of edge

  public:
    /**
     * Constructs a new Edge from one actor to another given a shared movie
     * Params:
     * - srcActor: the Node pointer to the actor the edge is coming from
     * - destActor: the Node pointer to theactor the edge is pointing to
     * - movie_name: the movie struct containing the name of the movie both
     *               actors starred in
     */
    Edge(Node* srcActor, Node* destActor, Movie movieName);

    /**
     * Updates the weight with newWeight
     * Params:
     * - newWeight: the new weight of the edge
     * Returns: void
     */
    void updateWeight(int newWeight);

    /**
     * Resets checked
     * Params: None
     * Returns: void
     */
    void unCheck();

    /**
     * Returns the weight of the edge
     * Params: none
     * Returns: weight of edge
     */
    int getWeight();

    /**
     * Returns the struct containing the name of the movie the two actors
     * starred in
     * Params: none Returns: name of movie
     */
    Movie getName();

    /**
     * Returns the node of the actor the edge is directed from
     * Params: none
     * Returns: name of source actor
     */
    Node* getSource();

    /**
     * Returns the node of the actor the edge is directed to
     * Params: none
     * Returns: name of destination actor
     */
    Node* getDest();

    /**
     * Checks off the node as being checked (usually the backwards of a directed
     * edge during search) Params: None Returns: None
     */
    void check();

    /**
     * Returns if the edge has been checked
     * Params: None
     * Returns: if edge is checked
     */
    bool isChecked();
};

/** Comparator of Edges. In priority queue, Edge with movie of an earlier date
 *  have lower priority than those with a later date.
 */

class Node {
  private:
    vector<Edge*> edges;  // List of edges connected to this node
    Edge* prev;           // Path from some node to this node
    string name;          // Name of actor
    int pathWeight = MAX_WEIGHT;
    bool done = false;  // True if checked
    Node* sentinel;     // Sentinel node of Node's subtree
    Node* base;  // For sentinel node only: pointer to base node of uptree

  public:
    /**
     * Constructs a new Node with the name of the actor
     * Params:
     *  - actor_name: the name of the actor
     */
    Node(string actor_name);

    /**
     * Adds an edge between this actor and another actor, given the movie name
     * and a pointer to the other actor
     * Params:
     * - other: a pointer to the Node containing the other actor
     * - movie: the name of the movie that joins the two actors
     * Returns: void
     */
    void addEdge(Node* other, Movie movie);

    /**
     * Adds an edge between this actor and another actor, given an existing
     * pointer to an edge
     * Params:
     * - newEdge: the new connecting edge between this actor node and another
     * Returns: void
     */
    void addEdge(Edge* newEdge);

    /**
     * Marks a Node as done
     * Params: None
     * Returns: void
     */
    void check();

    /**
     * Resets the Node
     * Params: None
     * Returns: void
     */
    void uncheck();

    /**
     * Returns if the Node is checked
     * Return: if the Node was checked
     */
    bool isDone();

    /**
     * Returns the pathweight
     * Return: the weight of the shortest path to the Node
     */
    int getPathWeight();

    /**
     * Sets the Node to be start Node
     * Params: None
     * Returns: void
     */
    void setStart();

    /**
     * Retuns the base of the uptree
     * Returns: the base of the uptree
     */
    Node* getBase();

    /**
     * Sets the base of the uptree to theBase
     * Params:
     * - theBase: the base of the uptree of the Node
     */
    void setBase(Node* theBase);

    /**
     * Gets the sentinel Node
     * Returns: The sentinel node of this Node
     */
    Node* getSentinel();

    /**
     *  Sets the sentinel Node to sent
     *  Params:
     *  - sent: the sentinel node
     */
    void setSentinel(Node* sent);

    /**
     * Sets the path from some other node to this node
     * Params:
     * - thePath: a vector containing nodes from some actor to this actor
     * Returns: void
     */
    void setPrev(Edge* thePrev);

    /**
     * Sets the previous of the Node
     * Params:
     * - thePrev: the Edge that is the previous of the Node
     * - weight: the weight of the path leading up to the previous Node
     */
    void setPrev(Edge* thePrev, int weight);

    /**
     * Returns the path from some actor to this actor
     * Params: None
     * Returns: a vector containing a list of nodes that are traversed to get to
     * this node
     */
    Edge* getPrev();

    /**
     * Returns the name of the actor contained by this node
     * Params: none
     * Returns: name of actor
     */
    string getName();

    /**
     * Returns the edge pair connecting the node of this actor and the other
     * actor given other actor's name
     * Params:
     * - actor: the name of other actor
     * Returns: the pair containing the edge between this actor and the other
     * actor, or empty pair if no movie links them both
     */
    Edge* findEdge(string actor);

    /**
     * Returns a vector of all other actors who starred in same movie as this
     * actor Params: None Returns: the vector of all other actors in same movies
     */
    vector<Edge*> getEdges();

    /**
     * Destructor for Node, frees all edges
     * Params: None
     */
    ~Node();
};

/**
 * This struct compares two Node*-weight pairs to be used for priority queue
 * ordering
 */
struct NodePairComp {
    /**
     * This operator compares two Node-weight pairs, returning true if rhs has
     * higher priority (lower weight) and false if lhs has higher priority
     * (lower weight)
     * Params:
     *  - lhs: the left hand side Edge
     *  - rhs: the right hand side Edge
     * Returns true if RHS > LHS
     */
    bool operator()(pair<Node*, int>& lhs, pair<Node*, int>& rhs) const {
        if (rhs.second != lhs.second) {
            return lhs.second > rhs.second;
        }
        return lhs.first->getName() > rhs.first->getName();
    }
};

/**
 * This struct compares two Edges to be used for priority queue ordering
 */
struct EdgeComp {
    /**
     * This operator compares two Edges, returning true if rhs has higher
     * priority (lower weight) and false if lhs has higher priority (lower
     * weight)
     * Params:
     *  - lhs: the left hand side Edge
     *  - rhs: the right hand side Edge
     * Returns true if RHS > LHS
     */
    bool operator()(Edge* lhs, Edge* rhs) const {
        if (rhs->getWeight() != rhs->getWeight()) {
            return lhs->getWeight() > rhs->getWeight();
        }
        return lhs->getWeight() > rhs->getWeight();
    }
};

#endif