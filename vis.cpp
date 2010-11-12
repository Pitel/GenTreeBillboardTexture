#include <stdio.h>
#include <string.h>
#include "vis.h"

void putpixel(char* canvas, size_t width, size_t x, size_t y, char pixel) {
	*(canvas + x * width + y) = pixel;
}

void GenTreeBillboardTexture_visualize(char* data, size_t width, size_t height) {
	printf("Visualizing tree...\n");
	memset(data, ' ', width * height);
	for (size_t x = 0; x < width; x++) {
		for (size_t y = 0; y < height; y++) {
			putpixel(data, width, x, y, '0' + y + x);
		}
	}
	return;
}
