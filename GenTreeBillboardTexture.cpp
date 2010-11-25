#include <cstdio>
#include "GenTreeBillboardTexture.h"

void GenTreeBillboardTexture(char *data, size_t width, size_t height, unsigned int seed, unsigned int depth) {
	TreeNode* tree = GenTreeBillboardTexture_grammar("tree1", depth, seed);  // args: name, iteration, seed
	tree->printTree();
	
	GenTreeBillboardTexture_parametrize(tree, PTREE_1, seed);
	tree->printTreeWithParams();
	tree->printTreeGL();
	
	GenTreeBillboardTexture_visualize(data, width, height, tree);
	return;
}
