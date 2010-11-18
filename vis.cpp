#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "vis.h"

#define SWAP(a, b) a ^= b; b ^= a; a ^= b;

void putpixel(char* canvas, size_t width, size_t x, size_t y, char pixel) {
	*(canvas + x + y * width) = pixel;
}

void drawline(char* canvas, size_t width, int x1, int y1, int x2, int y2, char pixel) {
	std::cout << "Line: " << '[' << x1 << ", " << y1 << "] -> [" << x2 << ", " << y2 << ']' << '\n';
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
		if (steep) {
			putpixel(canvas, width, y, x, pixel);
		} else {
			putpixel(canvas, width, x, y, pixel);
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
	printf("Visualizing tree...\n");
	memset(data, '.', width * height);
	
	boundingBox bounds = getBoundingBox(tree);
	//std::cout << "Bounds: " << (std::string)(bounds) << '\n';
	
	/* FIXME Scaling is wrong! This will stretch the tree. Make it preserve aspect ratio! */
	float scaleV = height / abs(bounds.minZ - bounds.maxZ);
	//std::cout << scaleV << '\n';
	float scaleH = width / abs(bounds.minX - bounds.maxX);
	//std::cout << scaleH << '\n';
	
	queue<TreeNode*> q;
	q.push(tree);
	while (!q.empty()) {
		std::cout << q.size() << '\n';
		TreeNode* node = q.front();
		
		cartesianCoords origin;
		if (!node->parentNode) {
			origin.x = 0;
			origin.z = 0;
		} else {
			origin = node->parentNode->param.branchEnd;
		}
		
		drawline(data, width,
			(origin.x + abs(bounds.minX)) * scaleH,
			height - 1 - origin.z * scaleV,
			(node->param.branchEnd.x + abs(bounds.minX)) * scaleH,
			height - 1 - node->param.branchEnd.z * scaleV,
			'#');
		
		for (size_t i = 0; i < node->childNodes.size(); i++) {
			std::cout << "Pushing new node\n";
			q.push(node->childNodes[i]);
		}
		
		q.pop();
	}
	std::cout << "Vis done!\n";
	return;
}
