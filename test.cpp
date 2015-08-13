// Test sensor network files
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "SensorNetwork.h"

// LearningType {QLEARNING, SARSA, SARSALAMBDA} ;

int main()
{
	// Default is 2 targets, 3 energy, 3 cells, full state observability (i.e. can observe all cells on either side)
	SensorNetwork * testNetwork = new SensorNetwork(3,10,4,1) ;
	LearningType algorithm = SARSALAMBDA ;
	testNetwork->SetLearningAlgorithm(algorithm) ;
	
	vector< vector<int> > allStates = testNetwork->GetAllStates() ;
	
	// Write states to file
	ofstream statesFile ;
	statesFile.open("states.txt") ;
	for (unsigned i = 0; i < allStates.size(); i++)
	{
		for (unsigned j = 0; j < allStates[i].size(); j++)
		{
			statesFile << allStates[i][j] ;
			if (j == allStates[i].size()-1)
				statesFile << "\n" ;
			else
				statesFile << "," ;
		}
	}
	statesFile.close() ;
	
	std::string logFileName = "log.txt" ;
	ofstream logFile ;
	
	unsigned k = 0 ;
	unsigned nEps = 10 ;
	while (k < nEps){
		logFile.open(logFileName,ios_base::app) ;
		logFile << "Episode " << k << endl ;
		logFile.close() ;
		
		cout << "Episode " << k << "..." ;
		
		// Iterate for 1000 steps or until all targets are eliminated
		unsigned i = 0 ;
		while (i < 1000){
			testNetwork->Iterate(logFileName) ;
			if (testNetwork->GetStateID() == 0)
				break ;
			i++ ;
		}
		
		cout << i << " steps.\n" ;
		
		testNetwork->ResetTargets() ;
		k++ ;
	}
	
	return 0 ;
}
