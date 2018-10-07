#include <sstream>
#include <algorithm>

#include <boost/lexical_cast.hpp>

#include "eval.h"



void eval::simulate() {

    for (int i = 0; i< numIter; i++){
        oneStep();
    }

}


void eval::oneStep() {

    // don't iterate over the number of iterations
    if (currStep >= numIter){ return; }

    string edgeName, fromName, toName;
    vector<int> activeTimes;
    vector<string> toAdd = toProtect[currStep];
    int newTime, newIndex;

    // iterate over all items to add to protection
    for (int i =0; i < toAdd.size(); i++){

        fromName = toAdd[i];
        // if has already been protected, skip adding it
        if (whenProtect[fromName] < currStep){ continue;}
        // if has no neighbors, continue
        if (neighborsInBinary.find(fromName) == neighborsInBinary.end()) { continue; }

        // add all neighbors
        for (int j = 0; j < neighborsInBinary[fromName].size(); j ++){

            toName = neighborsInBinary[fromName][j];

            // if been protected previously, skip
            if (whenProtect[toName] <= currStep ) { continue; }

            edgeName = fromName + " " + toName;
            activeTimes = predGraphBinary[edgeName];

            // finds the first time after currStep
            vector<int>::iterator upper = upper_bound(activeTimes.begin(), activeTimes.end(), currStep);
            if (upper != activeTimes.end()){

                newIndex = (upper - activeTimes.begin());
                newTime = activeTimes[newIndex];

                // if new time is lower, add the new connection
                if (whenProtect[toName] > newTime ){
                    whenProtect[toName] = newTime;
                    toProtect[newTime].push_back(toName);
                }
            }
        }
    }
    currStep += 1;
}





// contructor with filename
eval::eval(string fileNameInfected, string fileNamePred, unsigned long seed) : numIter(NUM_ITER), seed(seed)  {

    this->readPredGraph(fileNamePred , predGraphWeighted);
    this->getData(fileNameInfected);
}

// constructor with given weighted graph
eval::eval(string fileNameInfected, boost::unordered_map<string, float> predGraphWeighted, unsigned long seed)
        : numIter(NUM_ITER), predGraphWeighted(predGraphWeighted), seed(seed)  {

    this->getData(fileNameInfected);
    this->predGraphWeighted.clear(); // throw away weighted graph
}



void eval::getData(string fileNameInfected) {
    // called by constructors to fill datastructs

    this->readInfected(fileNameInfected, whenInfect);

    // create whenProtect, set to zero
    whenProtect.clear();
    boost::unordered_map<string, int>::iterator it;
    for ( it = whenInfect.begin(); it != whenInfect.end(); ++it ) {
        whenProtect.emplace(it->first, numIter);
    }
    

    // create toProtect
    vector<string> empty;
    for (int i=0; i< numIter; i++){
        toProtect.push_back(empty);
    }

    this->getBinary();

}


void eval::readInfected(string fileName, boost::unordered_map<string, int> &hashtable) {
    // file formatted as row col iter

    std::ifstream infile(fileName.c_str());
    if (!infile.good()) {
        cout << "can't read file: " << fileName << endl;
        throw 10;
    }

    string i, j, key;
    int time;

    while (infile >> i >> j >> time)
    {
        if (time == -1){ time = numIter; }
        key = i + "," + j;
        hashtable.emplace(key, time);
    }
}


void eval::readPredGraph(string fileName, boost::unordered_map<string, float> &hashtable) {
    // file formatted as row,col row,col weight

    std::ifstream infile(fileName.c_str());
    if (!infile.good()) {
        cout << "can't read file: " << fileName << endl;
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


void eval::readStratVec(vector<vector<int> > &strat) {

    currStep = 0; // set iter to 0
    // clear toProtect
    for (int i =0; i < numIter; i++){
        toProtect[i].clear();
    }
    // clear whenProtect
    whenProtect.clear();
    boost::unordered_map<string, int>::iterator it;
    for ( it = whenInfect.begin(); it != whenInfect.end(); ++it ) {
        whenProtect[it->first] = numIter;
    }

    string key;
    int time;
    // read the vector
    for (int i =0; i < strat.size(); i ++){

        key = boost::lexical_cast<std::string>(strat[i][0]) + "," + boost::lexical_cast<std::string>(strat[i][1]);
        time = strat[i][2];

        // only add items that gets infected
        if (whenInfect.find(key) != whenInfect.end()) {
            toProtect[time ].push_back(key);
            whenProtect[key] = min(whenProtect[key], time);
        }
    }

}

void eval::readStrat(string fileName) {
    // read strat from file, also resets simulator
    // file is formatted row col iter

    currStep = 0; // set iter to 0
    // clear toProtect
    for (int i =0; i < numIter; i++){
        toProtect[i].clear();
    }
    // clear whenProtect
    whenProtect.clear();
    boost::unordered_map<string, int>::iterator it;
    for ( it = whenInfect.begin(); it != whenInfect.end(); ++it ) {
        whenProtect[it->first] = numIter;
    }


    // file formatted as: row col year
    std::ifstream infile(fileName.c_str());
    if (!infile.good()) {
        cout << "can't open file: " << fileName << endl;
        throw 10;
    }

    string i, j, key;
    int time;

    while (infile >> i >> j >> time)
    {
        key = i + "," + j;
        // only add items that gets infected
        if (whenInfect.find(key) != whenInfect.end()) {
            toProtect[time ].push_back(key);
            whenProtect[key] = min(whenProtect[key], time);
        }
    }

}



void eval::getBinary() {
    // fills the binary hashtable with edgename -> times active

    srand(seed); //seed rand
    float rnd;
    vector<string> neighbors;
    vector<int> times;
    int startFrom;

    // for every edge in weighted graph
    boost::unordered_map<string, float>::iterator it;
    for (it = predGraphWeighted.begin(); it != predGraphWeighted.end(); it++) {

        // read to and from nodes
        istringstream words;
        string from, to;
        words.str(it->first);
        words >> from;
        words >> to;

        // if both to and from are infected, add edges
        if ( whenInfect.find (to) != whenInfect.end() && whenInfect.find (from) != whenInfect.end() ) {

            times.clear(); // empty container

            // start from when both are infected, check when active
            startFrom = max(whenInfect[from], whenInfect[to]);
            for (int t = startFrom; t < numIter; t++) {
                    rnd = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                    if (rnd < it->second*PRED_VIRALITY) {
                    times.push_back(t);
                }
            }

            // if active at least once, add edge
            if (times.size() > 0) {
                predGraphBinary.emplace(it->first, times);

                // add as neighbour in neighborsInBinary dataStruct
                if (neighborsInBinary.find (from) != neighborsInBinary.end()){
                    neighborsInBinary[from].push_back(to);
                } else {
                    neighbors.clear();
                    neighbors.push_back(to);
                    neighborsInBinary.emplace(from, neighbors);
                }

            }
        }
    }

}

void eval::writeSaved(string fileName) {

    ofstream myfile;
    myfile.open (fileName.c_str());


    boost::unordered_map<string, int>::iterator it;

    for (it = whenProtect.begin(); it != whenProtect.end(); it++) {
        if (it->second != NUM_ITER){
            myfile << it->first << " " << it->second << "\n";
        }
    }

    myfile.close();
}


int eval::numSaved() {

    int saved = 0;
    boost::unordered_map<string, int>::iterator it;

    for (it = whenProtect.begin(); it != whenProtect.end(); it++) {
        saved +=  numIter - it->second;
    }
    return saved;

}
