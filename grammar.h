#include<vector>
#include<queue>
#include<iostream>
#include "rules.h"
#include "param.h"
#include <fstream>

#ifndef GENTREE_GRAMMAR_H
#define GENTREE_GRAMMAR_H

/**
 *  Trida reprezentujici uzel stromu v grafove reprezentaci 
 */ 
class TreeNode
{
    public: 
        
      static queue<TreeNode*> printNodes;
        
      TreeNode();
      TreeNode(treeNodeType,TreeNode*);
      treeNodeType type;
      TreeNode* parentNode;
      vector<TreeNode*> childNodes;
		  paramInfo param;

    /**
     *  Pomocna funkce pro ladeni vystupu.
     *  Funkce prida aktualni strom do fronty a spusti generovani vetvi.
     *  (uspornejsi vypis)  
     */ 
    void printTree();
        
    /**
     *  Pomocna funkce pro ladeni vystupu.
     *  Funkce prida aktualni strom do fronty a spusti generovani vetvi.
     *  (rozsireny vypis)  
     */     
    void printTreeWithParams();
     
    /**
     *  Volana funkci printTree(). Obstarava samotny vypis vsech
     *  uzlu rozgenerovaneho strom
     *  (usporny vypis)  
     */     
		static void processPrint();
		
		/**
     *  Volana funkci printTreeWithParams(). Obstarava samotny vypis vsech
     *  uzlu rozgenerovaneho strom.
     *  (rozsireny vypis)  
     */
		static void processPrintWithParams();
};


/**
 *  Vstupem funkce je typ stromu, uroven generovani (detail), a nahodne cislo
 *  Funkce vraci ukazatel na vygenerovany strom z uzlu TreeNode
 */ 
TreeNode* GenTreeBillboardTexture_grammar(PTreeType /* Tree type */, int /*depth*/, int /*seed*/);

#endif
