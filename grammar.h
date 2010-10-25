#include<vector>
#include<queue>
#include<iostream>
#include "rules.h"
#include "param.h"

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
        
        void printTree();
        
        static void processPrint();
};

//
//  Functions
//
TreeNode* GenTreeBillboardTexture_grammar(string /* Tree type */, int /*depth*/, int /*seed*/);

#endif
