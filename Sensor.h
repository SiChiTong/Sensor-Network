#pragma once
#include <iostream>
#include <stdlib.h>
#include "Policy.h"

enum SensorAction {TRACKLEFT, NOTRACK, TRACKRIGHT} ; 

class Sensor
{
	public:
		Sensor(int left, int right): leftCell(left), rightCell(right), numActions(3) {
			// Assign random initial action
			int a0 = rand() % numActions ;
			switch (a0) {
				case 0:
					itsAction = TRACKLEFT ;
					break ;
				case 1:
					itsAction = NOTRACK ;
					break ;
				case 2:
					itsAction = TRACKRIGHT ;
					break ;
			}
		}
		
		~Sensor() {}
		
		int GetAction() const {return itsAction ;}
		int GetLeft() const {return leftCell ;}
		int GetRight() const {return rightCell ;}
		
		// Assign learning algorithm
		void SetLearning(LearningType algorithm){
			itsPolicy.SetLearningType(algorithm) ;
		}
		
		// Compute complete local state space given sensor observability range
		void SetRange(int range, vector< vector<int> > allStates){
			itsRange = range ;
			int numCells = allStates[0].size() ;
			int leftLim = max(0, leftCell - itsRange + 1) ;
			int rightLim = min(numCells, rightCell + itsRange - 1) ;
			int nCells = rightLim - leftLim ;
			vector<int> temp ;
			for (unsigned i = 0; i < allStates.size(); i++){
				temp.clear() ;
				for (int j = 0; j <= nCells; j++)
					temp.push_back(allStates[i][j]) ;
				
				bool newState = true ;
				for (unsigned j = 0; j < itsStates.size(); j++){
					if (VectorCompare(temp,itsStates[j]))
						newState = false ;
				}
				if (newState)
					itsStates.push_back(temp) ;
			}
			numStates = itsStates.size() ;
		}
		
		// Initialise decentralised learning algorithm based on local state space
		void InitialisePolicy(){
			itsPolicy.SetQ(numStates, numActions) ;
		}
		
		// Initialise centralised learning algorithm based on global state space
		void InitialisePolicy(int nStates){
			itsPolicy.SetQ(nStates, numActions) ;
		}
		
		// Reset state-action trace to zeros
		void ResetTrace(){
			itsPolicy.SetTrace() ;
		}
		
		void SetReward(int reward) {itsReward = reward ;}
		
		// Choose next action based on policy learned on local state
		void ChooseAction(vector<int> gState0, vector<int> gState1){
			int state0 = ObserveLocalState(gState0) ;
			int state1 = ObserveLocalState(gState1) ;
			int newAction = itsPolicy.NextAction(state0, itsAction, itsReward, state1) ;
			switch (newAction){
				case 0:
					itsAction = TRACKLEFT ;
					break ;
				case 1:
					itsAction = NOTRACK ;
					break ;
				case 2:
					itsAction = TRACKRIGHT ;
					break ;
			}
		}
		
		// Choose next action based on policy learned on global state
		void ChooseAction(int state0, int state1){
			int newAction = itsPolicy.NextAction(state0, itsAction, itsReward, state1) ;
			switch (newAction){
				case 0:
					itsAction = TRACKLEFT ;
					break ;
				case 1:
					itsAction = NOTRACK ;
					break ;
				case 2:
					itsAction = TRACKRIGHT ;
					break ;
			}
		}
		
	private:
		int leftCell ;
		int rightCell ;
		int numActions ;
		SensorAction itsAction ;
		int itsReward ;
		Policy itsPolicy ;
		int itsRange ;
		vector< vector<int> > itsStates ;
		int numStates ;
		
		// Compute local state given global state
		int ObserveLocalState(vector<int> globalState){
			int numCells = globalState.size() ;
			int leftLim = leftCell - itsRange + 1 ;
			int rightLim = rightCell + itsRange - 1 ;
			vector<int> temp ;
			for (int i = leftLim; i <= rightLim; i++){
				if (i >= 0 && i < numCells)
					temp.push_back(globalState[i]) ;
			}
	
			for (unsigned i = 0; i < itsStates.size(); i++)
				if (VectorCompare(temp, itsStates[i]))
					return i ;
	
			cout << "Error: invalid state!\n" ;
			return -1 ;
		}
		
		// Return true if input vectors have identical elements
		bool VectorCompare(vector<int> u, vector<int> v){
			bool eq = true ;
			for (unsigned i = 0; i < u.size(); i++){
				if (u[i] != v[i]){
					eq = false ;
					break ;
				}
			}
			return eq ;
		}
} ;
