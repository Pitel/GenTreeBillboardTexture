#include<iostream>
#include<map>
#include<vector>
#include<string>

#ifndef GENTREE_RULES_H
#define GENTREE_RULES_H

using namespace std;

enum treeNodeType {
    TRUNK,
    BRANCH,
    BRANCH_UP,
    BRANCH_DOWN,
    LEAVES
};

extern const char* printableRules[];

class GRules
{
      private:
      
        int seed;
        multimap<string, vector<int> > rules;
        
      public: 
      
        GRules(int);
        void AddRule(string, vector<int>);
        int GetRule(string , treeNodeType, vector<int>*);
};

#endif
