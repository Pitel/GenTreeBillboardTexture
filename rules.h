#include<iostream>
#include<map>
#include<vector>
#include<string>


#ifndef GENTREE_RULES_H
#define GENTREE_RULES_H

using namespace std;

/**
 *  Vyctovy typ reprezentujici dostupne modely stromu
 *  (gramatiky a parametry stromu) 
 */ 
enum PTreeType {PTREE_APPLE, PTREE_PICEA, PTREE_PALMA, PTREE_COUNT};

/**
 *  Vyctovy typ reprezentujici casti stromu
 *  (gramatika pracuje prave s temito castmi) 
 */ 
enum treeNodeType {
    TRUNK,
    BRANCH,
    BRANCH_UP,
    BRANCH_DOWN,
	  TRUNK_BRANCHLESS
};

/**
 *  Pomocne pole obsahujici zkratky pro ladici vypis jednotlivych pravidel
 */   
extern const char* printableRules[];

/**
 *  Trida starajici se o generovani gramatiky pro strom urciteho typu
 */  
class GRules
{
      private:
      
        int seed;
        multimap<PTreeType, vector<int> > rules;
        
      public: 
      
        GRules(int);
        
        /**
         *  Vstupem funkce je typ stromu, pravidlo, ktere je potreba rozgenerovat
         *  a vector integeru, do ktereho se nakopiruji vybrana rozgenerovana pravidla         
         */                          
        int GetRule(PTreeType , treeNodeType, vector<int>*);
};

#endif
