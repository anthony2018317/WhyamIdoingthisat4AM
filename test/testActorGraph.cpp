#include <iostream>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "ActorGraph.hpp"

using namespace std;
using namespace testing;

class SimpleActorGraph : public ::testing::Test {
  protected:
    ActorGraph graph;

  public:
    SimpleActorGraph() {
        // initialization code here
        graph.loadFromFile("./../data/imdb_2019.tsv", false);
    }
};

TEST_F(SimpleActorGraph, TEST_GET_ACTOR) {
    Node* actor = graph.getActorNode("Robert Downey Jr.");
    ASSERT_EQ(actor->getName(), "Robert Downey Jr.");
}

TEST_F(SimpleActorGraph, TEST_GET_ACTOR_NOT_IN_LIST) {
    Node* actor = graph.getActorNode("Grant Jiang");
    ASSERT_EQ(actor, nullptr);
}

TEST_F(SimpleActorGraph, TEST_GET_CAST) {
    Movie movie = {"Avengers: Age of Ultron", 2015};
    vector<Node*> actors = graph.getMovieCast(movie);
    bool firstFound = false;
    bool secondFound = false;
    for (int i = 0; i < actors.size(); i++) {
        if (actors[i]->getName() == "Robert Downey Jr.") {
            firstFound = true;
        }
        if (actors[i]->getName() == "Chris Evans") {
            secondFound = true;
        }
    }
    ASSERT_EQ(firstFound, true);
    ASSERT_EQ(secondFound, true);
}

TEST_F(SimpleActorGraph, TEST_GET_CAST_NOT_IN_LIST) {
    Movie movie = {"Doesn't exist", 2015};

    ASSERT_EQ(graph.getMovieCast(movie).size(), 0);
}
