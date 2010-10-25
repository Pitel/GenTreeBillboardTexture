#include <stdio.h>
#include <time.h>
#include "GenTreeBillboardTexture.h"

void GenTreeBillboardTexture(/*char *data, int width, int height, float param*/) {
	//time_t cas;
  int seed = 23505; // time(&cas);
  TreeNode* tree = GenTreeBillboardTexture_grammar("tree1",40,seed);  // args : name, iteration , seed
  tree->printTree();
	
	
	GenTreeBillboardTexture_parametrize();
	GenTreeBillboardTexture_visualize();
	
	return;
}
