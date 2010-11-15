#include <stdio.h>
#include "GenTreeBillboardTexture.h"
#include <iostream>
#include <fstream>
using namespace std;

int main() {
	const size_t width = 256;
	const size_t height = 512;
	char asciiart[height][width];
	ofstream xpm;
	xpm.open("tree.xpm");
	xpm << "/* XPM */\n";
	xpm << "static char * tree[] = {\n";
	xpm << '"' << width << ' ' << height << " 2 1\",\n";
	xpm << "\"# c #000000\",\n";
	xpm << "\". c #ffffff\",\n";
	GenTreeBillboardTexture((char*)asciiart, width, height, 23505);
	for (size_t y = 0; y < height; y++) {
		xpm << '"';
		for (size_t x = 0; x < width; x++) {
			xpm << asciiart[y][x];
		}
		xpm << "\",\n";
	}
	xpm << '}';
	xpm.close();
	return 0;
}
