#include <stdio.h>
#include <stdlib.h>
#include "rules.h"

// T - TRUNK , B - BRANCH ....
const char* printableRules[] = {"T","B","B_U","B_D", "T_NB" };

GRules::GRules(int seed)
{
    // Loading random seed
    
    this->seed = seed;

    // Loading default rules

    vector<int> rule1;
    // TRUNK -> BRANCH_UP, BRANCH, TRUNK, BRANCH_UP
    rule1.push_back(TRUNK);
    rule1.push_back(4);
    rule1.push_back(BRANCH_UP);
    rule1.push_back(BRANCH);
    rule1.push_back(TRUNK);
    rule1.push_back(BRANCH_UP);
    
    vector<int> rule2;
    // TRUNK -> TRUNK, BRANCH
    rule2.push_back(TRUNK);
    rule2.push_back(5);
    rule2.push_back(TRUNK);
    rule2.push_back(BRANCH_UP);
    rule2.push_back(BRANCH);
    rule2.push_back(BRANCH_UP);
    rule2.push_back(BRANCH_UP);
    
    vector<int> rule3;
    // BRANCH -> BRANCH, TRUNK, BRANCH
    rule3.push_back(BRANCH_UP);
    rule3.push_back(3);
    rule3.push_back(BRANCH);
    rule3.push_back(TRUNK);
    rule3.push_back(BRANCH);
    
    vector<int> rule4;
    // BRANCH -> TRUNK, BRANCH_UP, BRANCH_UP,BRANCH_UP, BRANCH_UP
    rule4.push_back(BRANCH);
    rule4.push_back(5);
    rule4.push_back(TRUNK);
    rule4.push_back(BRANCH_UP);
    rule4.push_back(BRANCH_UP);
    rule4.push_back(BRANCH_UP);
    rule4.push_back(BRANCH_UP);
    
    vector<int> rule5;
    // BRANCH_UP -> TRUNK, BRANCH_UP, BRANCH_UP,BRANCH, BRANCH_UP
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
    
    
    this->rules.insert(pair<string, vector<int> >("tree1", rule1));
    this->rules.insert(pair<string, vector<int> >("tree1", rule2));
    this->rules.insert(pair<string, vector<int> >("tree1", rule3));
    this->rules.insert(pair<string, vector<int> >("tree1", rule4));
    this->rules.insert(pair<string, vector<int> >("tree1", rule5));
    this->rules.insert(pair<string, vector<int> >("tree1", rule6));
    this->rules.insert(pair<string, vector<int> >("tree1", rule7));

	//smrk
    vector<int> rule_p1;
    rule_p1.push_back(TRUNK);
    rule_p1.push_back(6);
    rule_p1.push_back(TRUNK_BRANCHLESS);
    rule_p1.push_back(BRANCH);
    rule_p1.push_back(BRANCH);
    rule_p1.push_back(BRANCH);
    rule_p1.push_back(BRANCH);
    rule_p1.push_back(BRANCH);

    vector<int> rule_p2;
    rule_p2.push_back(TRUNK_BRANCHLESS);
    rule_p2.push_back(1);
    rule_p2.push_back(TRUNK);

    vector<int> rule_p3;
    rule_p3.push_back(TRUNK);
    rule_p3.push_back(9);
    rule_p3.push_back(TRUNK_BRANCHLESS);
    rule_p3.push_back(BRANCH);
    rule_p3.push_back(BRANCH);
    rule_p3.push_back(BRANCH);
    rule_p3.push_back(BRANCH);
    rule_p3.push_back(BRANCH);
    rule_p3.push_back(BRANCH);
    rule_p3.push_back(BRANCH);
    rule_p3.push_back(BRANCH);

    vector<int> rule_p4;
    rule_p4.push_back(BRANCH);
    rule_p4.push_back(4);
    rule_p4.push_back(BRANCH);
    rule_p4.push_back(BRANCH_DOWN);
    rule_p4.push_back(BRANCH_DOWN);
    rule_p4.push_back(BRANCH_DOWN);

    vector<int> rule_p5;
    rule_p5.push_back(BRANCH);
    rule_p5.push_back(5);
    rule_p5.push_back(BRANCH);
    rule_p5.push_back(BRANCH_DOWN);
    rule_p5.push_back(BRANCH_DOWN);
    rule_p5.push_back(BRANCH_DOWN);
    rule_p5.push_back(BRANCH_DOWN);

    this->rules.insert(pair<string, vector<int> >("picea", rule_p1));
    this->rules.insert(pair<string, vector<int> >("picea", rule_p2));
    this->rules.insert(pair<string, vector<int> >("picea", rule_p3));
    this->rules.insert(pair<string, vector<int> >("picea", rule_p4));
    this->rules.insert(pair<string, vector<int> >("picea", rule_p5));
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
