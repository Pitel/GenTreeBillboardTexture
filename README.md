GenTreeBillboardTexture
=======================
The tree impostor texture generator.

![GenTreeBillboardTexture](http://farm6.static.flickr.com/5003/5246466666_40d55ce14a_m.jpg)

[Fork it at GitHub!](https://github.com/Pitel/GenTreeBillboardTexture)

Usage
-----
	#include "GenTreeBillboardTexture.h"
	SDL_Surface canvas;
	GenTreeBillboardTexture(
		SDL_Surface &canvas,	//Pointer to SDL Surface
		unsigned int seed,		//Seed for random numbers
		unsigned int depth,		//Depth of L-system
		PTreeType typeTree,		//Type of tree
		SDL_Color wood,			//Color of wood
		SDL_Color leafs			//Color of leafs
	);

Dependencies
------------
* [SDL](http://www.libsdl.org)

Authors
-------
* Bc. Jan Kaláb <xkalab00@stud.fit.vutbr.cz>
* Bc. Tomáš Kubík <xkubik15@stud.fit.vutbr.cz>
* Bc. Jan Lipovský <xlipov00@stud.fit.vutbr.cz>
* Bc. František Skála <xskala05@stud.fit.vutbr.cz>
