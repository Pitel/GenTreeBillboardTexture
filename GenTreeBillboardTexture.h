#include "param.h"
#include "vis.h"
#include "grammar.h"
#include <SDL.h>

#ifndef GENTREE_H
#define GENTREE_H

/** Generování impostoru stromu.
 * Vygeneruje texturu stromu.
 * @param data Ukazatel na SDLSurface
 * @param seed Seed pro generování náhodných čísel
 * @param depth Hloubka L-systému
 * @param typeTree Typ stromu
 * @param wood Barva dřeva
 * @param leafs Barva listů
 */
void GenTreeBillboardTexture(SDL_Surface *data, unsigned int seed, unsigned int depth, PTreeType typeTree, SDL_Color wood, SDL_Color leafs);

#endif
