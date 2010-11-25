#include <stdio.h>
#include "GenTreeBillboardTexture.h"
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, const char* argv[]) {
	const size_t width = 256;
	const size_t height = 512;
	char asciiart[height][width];
	ofstream xpm;
	xpm.open("tree.xpm");
	xpm << "/* XPM */\n";
	xpm << "static char * tree[] = {\n";
	xpm << '"' << width << ' ' << height << " 3 1\",\n";
	xpm << "\"# c #000000\",\n";
	xpm << "\". c #ffffff\",\n";
	xpm << "\"O c #00ff00\",\n";

  int depth = 40;
	int seed = 23505;
	
	if(argc >= 2){
		stringstream seedss(argv[1]);
		seedss >> seed;
		cout << "seed: " << seed << endl;
	}
	
	if(argc >= 3)
	{
    stringstream depths(argv[2]);
		depths >> depth;
		cout << "depth: " << depth << endl;
  }
  
	GenTreeBillboardTexture((char*)asciiart, width, height, seed, depth);
	
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
