#include <cstdio>
#include "GenTreeBillboardTexture.h"

/** Generování impostoru stromu.
 * Vygeneruje texturu stromu.
 * @param data Ukazatel na SDLSurface
 * @param seed Seed pro generování náhodných čísel
 * @param depth Hloubka L-systému
 * @param typeTree Typ stromu
 * @param wood Barva dřeva
 * @param leafs Barva listů
 */
void GenTreeBillboardTexture(SDL_Surface *data, unsigned int seed, unsigned int depth, PTreeType typeTree, SDL_Color wood, SDL_Color leafs) {
	TreeNode* tree = GenTreeBillboardTexture_grammar(typeTree, depth, seed);  // args: name, iteration, seed
	//tree->printTree();
	
	GenTreeBillboardTexture_parametrize(tree, typeTree, seed);
	//tree->printTreeWithParams();
	
	GenTreeBillboardTexture_visualize(data, tree, wood, leafs);
	return;
}
