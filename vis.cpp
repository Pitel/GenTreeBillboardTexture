#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "vis.h"

#define SWAP(a, b) a ^= b; b ^= a; a ^= b;
#define LEAFSIZE(canvas) (canvas->w + canvas->h) / 2 / 60

int clamp(int val, int min, int max) {
	if (val < min) {
		return min;
	} else if (val > max) {
		return max;
	}
	return val;
}

void putpixel(SDL_Surface *surface, size_t x, size_t y, SDL_Color c, int alpha=255) {
	// Presahuje rozmery surface, zapis do neplatne pameti
	if (x >= abs(surface->w) || y >= abs(surface->h)) {
		return;
	}
	/*const unsigned short noise = 128;
	const double random = (basicRandom() * noise) - noise / 2;
	c.r = clamp(c.r + random, 0, 255);
	c.g = clamp(c.g + random, 0, 255);
	c.b = clamp(c.b + random, 0, 255);
	*/
	const float noise = 0.8;
	const double random = 1.0+basicRandom()*noise - noise/2.0;
	c.r = clamp(c.r*random, 0, 255);
	c.g = clamp(c.g*random, 0, 255);
	c.b = clamp(c.b*random, 0, 255);
	//Uint32 color = SDL_MapRGB(surface->format, c.r, c.g, c.b);
	
	/*
	switch(surface->format->BytesPerPixel) {
		case 1:// Assuming 8-bpp
			{
			Uint8 *bufp;
			bufp = (Uint8 *)surface->pixels + y*surface->pitch + x;
			*bufp = color;
			}
			break;
		case 2:// Probably 15-bpp or 16-bpp
			{
			Uint16 *bufp;
			bufp = (Uint16 *)surface->pixels + y*surface->pitch/2 + x;
			*bufp = color;
			}
			break;
		case 3:// Slow 24-bpp mode, usually not used
			{
			Uint8 *bufp;
			bufp = (Uint8 *)surface->pixels + y*surface->pitch + x*surface->format->BytesPerPixel;
			*(bufp+surface->format->Rshift/8) = c.r;
			*(bufp+surface->format->Gshift/8) = c.g;
			*(bufp+surface->format->Bshift/8) = c.b;
			}
			break;
		case 4:// Probably 32-bpp
			{
			Uint32 *bufp;
			bufp = (Uint32 *)surface->pixels + y*surface->pitch/4 + x;
			*bufp = color;
			}
			break;
	}
	*/
	//pouzivame napevno 32bitovou barevnou hloubku
	Uint32 *bufp;
	bufp = (Uint32 *)surface->pixels + y*surface->pitch/4 + x;
	Uint8 orig_r;
	Uint8 orig_g;
	Uint8 orig_b;
	SDL_GetRGB(*bufp, surface->format, &orig_r, &orig_g, &orig_b);
	Uint8 new_r = c.r*alpha/255.0+orig_r*(1.0-alpha/255.0);
	Uint8 new_g = c.g*alpha/255.0+orig_g*(1.0-alpha/255.0);
	Uint8 new_b = c.b*alpha/255.0+orig_b*(1.0-alpha/255.0);

	Uint32 color = SDL_MapRGB(surface->format, new_r, new_g, new_b);
	*bufp = color;
}

void drawleaf(SDL_Surface *canvas, size_t x, size_t y, SDL_Color color) {
	const size_t size = LEAFSIZE(canvas);
	//std::clog << size << '\n';
	
	x -= size / 2;
	y -= size / 2;
	
	float alpha;
	size_t tx, ty;
	for (size_t i = 0; i <= size / 2; i++) {
		for (size_t j = i * 2; j > 0; j--) {
			tx = x + j + size / 2 - i;
			alpha = 1.0-(abs(int(tx-(x+size/2)))/float(size)*2.0);
			alpha *= 1.0-(abs(float(i)-size/2)/float(size)*2.0); //cim vice ke kraji, tim pruhlednejsi
			putpixel(canvas, tx, y + i, color, alpha*255);
			putpixel(canvas, tx, y + size - i, color, alpha*255);
		}
	}
}

