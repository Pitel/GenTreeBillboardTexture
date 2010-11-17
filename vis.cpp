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

/*
vector<float> s2c(float r, float theta, float phi) {
	std::cout << r;
	vector<float> xyz;
	xyz[0] = r * sin(theta) * cos(phi);
	xyz[1] = r * sin(theta) * sin(phi);
	xyz[2] = r * cos(phi);
	return xyz;
}
*/

void GenTreeBillboardTexture_visualize(char* data, size_t width, size_t height, TreeNode* tree) {
	printf("Visualizing tree...\n");
	memset(data, '.', width * height);
	
	boundingBox bounds = getBoundingBox(tree);
	//std::cout << "Bounds: " << (std::string)(bounds) << '\n';
	
	/* FIXME Scaling is wrong! This will stretch the tree. Make it preserve aspect ratio! */
	float scaleH = height / abs(bounds.minZ - bounds.maxZ);
	//std::cout << scaleH << '\n';
	float scaleV = width / abs(bounds.minX - bounds.maxX);
	//std::cout << scaleV << '\n';
	
	drawline(data, width,
		width / 2,
		height - 1,
		tree->param.branchEnd.x * scaleH + width / 2,
		height - tree->param.branchEnd.z * scaleV,
		'#');
	return;
}
