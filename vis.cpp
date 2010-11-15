#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vis.h"

#define SWAP(a,b) a^=b^=a^=b;

void putpixel(char* canvas, size_t width, size_t x, size_t y, char pixel) {
	*(canvas + x + y * width) = pixel;
}

void drawline(char* canvas, size_t width, size_t x1, size_t y1, size_t x2, size_t y2, char pixel) {
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
	
	size_t dx = abs(x2 - x1);
	size_t dy = abs(y2 - y1);
	ssize_t P = 2 * dy - dx;
	size_t P1 = 2 * dy;
	ssize_t P2 = P1 - 2 * dx;
	size_t y = y1;
	for (size_t x = x1; x <= x2; x++) {
		if (steep) {
			putpixel(canvas, width, y, x, pixel);
		} else {
			putpixel(canvas, width, x, y, pixel);
		}
		if (P >= 0) {
			P += P2;
			y += step_y;
		}
		else {
			P += P1;
		}
	}
}

void GenTreeBillboardTexture_visualize(char* data, size_t width, size_t height) {
	printf("Visualizing tree...\n");
	memset(data, '.', width * height);
	drawline(data, width, 0, 0, width - 1, height - 1, '#');
	drawline(data, width, width - 1, 0, 0, height - 1, '#');
	drawline(data, width, 0, 0, width - 1, 0, '#');
	drawline(data, width, width - 1, 0, width - 1, height - 1, '#');
	drawline(data, width, width - 1, height - 1, 0, height - 1, '#');
	drawline(data, width, 0, height - 1, 0, 0, '#');
	return;
}
