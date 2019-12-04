/**
 * File Header
 * Author: Anthony Yang
 * Email: a5yang@ucsd.edu
 * Sources: None
 * Description: This file contains the implementation for the Edge object, which
 * connects two actors together when part of same movie.
 */

#include "ActorGraph.hpp"

using namespace std;

// Movies made in year 2019 will have weight 1
#define WEIGHT_YEAR 2019

/**
 * Constructs a new Edge from one actor to another given a shared movie
 * Params:
 * - srcActor: the Node pointer to the actor the edge is coming from
 * - destActor: the Node pointer to theactor the edge is pointing to
 * - movie_name: the movie struct containing the name of the movie both actors
 *               starred in
 */
Edge::Edge(Node* srcActor, Node* destActor, Movie movie_name) {
    src = srcActor;
    dest = destActor;
    name = movie_name;
    weight = 1 + (WEIGHT_YEAR - name.year);
    checked = false;
}

/**
 * Updates the weight with newWeight
 * Params:
 * - newWeight: the new weight of the edge
 * Returns: void
 */
void Edge::updateWeight(int newWeight) { weight = newWeight; }

/**
 * Returns the weight of the edge
 * Params: none
 * Returns: weight of edge
 */
int Edge::getWeight() { return weight; }


/**
 * Returns the name of the movie the two actors starred in
 * Params: none
 * Returns: name of movie
 */
Movie Edge::getName() { return name; }

/**
 * Checks off the node as being checked (usually the backwards of a directed
 * edge during search) Params: None Returns: None
 */
void Edge::check() { checked = true; }

/**
 * Returns if the edge has been checked
 * Params: None
 * Returns: if edge is checked
 */
bool Edge::isChecked() { return checked; }

/**
 * Resets checked and weight
 * Params: None
 * Returns: void
 */
void Edge::unCheck() {
    checked = false;
    weight = 1 + (WEIGHT_YEAR - name.year);
}

/**
 * Returns the node of the actor the edge is directed from
 * Params: none
 * Returns: name of source actor
 */
Node* Edge::getSource() { return src; }

/**
 * Returns the node of the actor the edge is directed to
 * Params: none
 * Returns: name of destination actor
 */
Node* Edge::getDest() { return dest; }