#include <stdio.h>
#include "vis.h"

void GenTreeBillboardTexture_visualize(char* data, size_t width, size_t height) {
	printf("Visualizing tree...\n");
	for (size_t x = 0; x < width; x++) {
		for (size_t y = 0; y < height; y++) {
			*(data + x * width + y) = '0' + y + x;
		}
	}
	return;
}
