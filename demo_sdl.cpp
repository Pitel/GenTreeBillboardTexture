#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <ctime>

#include <SDL.h>

#include "GenTreeBillboardTexture.h"

#define SDL_SUBSYSTEMS SDL_INIT_VIDEO
#define WIN_FLAGS SDL_SWSURFACE |SDL_RESIZABLE
#define WIN_BPP 32


unsigned int width = 600;
unsigned int height = 800;
unsigned int seed = 2353;
unsigned int depth = 300;

PTreeType treeType = PTREE_APPLE;

bool use_sdlsurface = false;

SDL_Color trunk = {90,60,30,0};
SDL_Color leaf = {30,150,30,0};

SDL_Surface *window;
SDL_Surface *tree_texture;  // Vygenerovany strom

void InitTexture(){
	if(tree_texture != NULL){
		free(tree_texture);
	}

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

    if(SDL_MUSTLOCK(tree_texture))
        SDL_LockSurface(tree_texture);

    GenTreeBillboardTexture(tree_texture, seed, depth, treeType, trunk, leaf);

    if(SDL_MUSTLOCK(tree_texture))
        SDL_UnlockSurface(tree_texture);

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


SDL_Event event;

bool ProcessEvent()
{

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

    if(argc == 1)
    {
        cout << "Please select type of tree:" << endl;
        cout << "----------------------------" << endl;
        cout << "1 - Apple tree" << endl;
        cout << "2 - Spruce" << endl;
        cout << "3 - Palm tree" << endl << endl;
        cout << "Please enter nubmer [1, 2 or 3]: " << flush;
        string typ("");


        getline(cin,typ);


        if(!strcmp(typ.c_str(),"1"))
        {
            treeType = PTREE_APPLE;
            depth = 350;
        }

        if(!strcmp(typ.c_str(),"2"))
        {
            treeType = PTREE_PICEA;
            depth = 4000;
        }

        if(!strcmp(typ.c_str(),"3"))
        {
            treeType = PTREE_PALMA;
            depth = 2500;
        }
    }
    else
    if(argc == 12)
    {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
        seed = atoi(argv[3]);
        depth = atoi(argv[4]);


        trunk.r = atoi(argv[5]);
        trunk.g = atoi(argv[6]);
        trunk.b = atoi(argv[7]);

        leaf.r = atoi(argv[8]);
        leaf.g = atoi(argv[9]);
        leaf.b = atoi(argv[10]);

        if(!strcmp(argv[11],"apple"))
            treeType = PTREE_APPLE;

        if(!strcmp(argv[11],"spruce"))
            treeType = PTREE_PICEA;

        if(!strcmp(argv[11],"palm"))
            treeType = PTREE_PALMA;
    }
    else
    {
        cout << "Run ./demo and select tree or use ./demo_gui for better parametrization of tree." << endl;
        exit(0);
    }



    // Inicializace
	if(!Init())
	{
		Destroy();
		return 1;
	}


    for(;;)// Infinite loop
    {
        SDL_Event event;

        // Wait for event
        if(SDL_WaitEvent(&event) == 0) //throw SDL_Exception();
            cerr << "Error SDL_WaitEvent" << endl;

         // Handle all waiting events
        do
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

        } while(SDL_PollEvent(&event) == 1);

        Draw();
    }

	Destroy();
	return 0;
}
