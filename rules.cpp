#include <stdio.h>
#include <stdlib.h>
#include "rules.h"

// T - TRUNK , B - BRANCH ....
const char* printableRules[] = {"T","B","B_U","B_D" };

GRules::GRules(int seed)
{
    // Loading random seed
    
    this->seed = seed;

    // Loading default rules

    //
    //  type : JABLON
    //

    vector<int> rule1;
    rule1.push_back(TRUNK);
    rule1.push_back(4);
    rule1.push_back(BRANCH);
    rule1.push_back(BRANCH_UP);
    rule1.push_back(TRUNK);
    rule1.push_back(BRANCH_UP);
    
    vector<int> rule2;
    rule2.push_back(TRUNK);
    rule2.push_back(5);
    rule2.push_back(TRUNK);
    rule2.push_back(BRANCH_UP);
    rule2.push_back(BRANCH);
    rule2.push_back(BRANCH_UP);
    rule2.push_back(BRANCH_UP);
    
    vector<int> rule3;
    rule3.push_back(BRANCH_UP);
    rule3.push_back(3);
    rule3.push_back(BRANCH);
    rule3.push_back(TRUNK);
    rule3.push_back(BRANCH);
    
    vector<int> rule4;
    rule4.push_back(BRANCH);
    rule4.push_back(5);
    rule4.push_back(TRUNK);
    rule4.push_back(BRANCH_UP);
    rule4.push_back(BRANCH_UP);
    rule4.push_back(BRANCH_UP);
    rule4.push_back(BRANCH_UP);
    
    vector<int> rule5;
    rule5.push_back(BRANCH_UP);
    rule5.push_back(5);
    rule5.push_back(TRUNK);
    rule5.push_back(BRANCH_UP);
    rule5.push_back(BRANCH_UP);
    rule5.push_back(BRANCH);
    rule5.push_back(BRANCH_UP);
    
    vector<int> rule6;
    rule6.push_back(TRUNK);
    rule6.push_back(4);
    rule6.push_back(TRUNK);
    rule6.push_back(BRANCH_UP);
    rule6.push_back(BRANCH_UP);
    rule6.push_back(BRANCH);
    
    vector<int> rule7;
    rule7.push_back(BRANCH);
    rule7.push_back(5);
    rule7.push_back(TRUNK);
    rule7.push_back(BRANCH_UP);
    rule7.push_back(BRANCH_UP);
    rule7.push_back(BRANCH);
    rule7.push_back(BRANCH_UP);
    
    //
    //  type : SMRK
    //
    
    vector<int> rule8;
    rule8.push_back(TRUNK);
    rule8.push_back(6);
    rule8.push_back(BRANCH_UP);
    rule8.push_back(BRANCH_UP);
    rule8.push_back(BRANCH_UP);
    rule8.push_back(BRANCH_UP);
    rule8.push_back(BRANCH_UP);
    rule8.push_back(TRUNK);
    
    vector<int> rule9;
    rule9.push_back(BRANCH_UP);
    rule9.push_back(5);
    rule9.push_back(BRANCH_UP);
    rule9.push_back(BRANCH_UP);
    rule9.push_back(BRANCH_UP);
    rule9.push_back(BRANCH_UP);
    rule9.push_back(TRUNK);
    
    
    
    this->rules.insert(pair<string, vector<int> >("jablon", rule1));
    this->rules.insert(pair<string, vector<int> >("jablon", rule2));
    this->rules.insert(pair<string, vector<int> >("jablon", rule3));
    this->rules.insert(pair<string, vector<int> >("jablon", rule4));
    this->rules.insert(pair<string, vector<int> >("jablon", rule5));
    this->rules.insert(pair<string, vector<int> >("jablon", rule6));
    this->rules.insert(pair<string, vector<int> >("jablon", rule7));
    
    this->rules.insert(pair<string, vector<int> >("smrk", rule8));
    this->rules.insert(pair<string, vector<int> >("smrk", rule9));
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
