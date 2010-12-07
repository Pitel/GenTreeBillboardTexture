#include <cstdio>
#include "GenTreeBillboardTexture.h"

void GenTreeBillboardTexture(SDL_Surface *data, size_t width, size_t height, unsigned int seed, unsigned int depth, SDL_Color wood, SDL_Color leafs) {
	TreeNode* tree = GenTreeBillboardTexture_grammar("picea", depth, seed);  // args: name, iteration, seed
	//tree->printTree();
	
	GenTreeBillboardTexture_parametrize(tree, PTREE_PICEA, seed);
	//tree->printTreeWithParams();
	
	GenTreeBillboardTexture_visualize(data, width, height, tree, wood, leafs);
	return;
}
