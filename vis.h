#ifndef GENTREE_VISUAL_H
#define GENTREE_VISUAL_H

#include <SDL.h>
#include "grammar.h"
#include "param.h"

void GenTreeBillboardTexture_visualize(SDL_Surface* data, TreeNode* tree, SDL_Color wood, SDL_Color leafs);

#endif
