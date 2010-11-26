#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>

#include <SDL.h>
#include <SDL_image.h>

#include "GenTreeBillboardTexture.h"

#define SDL_SUBSYSTEMS SDL_INIT_VIDEO
#define WIN_FLAGS SDL_HWSURFACE|SDL_RESIZABLE
#define WIN_BPP 0


unsigned int width = 256;
unsigned int height = 512;
unsigned int seed = 23505;
unsigned int depth = 40;

bool use_sdlsurface = false;


SDL_Surface *window;
SDL_Surface *tree_texture;  // Vygenerovany strom


void generate_xpm()
{
	char asciiart[height][width];
	//char *asciiart = new char[int(height)];
	ofstream xpm;
	xpm.open("tree.xpm");
	xpm << "/* XPM */\n";
	xpm << "static char * tree[] = {\n";
	xpm << '"' << width << ' ' << height << " 2 1\",\n";
	xpm << "\"# c #000000\",\n";
	xpm << "\". c #ffffff\",\n";

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
}

SDL_Surface *LoadImage(const char *filename, bool alpha = false);
SDL_Surface *LoadImage(const char *filename, bool alpha)
{
	SDL_Surface *tmp;
	SDL_Surface *ret;

	if((tmp = IMG_Load(filename)) == NULL)
	{
		fprintf(stderr, "%s\n", SDL_GetError());
		return NULL;
	}

	if((ret = (alpha) ? SDL_DisplayFormatAlpha(tmp)
			: SDL_DisplayFormat(tmp)) == NULL)
	{
		fprintf(stderr, "%s\n", SDL_GetError());
		SDL_FreeSurface(tmp);
		return NULL;
	}

	SDL_FreeSurface(tmp);

	return ret;
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


    if(!use_sdlsurface)
    {
        //Pouzije se pro sdl surface nacteni z obrazku...
        generate_xpm();
        if((tree_texture = LoadImage("tree.xpm")) == NULL)
            return false;
    }
    else
    {
        //Pouzije se SDL_surface
    }

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

   //std::cout << "ARGC: "<< argc << std::endl;

   //std::cout << "ARGV: "<< argv[0] << std::endl;

    if(argc == 6)
    {
        if(!strcmp(argv[1],"sdl"))
           use_sdlsurface = true;

        width = atoi(argv[2]);
        height = atoi(argv[3]);
        seed = atoi(argv[4]);
        depth = atoi(argv[5]);
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
