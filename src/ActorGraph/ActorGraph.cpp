/*
 * ActorGraph.cpp
 * Author: Anthony Yang
 * Email: a5yang@ucsd.edu
 * Date: 11/26/2019
 * Sources: None
 * This file is meant to exist as a container for starter code that you can use
 * to read the input file format defined in imdb_2019.tsv. Feel free to modify
 * any/all aspects as you wish.
 */

#include <fstream>
#include <iostream>
#include <sstream>

#include "ActorGraph.hpp"

using namespace std;

/**
 * Constructor of the Actor graph
 */
ActorGraph::ActorGraph() {}

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
bool ActorGraph::loadFromFile(const char* in_filename,
                              bool use_weighted_edges) {
    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;

    // keep reading lines until the end of file is reached
    while (infile) {
        string s;

        // get the next line
        if (!getline(infile, s)) break;
        // cout << "READ: " << s << endl;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss(s);
        vector<string> record;

        while (ss) {
            string str;

            // get the next string before hitting a tab character and put it in
            // 'str'
            if (!getline(ss, str, '\t')) break;
            record.push_back(str);
        }

        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor(record[0]);
        string movie_title(record[1]);
        int year = stoi(record[2]);
        Movie movie = {movie_title, year};

        // Builds graph up
        if (movieList.find(movie) == movieList.end()) {
            // if movie not in list
            vector<Node*> cast;
            // cout << "Cannot find movie" << endl;
            if (actorList.find(actor) == actorList.end()) {
                // actor not in list
                // cout << "Cannot find actor" << endl;
                Node* currActor = new Node(actor);
                cast.push_back(currActor);
                actorList[actor] = currActor;
            } else {
                // actor is in list
                // cout << "Found actor" << endl;
                cast.push_back(actorList.find(actor)->second);
            }
            movieList.insert({movie, cast});
        } else {
            // movie is in list
            vector<Node*> cast = movieList.find(movie)->second;
            Node* currActor;
            // cout << "found movie" << endl;
            if (actorList.find(actor) == actorList.end()) {
                // actor not in list
                currActor = new Node(actor);
                actorList[actor] = currActor;
                // cout << "Cannot find actor" << endl;
            } else {
                // actor is in list
                currActor = actorList.find(actor)->second;
                // cout << "found actor" << endl;
            }

            // cout << "Prints actorlist: " << endl;
            // for (auto iter = actorList.begin(); iter != actorList.end();
            //      iter++) {
            //     cout << iter->first << " " << iter->second->getName() <<
            //     "end"
            //          << endl;
            // }

            // cout << "Print actors:" << endl;
            for (int actor = 0; actor < cast.size(); actor++) {
                // cout << cast[actor]->getName() << endl;
                // Adds each actor in the movie as an edge to current actor
                Edge* edgeFromCurr = new Edge(currActor, cast[actor], movie);
                // Adds edge from current to other
                currActor->addEdge(edgeFromCurr);
                Edge* edgeToCurr = new Edge(cast[actor], currActor, movie);
                // Adds edge from other to curent
                cast[actor]->addEdge(edgeToCurr);
            }
            // cout << "end print" << endl;
            cast.push_back(currActor);
        }
    }
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    return true;
}

/**
 * Returns the node of the actor given their name
 * Params:
 * - actorName: the name of the actor
 * Returns: a pointer to the actor's Node, or nullptr if actor not in list
 */
Node* ActorGraph::getActorNode(string actorName) {
    if (actorList.find(actorName) == actorList.end()) {
        return nullptr;
    }
    return actorList.find(actorName)->second;
}

/**
 * Returns the list of actors of a particular movie given its name
 * Params:
 * - movieName: the name of the movie
 * Returns: a vector containing all the actors in the movie, or empty vector
 * if movie does not exist
 */
vector<Node*> ActorGraph::getMovieCast(Movie movie) {
    if (movieList.find(movie) == movieList.end()) {
        cout << "Not in list" << endl;
        return {};
    }
    return movieList.find(movie)->second;
}
