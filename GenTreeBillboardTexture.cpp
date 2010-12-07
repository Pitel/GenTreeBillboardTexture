#include <cstdio>
#include "GenTreeBillboardTexture.h"

void GenTreeBillboardTexture(SDL_Surface *data, size_t width, size_t height, unsigned int seed, unsigned int depth, string typeTree, SDL_Color wood, SDL_Color leafs) {
	TreeNode* tree = GenTreeBillboardTexture_grammar(typeTree, depth, seed);  // args: name, iteration, seed
	//tree->printTree();
	
	GenTreeBillboardTexture_parametrize(tree, PTREE_1, seed);
	//tree->printTreeWithParams();
	
	GenTreeBillboardTexture_visualize(data, width, height, tree, wood, leafs);
	return;
}
