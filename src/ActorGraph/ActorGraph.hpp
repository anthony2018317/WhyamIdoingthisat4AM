/*
 * ActorGraph.hpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use
 * to read the input file format defined imdb_2019.tsv. Feel free to modify
 * any/all aspects as you wish.
 */

// #ifndef ACTORGRAPH_HPP
// #define ACTORGRAPH_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "Edge.hpp"
// #include "Movie.hpp"
// #include "Node.hpp"

using namespace std;

// Maybe include some data structures here

/**
 * This class represents an actor in a graph created from a list of movies. It
 * has vector of pairs as a field, with each pair containing a pointer to
 * another Node whose actor has beein in the same movie as this actor, and a
 * string which is the title of the movie. It also contains the actor's name as
 * a string.
 */
class ActorGraph {
  protected:
    // Maybe add class data structure(s) here
    map<Movie, vector<Node*> >
        movieList;  // Hash map mapping movie names to list of actors
    map<string, Node*>
        actorList;  // Hash map mapping actors to their nodes

  public:
    /**
     * Constuctor of the Actor graph
     */
    ActorGraph();

    // Maybe add some more methods here

    /** You can modify this method definition as you wish
     *
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     * use_weighted_edges - if true, compute edge weights as 1 + (2019 -
     * movie_year), otherwise all edge weights will be 1
     *
     * return true if file was loaded sucessfully, false otherwise
     */
    bool loadFromFile(const char* in_filename, bool use_weighted_edges);

    /**
     * Returns the node of the actor given their name
     * Params:
     * - actorName: the name of the actor
     * Returns: a pointer to the actor's Node, or nullptr if actor not in list
     */
    Node* getActorNode(string actorName);

    /**
     * Returns the list of actors of a particular movie given its name
     * Params:
     * - movieName: the name of the movie
     * Returns: a vector containing all the actors in the movie, or empty vector
     * if movie does not exist
     */
    vector<Node*> getMovieCast(Movie movieName);
};

// #endif  // ACTORGRAPH_HPP