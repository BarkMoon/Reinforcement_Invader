#pragma once
#include <fstream>
#include <vector>
#include <deque>
#include <map>
#include <string>
#include <random>
#include "Environment.h"

class Agent{
private:
	std::map<State, Action> target_policy;
	std::map<State, Action> behavior_policy;
	std::map<StateAction, Value> Q;
	std::map<StateAction, Value> C;
	int frame;
public:
	void InitAgent();
	int OffPolicyEveryVisitWeighted(State& init_state, int rep, double Gamma, double epsilon, bool mu_improve);
	Action TargetPolicy(State& state){ return (target_policy.find(state) != target_policy.end()) ? target_policy[state] : 0; }
	int SavePolicy();
	int LoadPolicy(std::string filename);
};