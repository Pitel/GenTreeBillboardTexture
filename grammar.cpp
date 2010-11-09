#include <stdio.h>
#include <queue>
#include "grammar.h"

queue<TreeNode*> TreeNode::printNodes;

TreeNode::TreeNode(treeNodeType type,TreeNode* parent)
{
      this->type = type;
      parentNode = parent;
}

void TreeNode::printTree()
{
    TreeNode::printNodes.push(this);
    TreeNode::processPrint();
}
void TreeNode::printTreeWithParams()
{
	TreeNode::printNodes.push(this);
	TreeNode::processPrintWithParams();
}


void TreeNode::processPrint()
{
    int size = TreeNode::printNodes.size();
    for(int i = 0;i < size;i++)
    {
        TreeNode* node = TreeNode::printNodes.front();
        TreeNode::printNodes.pop();
        std::cout << printableRules[node->type] << "(childs: " << node->childNodes.size() << ")  ";

        for(unsigned int i=0;i<node->childNodes.size();i++)
        {
             TreeNode::printNodes.push(node->childNodes[i]);
        }
        
    }
    
    if(size != 0)
    {
      std::cout << endl << "==============" << endl;
      processPrint();
    }
}

void TreeNode::processPrintWithParams()
{
	int size = TreeNode::printNodes.size();
	for(int i = 0;i < size;i++)
	{
		TreeNode* node = TreeNode::printNodes.front();
		TreeNode::printNodes.pop();
		std::cout << printableRules[node->type] << "[" << (std::string)(node->param) << "]" << "(childs: " << node->childNodes.size() << ")  ";

		for(unsigned int i=0;i<node->childNodes.size();i++)
		{
			 TreeNode::printNodes.push(node->childNodes[i]);
		}

	}

	if(size != 0)
	{
	  std::cout << endl << "==============" << endl;
	  processPrintWithParams();
	}
}


TreeNode* GenTreeBillboardTexture_grammar(string treeType, int depth ,int seed) {

   int currentDepth = 0;
   queue<TreeNode*> processingNodes;
   TreeNode* treeStructure = new TreeNode(TRUNK,NULL);
   
   processingNodes.push(treeStructure);
  
   while(currentDepth!=depth && !processingNodes.empty())
   {
        currentDepth++;
        TreeNode* currentNode = processingNodes.front();
        processingNodes.pop();
        GRules grammar(seed);
        vector<int> rule; 
        if(grammar.GetRule(treeType,currentNode->type,&rule))
        {
             for(int i = 2;i < 2 + rule[1]; i++)
             {
                 TreeNode* node = new TreeNode((treeNodeType)rule[i],currentNode);
                 currentNode->childNodes.push_back(node);
                 processingNodes.push(node);
             }
        }
        else
            continue;
   }
   
  return treeStructure;
}
