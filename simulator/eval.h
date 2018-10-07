#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

#include <boost/unordered_map.hpp>
#include <stdlib.h>     /* srand, rand */

#include "../constants.h"

using namespace std;


// TIMING DEFINITIONS
// 1. spread of prey
// 2. spread of predator
// 3. placement of predator
// 4. counting number of saved instances



class eval{

public:

    eval(string fileNameInfected, string fileNamePred, unsigned long seed); // owns everything itself
    // throws away weighted graph after use
    eval(string fileNameInfected, boost::unordered_map<string, float> predGraphWeighted, unsigned long seed);

    void simulate();
    void readStrat(string fileName);
    void readStratVec(vector<vector<int> > &strat);
    void writeSaved(string fileName);
    int numSaved();

    // data structures for graph
    boost::unordered_map<string, int> whenInfect; // maps node -> when infected
    boost::unordered_map<string, int> whenProtect; // maps node -> earliest protected
    boost::unordered_map<string, vector<int> > predGraphBinary; // maps edge -> list of active timesteps
    boost::unordered_map<string, vector<string> > neighborsInBinary; // maps node -> list of neigbour nodes in binaryGraph

private:

    unsigned long seed;

    void oneStep(); // logic for simulation
    // called by constructors, requires predGraphWeighted to be constructed
    void getData(string fileNameInfected);

    int numIter; //serves as "null" time step
    int currStep; // records current Step in simulation

    // data structures for the graph
    boost::unordered_map<string, float> predGraphWeighted; // maps edge -> weight
    vector<vector<string> > toProtect; // store all nodes that are to become protected

    // funcs for creating data structs
    void getBinary(); //fills binary graph from weighted one
    void readInfected(string fileName, boost::unordered_map<string, int> &hashtable);
    void readPredGraph(string fileName, boost::unordered_map<string, float> &hashtable);



};
