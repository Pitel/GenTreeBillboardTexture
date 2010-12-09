#ifndef GENTREE_VISUAL_H
#define GENTREE_VISUAL_H

#include <SDL.h>
#include "grammar.h"
#include "param.h"

/** Vizualizace stromu.
 * Rasterizuje strom podle zadaných parametrů.
 * @param data Ukazatel na SDL_Surface
 * @param tree Struktura se stromem
 * @param wood Barva dreva
 * @param leafs Barva listu
 */
void GenTreeBillboardTexture_visualize(SDL_Surface* data, TreeNode* tree, SDL_Color wood, SDL_Color leafs);

#endif
