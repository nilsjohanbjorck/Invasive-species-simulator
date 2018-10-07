#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include <boost/shared_ptr.hpp>
#include "eval.h"

using namespace std;

//TODO
// using seed consistently


class simulator{

public:

    double evalStratVec(vector<vector<int> > inputStratVec);
    double evalStrat(string fileNameStrat);
    simulator(string graphWeightsFile, string infectionsFile);
    void writeSavedEval(vector<vector<int> > inputStratVec, int eval, string fileName);
    void writeSavedEvalLast( int eval, string fileName);

    vector<boost::shared_ptr<eval> > evals; // vector of pointers to evals

private:

    vector<vector<int> > stratVec;
    void readStrat(string fileName);
    boost::unordered_map<string, float> predGraphWeighted; // maps edge -> weight

    void readPredGraph(string fileName, boost::unordered_map<string, float> &hashtable);

};




