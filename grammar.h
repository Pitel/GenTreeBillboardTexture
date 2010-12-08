#include<vector>
#include<queue>
#include<iostream>
#include "rules.h"
#include "param.h"
#include <fstream>

#ifndef GENTREE_GRAMMAR_H
#define GENTREE_GRAMMAR_H

//
//  Data types
//
typedef struct tNode {
    struct tNode* parentNode;
    treeNodeType type;
    vector<struct tNode*> childNodes;
    paramInfo param;
} treeNode;

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

        void printTree();
        void printTreeWithParams();
        
		static void processPrint();
		static void processPrintWithParams();
};

//
//  Functions
//
TreeNode* GenTreeBillboardTexture_grammar(PTreeType /* Tree type */, int /*depth*/, int /*seed*/);

#endif
