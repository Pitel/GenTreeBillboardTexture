#include<iostream>
#include<map>
#include<vector>
#include<string>


#ifndef GENTREE_RULES_H
#define GENTREE_RULES_H

using namespace std;

//PTREE_COUNT znaci pocet typu stromu, tedy cislovat postupne a PTREE_COUNT nechat posledni
enum PTreeType {PTREE_APPLE, PTREE_PICEA, PTREE_PALMA, PTREE_COUNT};

enum treeNodeType {
    TRUNK,
    BRANCH,
    BRANCH_UP,
    BRANCH_DOWN,
	  TRUNK_BRANCHLESS
};

extern const char* printableRules[];

class GRules
{
      private:
      
        int seed;
        multimap<PTreeType, vector<int> > rules;
        
      public: 
      
        GRules(int);
        void AddRule(PTreeType, vector<int>);
        int GetRule(PTreeType , treeNodeType, vector<int>*);
};

#endif
