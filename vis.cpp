#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "vis.h"

#define SWAP(a, b) a ^= b; b ^= a; a ^= b;

int clamp(int val, int min, int max) {
	if (val < min) {
		return min;
	} else if (val > max) {
		return max;
	}
	return val;
}

void putpixel(SDL_Surface *surface, size_t x, size_t y, SDL_Color c) {
	// Presahuje rozmery surface, zapis do neplatne pameti
	if (x >= abs(surface->w) || y >= abs(surface->h)) {
		return;
	}
	c.r = clamp(normalRandom(c.r, c.r / 10), 0, 255);
	c.g = clamp(normalRandom(c.g, c.g / 10), 0, 255);
	c.b = clamp(normalRandom(c.b, c.b / 10), 0, 255);
	Uint32 color = SDL_MapRGB(surface->format, c.r, c.g, c.b);
	
	switch(surface->format->BytesPerPixel) {
		case 1:/* Assuming 8-bpp */
			{
			Uint8 *bufp;
			bufp = (Uint8 *)surface->pixels + y*surface->pitch + x;
			*bufp = color;
			}
			break;
		case 2:/* Probably 15-bpp or 16-bpp */
			{
			Uint16 *bufp;
			bufp = (Uint16 *)surface->pixels + y*surface->pitch/2 + x;
			*bufp = color;
			}
			break;
		case 3:/* Slow 24-bpp mode, usually not used */
			{
			Uint8 *bufp;
			bufp = (Uint8 *)surface->pixels + y*surface->pitch + x*surface->format->BytesPerPixel;
			*(bufp+surface->format->Rshift/8) = c.r;
			*(bufp+surface->format->Gshift/8) = c.g;
			*(bufp+surface->format->Bshift/8) = c.b;
			}
			break;
		case 4:/* Probably 32-bpp */
			{
			Uint32 *bufp;
			bufp = (Uint32 *)surface->pixels + y*surface->pitch/4 + x;
			*bufp = color;
			}
			break;
	}
}

void drawline(SDL_Surface *canvas, size_t width, size_t height, int x1, int y1, int x2, int y2, size_t thickness, SDL_Color color) {
	std::clog << "Line: " << '[' << x1 << ", " << y1 << "] -> [" << x2 << ", " << y2 << ']' << '\n';
	
	x1 = clamp(x1, 0, width - 1);
	y1 = clamp(y1, 0, height - 1);
	x2 = clamp(x2, 0, width - 1);
	y2 = clamp(y2, 0, height - 1);
	if (thickness < 1) {
		thickness = 1;
	}
	
	bool steep = abs(y1 - y2) > abs(x1 - x2);
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
	
	unsigned int dx = abs(x2 - x1);
	unsigned int dy = abs(y2 - y1);
	int P = 2 * dy - dx;
	unsigned int P1 = 2 * dy;
	int P2 = P1 - 2 * dx;
	unsigned int y = y1;
	for (int x = x1; x <= x2; x++) {
		for (size_t t = thickness; t > 0; t--) {
			if (steep) {
				putpixel(canvas, y - thickness / 2 + t, x, color);
			} else {
				putpixel(canvas, x , y - thickness / 2 + t, color);
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
	std::clog << "Visualizing tree...\n";
	
	boundingBox bounds = getBoundingBox(tree);
	//std::clog << "Bounds: " << (std::string)(bounds) << '\n';
	float treeWidth = abs(bounds.minX - bounds.maxX);
	float treeHeight = abs(bounds.minZ - bounds.maxZ);
	
	float treeAspect = treeWidth / treeHeight;
	//std::clog << "Tree aspect ratio: " << treeAspect << '\n';
	float imageAspect = (float) width / (float) height;
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
		std::clog << q.size() << '\n';
		TreeNode* node = q.front();
		
		cartesianCoords origin;
		if (!node->parentNode) {
			origin.x = 0;
			origin.z = 0;
		} else {
			origin = node->parentNode->param.branchEnd;
		}
		
		drawline(data, width, height,
			(origin.x + abs(bounds.minX)) * scale + offset,
			height - 1 - origin.z * scale,
			(node->param.branchEnd.x + abs(bounds.minX)) * scale + offset,
			height - node->param.branchEnd.z * scale,
			node->param.thickness * scale,
			wood);
		
		for (size_t i = 0; i < node->childNodes.size(); i++) {
			std::clog << "Pushing new node\n";
			q.push(node->childNodes[i]);
		}
		
		q.pop();
	}
	
	std::clog << "Vis done!\n";
	return;
}
