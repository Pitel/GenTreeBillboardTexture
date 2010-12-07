#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>

#include <SDL.h>

#include "GenTreeBillboardTexture.h"

#define SDL_SUBSYSTEMS SDL_INIT_VIDEO
#define WIN_FLAGS SDL_SWSURFACE |SDL_RESIZABLE
#define WIN_BPP 32


unsigned int width = 256;
unsigned int height = 512;
unsigned int seed = 2353;
unsigned int depth = 150;
PTreeType treeType = PTREE_PICEA;

bool use_sdlsurface = false;

SDL_Color trunk = {150,75,0,0};
SDL_Color leaf = {0,255,0,0};

SDL_Surface *window;
SDL_Surface *tree_texture;  // Vygenerovany strom

void InitTexture(){
	if(tree_texture != NULL){
		free(tree_texture);
	}
    //Pouzije se SDL_surface

        tree_texture = SDL_CreateRGBSurface(
                SDL_SWSURFACE,
                width, height, 32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
                0x000000FF,
                0x0000FF00,
                0x00FF0000,
                0xFF000000
#else
                0xFF000000,
                0x00FF0000,
                0x0000FF00,
                0x000000FF
#endif
                );

		GenTreeBillboardTexture(tree_texture, width, height, seed, depth, treeType, trunk, leaf);
}
bool Init()
{
	// Inicializace SDL
	if(SDL_Init(SDL_SUBSYSTEMS) == -1)
	{
		fprintf(stderr, "Unable to initialize SDL: %s\n",
				SDL_GetError());
		return false;
	}

	// Vytvori okno s definovanymi vlastnostmi
	window = SDL_SetVideoMode(width, height, WIN_BPP, WIN_FLAGS);

	if(window == NULL)
	{
		fprintf(stderr, "Unable to set %dx%d video: %s\n",
				width, height, SDL_GetError());
		return false;
	}

	SDL_WM_SetCaption("GenTreeBillboardTexture - SDL", NULL);

	SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 0, 0, 0));

	InitTexture();

	return true;
}

void Destroy()
{
	if(tree_texture != NULL)
		SDL_FreeSurface(tree_texture);
	SDL_Quit();
}



void Draw()
{
	SDL_Rect rect;

	rect.x = (window->w >> 1) - (tree_texture->w >> 1);
	rect.y = (window->h >> 1) - (tree_texture->h >> 1);

	SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 0, 0, 0));
	SDL_BlitSurface(tree_texture, NULL, window, &rect);
	SDL_UpdateRect(window, rect.x, rect.y, rect.w, rect.h);
}


bool ProcessEvent()
{
	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			// Klavesnice
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						return false;
						break;

					default:
						break;
				}
				break;

			// Zmena velikosti okna
			case SDL_VIDEORESIZE:
				window = SDL_SetVideoMode(event.resize.w,
					event.resize.h, WIN_BPP, WIN_FLAGS);
				width = event.resize.w;
				height = event.resize.h;
				InitTexture();

				if(window == NULL)
				{
					fprintf(stderr,
						"Unable to resize window: %s\n",
						SDL_GetError());
					return false;
				}
				break;

			// Pozadavek na ukonceni
			case SDL_QUIT:
				return false;
				break;

			default:
				break;
		}
	}

	return true;
}



int main(int argc, char *argv[])
{

  // std::cout << "ARGC: "<< argc << std::endl;

   //std::cout << "ARGV: "<< argv[0] << std::endl;

    if(argc == 11)
    {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
        seed = atoi(argv[3]);
        depth = atoi(argv[4]);
/*      
        std::cout << std::endl << "**** demo_sdl ****" << std::endl;
        std::cout << "ARGV: "<< argv[1] << std::endl;
        std::cout << "ARGV: "<< argv[2] << std::endl;
        std::cout << "ARGV: "<< argv[3] << std::endl;
        std::cout << "ARGV: "<< argv[4] << std::endl;
        std::cout << "ARGV: "<< argv[5] << std::endl;
        std::cout << "ARGV: "<< argv[6] << std::endl;
        std::cout << "ARGV: "<< argv[7] << std::endl;
        std::cout << "ARGV: "<< argv[8] << std::endl;
        std::cout << "ARGV: "<< argv[9] << std::endl;
        std::cout << "ARGV: "<< argv[10] << std::endl;*/

        trunk.r = atoi(argv[5]);
        trunk.g = atoi(argv[6]);
        trunk.b = atoi(argv[7]);

        leaf.r = atoi(argv[8]);
        leaf.g = atoi(argv[9]);
        leaf.b = atoi(argv[10]);

    }



    // Inicializace
	if(!Init())
	{
		Destroy();
		return 1;
	}

	// Hlavni smycka programu
	while(ProcessEvent())
	{
		Draw();
	}

	// Deinicializace a konec
	Destroy();
	return 0;
}
