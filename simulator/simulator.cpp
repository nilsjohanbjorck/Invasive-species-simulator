#include "simulator.h"
#include <omp.h>
using namespace std;


double simulator::evalStrat(string fileNameStrat) {
    // reads file and calls other function

    readStrat(fileNameStrat);
    return evalStratVec(stratVec);

}

double simulator::evalStratVec(vector<vector<int> > inputStratVec) {

    int totalSaved = 0;
    double outP = 0;

    stratVec = inputStratVec;

#pragma omp parallel for reduction(+ : totalSaved)
    for (int i =0; i < evals.size(); i++) {
        (*evals[i]).readStratVec(stratVec);
        (*evals[i]).simulate();
        int saved = (*evals[i]).numSaved();
        //cout << "in real simulation " << i << " we save " << saved << endl;
        totalSaved += saved;
    }

    //cout << "we have saved in total " << totalSaved << endl;
    // do averaging
    outP = (double) totalSaved / (double) evals.size();
    return outP;

}


simulator:: simulator(string graphWeightsFile, string infectionsFile){

    // reads the weighted graph
    this->readPredGraph(graphWeightsFile , predGraphWeighted);

    // create evals
    std::ifstream infile(infectionsFile.c_str());
    string fname;

    // set seed
    unsigned long simSeed = 305186928; //random seed found online
    srand(simSeed);
    unsigned long seed;

    int numEvals = 0;

    while (infile >> fname)
    {
        seed = rand();
        // create ptr to new:ed eval object
        boost::shared_ptr<eval> myPtr (new eval(fname, this->predGraphWeighted, seed));
        evals.push_back(myPtr);
        numEvals +=1;

    }

    cout << "created simulator with : " << numEvals << " cascade scenarios" << endl;

}



void simulator::readPredGraph(string fileName, boost::unordered_map<string, float> &hashtable) {
    // file formatted as row,col row,col weight

    std::ifstream infile(fileName.c_str());
    if (!infile.good()) {
        cout << fileName << " Not Good" << endl;
        throw 10;
    }

    string i, j, key;
    float weigth;

    while (infile >> i >> j >> weigth)
    {
        key = i + " " + j;
        hashtable.emplace(key, weigth);
    }

}


void simulator::readStrat(string fileName) {
    // reads strat and put into vector
    // file formatted as row col iter

    stratVec.clear();

    // file formatted as: row col year
    std::ifstream infile(fileName.c_str());
    if (!infile.good()) {
		cout << fileName << " Not Good" << endl;
		throw 10;
	}

    int i, j, time;
    vector<int> oneLine;

    while (infile >> i >> j >> time)
    {
        oneLine.clear();
        oneLine.push_back(i);
        oneLine.push_back(j);
        oneLine.push_back(time);
        stratVec.push_back(oneLine);

    }


}



void simulator::writeSavedEval(vector<vector<int> > inputStratVec, int eval, string fileName){
    // ask eval to write its saved to file
    evalStratVec(inputStratVec);
    (*evals[eval]).writeSaved(fileName);    
}


void simulator::writeSavedEvalLast(int eval, string fileName){
    // ask eval to write its saved to file
    (*evals[eval]).writeSaved(fileName);
}

