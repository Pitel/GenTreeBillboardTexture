#include "param.h"
#include "vis.h"
#include "grammar.h"
#include <SDL.h>

#ifndef GENTREE_H
#define GENTREE_H

void GenTreeBillboardTexture(SDL_Surface *data, size_t width, size_t height, unsigned int seed, unsigned int depth, PTreeType typeTree, SDL_Color wood, SDL_Color leafs);

#endif
