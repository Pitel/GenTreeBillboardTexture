#include <stdio.h>
#include "GenTreeBillboardTexture.h"

void GenTreeBillboardTexture(char *data, size_t width, size_t height, unsigned int seed) {
	TreeNode* tree = GenTreeBillboardTexture_grammar("tree1", 40, seed);  // args: name, iteration, seed
 	tree->printTree();
	
	GenTreeBillboardTexture_parametrize(tree, PTREE_1, seed);
	tree->printTreeWithParams();
	boundingBox bb = getBoundingBox(tree);

	GenTreeBillboardTexture_visualize();
	
	return;
}
