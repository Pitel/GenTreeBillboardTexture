#include "param.h"
#include "vis.h"
#include "grammar.h"

#ifndef GENTREE_H
#define GENTREE_H

struct branch {
	enum branchType type;
	short angles[2];
	size_t length;
};

void GenTreeBillboardTexture(char *data, size_t width, size_t height, unsigned int seed);

#endif
