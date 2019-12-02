#ifndef MOVIE_HPP
#define MOVIE_HPP

#include <string>

using namespace std;

/**
 * This struct keeps track of specific movies given their titles and year
 */
typedef struct Movie {
    string title;  // Title includes year
    string name;   // Name of movie
    int year;      // Year of movie

    bool operator==(const Movie& other) { return title == other.title; }
    bool operator!=(const Movie& other) { return title != other.title; }

    bool operator<(const Movie& other) const { return title < other.title; }
} Movie;

#endif