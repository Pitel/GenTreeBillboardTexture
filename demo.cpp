#include <stdio.h>
#include "GenTreeBillboardTexture.h"

int main() {
	const size_t width = 80;
	const size_t height = 25;
	char asciiart[height][width];
	GenTreeBillboardTexture((char*)asciiart, width, height, 23505);
	for (size_t y = 0; y < height; y++) {
		for (size_t x = 0; x < width; x++) {
			putchar(asciiart[y][x]);
		}
		putchar('\n');
	}
	return 0;
}
