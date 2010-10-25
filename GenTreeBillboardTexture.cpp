#include <stdio.h>
#include <time.h>
#include "GenTreeBillboardTexture.h"

void GenTreeBillboardTexture(char *data, int width, int height, unsigned int seed) {
	TreeNode* tree = GenTreeBillboardTexture_grammar("tree1", 40, seed);  // args: name, iteration, seed
 	tree->printTree();
	
	GenTreeBillboardTexture_parametrize();
	GenTreeBillboardTexture_visualize();
	
	return;
}
