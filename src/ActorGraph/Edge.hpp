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

class Node {
  private:
    vector<Edge*> edges;  // List of edges connected to this node
    vector<Edge*> path;   // Path from some node to this node
    string name;          // Name of actor

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
     * Sets the path from some other node to this node
     * Params:
     * - thePath: a vector containing nodes from some actor to this actor
     * Returns: void
     */
    void setPath(vector<Edge*> thePath);

    /**
     * Returns the path from some actor to this actor
     * Params: None
     * Returns: a vector containing a list of nodes that are traversed to get to
     * this node
     */
    vector<Edge*> getPath();

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

#endif