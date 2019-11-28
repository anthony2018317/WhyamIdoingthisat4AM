#ifndef NODE_HPP
#define NODE_HPP

// #include "ActorGraph.hpp"
// #include "Edge.hpp"

using namespace std;

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