void drawbranch(SDL_Surface *canvas, size_t width, size_t height, int x1, int y1, int x2, int y2, float thickness, SDL_Color wood, SDL_Color leaf, size_t leafinterval) {
	//std::clog << "Line: " << '[' << x1 << ", " << y1 << "] -> [" << x2 << ", " << y2 << ']' << '\n';
	//std::clog << leafinterval << '\n';
	
	x1 = clamp(x1, 0, width - 1);
	y1 = clamp(y1, 0, height - 1);
	x2 = clamp(x2, 0, width - 1);
	y2 = clamp(y2, 0, height - 1);
	float thickness_orig = thickness;
	if (thickness < 1) {
		thickness = 1;
	}
	if (leafinterval == 0) {
		leafinterval = max(canvas->w, canvas->h);
	}
	
	const bool steep = abs(y1 - y2) > abs(x1 - x2);
	if (steep) {
		SWAP(x1, y1);
		SWAP(x2, y2);
	}
	if (x1 > x2) {
		SWAP(x1, x2);
		SWAP(y1, y2);
	}
	short step_y = 1;
	if (y1 > y2) {
		step_y = -1;
	}
	
	const unsigned int dx = abs(x2 - x1);
	const unsigned int dy = abs(y2 - y1);
	int P = 2 * dy - dx;
	const unsigned int P1 = 2 * dy;
	const int P2 = P1 - 2 * dx;
	unsigned int y = y1;
	SDL_Color wood_orig = wood;
	
	for (int x = x1; x <= x2; x++) {	//Vetev
		for (size_t t = thickness; t > 0; t--) {
			wood = wood_orig;
			float koef = sqrt((thickness-abs(thickness-(t*2.0-0.5)))/(thickness)); //pixel, vycentrovany, z nej spocitana tmavost kmene
			koef += sqrt((thickness-abs(thickness-(t*2.0)))/(thickness)); //hodnota na jednom kraji pixelu
			koef += sqrt((thickness-abs(thickness-(t*2.0-1.0)))/(thickness)); //hodnota na druhem kraji pixelu
			koef /= 3.0; //prumer
			koef *= float(thickness_orig)/thickness;
			wood.r *= koef;
			wood.g *= koef;
			wood.b *= koef;
			if (steep) {
				putpixel(canvas, y - thickness / 2 + t, x, wood);
			} else {
				putpixel(canvas, x , y - thickness / 2 + t, wood);
			}
		}
		
		if (P >= 0) {
			P += P2;
			y += step_y;
		} else {
			P += P1;
		}
	}
	
	y = y1;
	SDL_Color leaf_orig = leaf;
	for (int x = x1; x <= x2; x++) {	//Listy
		if (x % leafinterval == 0) {
			leaf = leaf_orig;
			float yrand = basicRandom();
			float xrand = basicRandom();
			size_t yoffset = yrand * thickness - thickness / 2;
			size_t xoffset = xrand * dx - dx / 2;
			float koef = xrand;
			koef = 1.0+koef-0.5;
			leaf.r = clamp(leaf.r*koef, 0, 255);
			leaf.g = clamp(leaf.g*koef, 0, 255);
			leaf.b = clamp(leaf.b*koef, 0, 255);
			if (steep) {
				drawleaf(canvas, y + yoffset, x + xoffset, leaf);
			} else {
				drawleaf(canvas, x + xoffset, y + yoffset, leaf);
			}
		}
		
		if (P >= 0) {
			P += P2;
			y += step_y;
		} else {
			P += P1;
		}
	}
}

void GenTreeBillboardTexture_visualize(SDL_Surface * data, size_t width, size_t height, TreeNode* tree, SDL_Color wood, SDL_Color leafs) {
	//std::clog << "Visualizing tree...\n";
	
	boundingBox bounds = getBoundingBox(tree);
	//std::clog << "Bounds: " << (std::string)(bounds) << '\n';
	const float treeWidth = abs(bounds.minX - bounds.maxX);
	const float treeHeight = abs(bounds.minZ - bounds.maxZ);
	
	const float treeAspect = treeWidth / treeHeight;
	//std::clog << "Tree aspect ratio: " << treeAspect << '\n';
	const float imageAspect = (float) width / (float) height;
	//std::clog << "Image aspect ratio: " << imageAspect << '\n';
	float scale = 1;
	size_t offset = 0;
	if (treeAspect < imageAspect) {
		scale = height / treeHeight;
		offset = (width - treeWidth * scale) / 2;
	} else {
		scale = width / treeWidth;
	}
	
	queue<TreeNode*> q;
	q.push(tree);
	while (!q.empty()) {
		//std::clog << q.size() << '\n';
		TreeNode* node = q.front();
		
		cartesianCoords origin;
		if (!node->parentNode) {
			origin.x = 0;
			origin.z = 0;
		} else {
			origin = node->parentNode->param.branchEnd;
		}
		
		//std::clog << node->param.leafs << ' ' << scale << '\n';
		drawbranch(data, width, height,
			(origin.x + abs(bounds.minX)) * scale + offset,
			height - 1 - origin.z * scale,
			(node->param.branchEnd.x + abs(bounds.minX)) * scale + offset,
			height - node->param.branchEnd.z * scale,
			node->param.thickness * scale,
			wood,
			leafs,
			LEAFSIZE(data) * (1 / node->param.leafs));
		
		for (size_t i = 0; i < node->childNodes.size(); i++) {
			//std::clog << "Pushing new node\n";
			q.push(node->childNodes[i]);
		}
		
		q.pop();
	}
	
	//std::clog << "Vis done!\n";
	return;
}
