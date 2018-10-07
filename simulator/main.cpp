#include <iostream>

#include "eval.h"
#include "simulator.h"

using namespace std;

int main() {


   cout << "NUM_ITER is " << NUM_ITER << endl;

    cout << "INPUT :provide infectionList" << endl;
    string fileNameList;
    cin >> fileNameList;

    cout << "INPUT : provide weighted predatorGraph" << endl;
    string fileNamePred;
    cin >> fileNamePred;

    try {
        simulator simul(fileNamePred, fileNameList);

        string fileNameStrat;
        do {

            cout << "INPUT : provide strategy for evaluation, or write <end> or <write to file>" << endl;
            cin >> fileNameStrat;
            cout << "you wrote " << fileNameStrat << endl;


            if (fileNameStrat != "end") {
                
                // enables writing saved to file
                if (fileNameStrat == "write"){
                    string saveFile;
                    int numEval;
                    cout << "saving to file" << endl;
                    cout << "INPUT : please provide save file" << endl;
                    cin >> saveFile;
                    cout << "INPUT : please provide num eval" << endl;
                    cin >> numEval;
                    simul.writeSavedEvalLast(numEval, saveFile);
                    cout << "done writing" << endl;

                } else {
                    cout << "evaluating strategy" << endl;
                    double avg = simul.evalStrat(fileNameStrat);
                    cout << "OUTPUT : average value is :" << avg << endl;
                }
            }

        } while (fileNameStrat != "end");


    } catch (int e){
        if (e == 10){
            cout << "problem reading file, aborting..." << endl;
            return 0;
        }
    }


    return 0;
}
