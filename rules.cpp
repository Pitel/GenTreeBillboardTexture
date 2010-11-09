#include <stdio.h>
#include <stdlib.h>
#include "rules.h"

// T - TRUNK , B - BRANCH ....
const char* printableRules[] = {"T","B","B_U","B_D","L" };

GRules::GRules(int seed)
{
    // Loading random seed
    
    this->seed = seed;

    // Loading default rules

    vector<int> rule1;
    // TRUNK -> BRANCH, BRANCH_UP
    rule1.push_back(TRUNK);
    rule1.push_back(2);
    rule1.push_back(BRANCH);
    rule1.push_back(BRANCH_UP);
    
    vector<int> rule2;
    // TRUNK -> BRANCH, BRANCH_UP, BRANCH
    rule2.push_back(TRUNK);
    rule2.push_back(3);
    rule2.push_back(BRANCH_UP);
    rule2.push_back(TRUNK);
    rule2.push_back(BRANCH);
    
    vector<int> rule3;
    // BRANCH -> BRANCH, BRANCH_UP, BRANCH
    rule3.push_back(BRANCH);
    rule3.push_back(3);
    rule3.push_back(BRANCH);
    rule3.push_back(BRANCH_UP);
    rule3.push_back(BRANCH);
    
    vector<int> rule4;
    // BRANCH -> BRANCH, BRANCH
    rule4.push_back(BRANCH);
    rule4.push_back(2);
    rule4.push_back(BRANCH);
    rule4.push_back(BRANCH);
    
    vector<int> rule5;
    // BRANCH -> BRANCH_DOWN
    rule5.push_back(BRANCH);
    rule5.push_back(1);
    rule5.push_back(BRANCH_DOWN);
    
    vector<int> rule6;
    // BRANCH -> BRANCH_DOWN
    rule6.push_back(BRANCH);
    rule6.push_back(1);
    rule6.push_back(LEAVES);
    
    this->rules.insert(pair<string, vector<int> >("tree1", rule1));
    this->rules.insert(pair<string, vector<int> >("tree1", rule2));
    this->rules.insert(pair<string, vector<int> >("tree1", rule3));
    this->rules.insert(pair<string, vector<int> >("tree1", rule4));
    this->rules.insert(pair<string, vector<int> >("tree1", rule5));
    this->rules.insert(pair<string, vector<int> >("tree1", rule6));
}

void GRules::AddRule(string name, vector<int> rule)
{
     // Modularity support
     
     this->rules.insert(pair<string, vector<int> >(name, rule));
}

int GRules::GetRule(string name, treeNodeType symbol, vector<int> *data)
{
    pair<multimap<string, vector<int> >::iterator,multimap<string, vector<int> >::iterator> ret;
    multimap<string, vector<int> >::iterator it;

    srand(this->seed);
    
    vector<vector<int> > temp;

    ret = this->rules.equal_range(name);
    for (it=ret.first; it!=ret.second; ++it)
    {
       if((*it).second[0] == symbol) 
          temp.push_back((*it).second);
    }  
    if(temp.size()>0)
    {
      (*data) = temp[rand() % temp.size()];
      return 1;
    }
    else
      return 0;
}