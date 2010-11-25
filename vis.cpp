#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "vis.h"

#define SWAP(a, b) a ^= b; b ^= a; a ^= b;

void putpixel(char* canvas, size_t width, size_t x, size_t y, char pixel) {
	*(canvas + x + y * width) = pixel;
}

void drawline(char* canvas, size_t width, int x1, int y1, int x2, int y2, size_t thickness, char pixel) {
	std::clog << "Line: " << '[' << x1 << ", " << y1 << "] -> [" << x2 << ", " << y2 << ']' << '\n';
	if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0) {
		std::cerr << "Oh, crap! Can't draw to negative coords!\n";	//FIXME
		return;
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
				putpixel(canvas, width, y - thickness / 2 + t, x, pixel);
			} else {
				putpixel(canvas, width, x , y - thickness / 2 + t, pixel);
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

void GenTreeBillboardTexture_visualize(char* data, size_t width, size_t height, TreeNode* tree) {
	std::clog << "Visualizing tree...\n";
	memset(data, '.', width * height);
	
	boundingBox bounds = getBoundingBox(tree);
	//std::clog << "Bounds: " << (std::string)(bounds) << '\n';
	
	float treeAspect = abs(bounds.minX - bounds.maxX) / abs(bounds.minZ - bounds.maxZ);
	//std::clog << "Tree aspect ratio: " << treeAspect << '\n';
	float imageAspect = (float) width / (float) height;
	//std::clog << "Image aspect ratio: " << imageAspect << '\n';
	float scale = 0;
	if (treeAspect < imageAspect) {
		scale = height / abs(bounds.minZ - bounds.maxZ);
	} else {
		scale = width / abs(bounds.minX - bounds.maxX);
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
		
		char pixel = '#';
		
		drawline(data, width,
			(origin.x + abs(bounds.minX)) * scale,
			height - 1 - origin.z * scale,
			(node->param.branchEnd.x + abs(bounds.minX)) * scale,
			height - 1 - node->param.branchEnd.z * scale,
			5,
			pixel);
		
		for (size_t i = 0; i < node->childNodes.size(); i++) {
			std::clog << "Pushing new node\n";
			q.push(node->childNodes[i]);
		}
		
		q.pop();
	}
	std::clog << "Vis done!\n";
	return;
}
