#include <stdio.h>
#include "GenTreeBillboardTexture.h"

int main() {
	const size_t width = 10;
	const size_t height = 10;
	char asciiart[width][height];
	GenTreeBillboardTexture((char*)asciiart, width, height, 23505);
	for (size_t x = 0; x < width; x++) {
		for (size_t y = 0; y < height; y++) {
			putchar(asciiart[x][y]);
		}
		putchar('\n');
	}
	return 0;
}
