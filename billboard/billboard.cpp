#include "pgr.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_projection.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <queue>


#include <SDL.h>
#include "../GenTreeBillboardTexture.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Data
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int width, height;// Window size
float rx = 0.0f, ry = 0.0f, pz = 3.0f;

int depth = 40;
int seed = 23505;

int camposx = 0, camposz = 0; //pozice kamery

bool isHoldKey = false;
SDLKey holdKey = SDLK_ESCAPE;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Shaders
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Event handlers
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

unsigned int width_tree = 300;
unsigned int height_tree = 300;
unsigned int seed_tree = 23505;
unsigned int depth_tree = 400;

bool use_sdlsurface = false;


SDL_Surface *window;

GLuint g_grass = 0;

PTreeType treeType = PTREE_APPLE;


#define TREE_COUNT 30
GLuint g_tree[TREE_COUNT];
cartesianCoords tc[TREE_COUNT];

SDL_Color trunk[PTREE_COUNT] = {{120,65,0,0},{150,75,0,0},{180,105,45,0}};
SDL_Color leaf[PTREE_COUNT] = {{42,150,40,0},{30,90,0,30},{45,180,15,0}};

void onInit()
{

}

void genTexture()
{
    GLint  nOfColors;
    GLenum texture_format;


	glGenTextures(TREE_COUNT, g_tree);
	for(int i=0; i<TREE_COUNT; i++){
		tc[i].x = uniformRandom(-10.0, 10.0);
		tc[i].y = 0.0;
		tc[i].z = (1.0-i/float(TREE_COUNT))*40.0;
		float dist = sqrt(tc[i].x*tc[i].x+tc[i].z*tc[i].z);
		PTreeType tt = (PTreeType)(int)uniformRandom(0.0, PTREE_COUNT);
		int depth;
		if(dist < 10.0){
			depth = 4000;
			width_tree = 512;
			height_tree = 512;
		}else if(dist < 20.0){
			depth = 1000;
			width_tree = 256;
			height_tree = 256;
		}else if(dist < 30.0){
			depth = 200;
			width_tree = 128;
			height_tree = 128;
		}else{
			depth = 40;
			width_tree = 32;
			height_tree = 32;
		}

		if(tt == PTREE_APPLE){
			depth = max(40, depth/10);
		}

 SDL_Surface * tree_texture = SDL_CreateRGBSurface(
                SDL_SWSURFACE | SDL_SRCCOLORKEY | SDL_SRCALPHA,
                width_tree, height_tree, 32,
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

    SDL_SetColorKey(tree_texture, SDL_SRCCOLORKEY,
			SDL_MapRGB(tree_texture->format, 0, 0, 0));
    SDL_SetAlpha(tree_texture, 0, 0);

    GenTreeBillboardTexture(tree_texture, basicRandom()*5000, depth, tt, trunk[tt], leaf[tt]);


    nOfColors = tree_texture->format->BytesPerPixel;
		if (nOfColors == 4)     // contains an alpha channel
		{
			if (tree_texture->format->Rmask == 0x000000ff)
				texture_format = GL_RGBA;
			else
				texture_format = GL_BGRA;
		} else if (nOfColors == 3)     // no alpha channel
		{
			if (tree_texture->format->Rmask == 0x000000ff)
				texture_format = GL_RGB;
			else
				texture_format = GL_BGR;
		} else {
			printf("warning: the image is not truecolor..  this will probably break\n");
			// this error should not go unhandled
		}


		glBindTexture(GL_TEXTURE_2D, g_tree[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, tree_texture->w, tree_texture->h, 0,
				texture_format, GL_UNSIGNED_BYTE, tree_texture->pixels );

		SDL_FreeSurface(tree_texture);
	}

    //Load texture from file
    SDL_Surface * grass = SDL_LoadBMP("grass.bmp");
    if( grass == NULL)
        cerr << "ERROR: SDL_LoadBMP" << endl;

    glGenTextures(1, &g_grass);
    glBindTexture(GL_TEXTURE_2D, g_grass);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    SurfaceImage2D(GL_TEXTURE_2D, 0, GL_RGB, grass);

    SDL_FreeSurface(grass);
}


void onWindowRedraw()
{
	if(isHoldKey == true){
		switch(holdKey) {
			case SDLK_ESCAPE : quit(); break;
			case SDLK_a : camposx -= 1; break;
			case SDLK_d : camposx += 1; break;
			case SDLK_w : camposz -= 1; break;
			case SDLK_s : camposz += 1; break;
			default : break;
		}
	}

	glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);


    glm::mat4 projection = glm::perspective(45.0f, (float)width/(float)height, 1.0f, 1000.0f);

	glm::mat4 mv =
				glm::rotate(
					glm::rotate(
						glm::translate(
                            glm::translate(
                                glm::translate(
                                    glm::mat4(1.0f),
                                    glm::vec3(0, 0, -pz)
                                    ),
                                    glm::vec3(-camposx, 0, 0)
                                ),
                                glm::vec3(0, 0, -camposz)
							),
                        rx, glm::vec3(0, 1, 0)
						),
                    ry, glm::vec3(1, 0, 0));


	glm::mat4 mvp = projection*mv;


    glLoadMatrixf(glm::value_ptr(mvp));

	glColor4f(1.0f,1.0f,1.0f, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, g_grass);

    glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 20.0f); glVertex3f(-10.0f, -1.0f, -40.0f);
		glTexCoord2f(20.0f, 20.0f); glVertex3f(-10.0f, -1.0f,  0.0f);
		glTexCoord2f(20.0f, 0.0f); glVertex3f(10.0f, -1.0f,  0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, -1.0f, -40.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
	glColor4f(1.0f,1.0f,1.0f, 1.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glDepthMask(GL_FALSE);
	for(int i=0; i<TREE_COUNT; i++){
		mv =    glm::rotate(
					glm::rotate(
						glm::translate(
							glm::translate(
								glm::translate(
									glm::mat4(1.0f),
									glm::vec3(0, 0, -pz)
									),
								glm::vec3(-camposx, 0, 0)
								),
							glm::vec3(0, 0, -camposz)
							),
						rx, glm::vec3(0, 1, 0)
						),
					ry, glm::vec3(1, 0, 0)
				);

		mvp =  glm::rotate(projection*mv, -rx, glm::vec3(0, 1, 0)); //Pootoceni zpet

		glLoadMatrixf(glm::value_ptr(mvp));

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, g_tree[i]);

		float x,y,z;
		x = tc[i].x;
		y = tc[i].y;
		z = tc[i].z;

		mv = glm::translate(
					glm::rotate(
						glm::rotate(
							glm::translate(
								glm::translate(
									glm::translate(
										glm::mat4(1.0f),
										glm::vec3(0, 0, -pz)
										),
									glm::vec3(-camposx, 0, 0)
									),
								glm::vec3(0, 0, -camposz)
								),
							rx, glm::vec3(0, 1, 0)
							),
						ry, glm::vec3(1, 0, 0)
						),
					glm::vec3(-x,-y,-z));


		glm::vec4 a =  mv * glm::vec4(0, 0, 0, 1);

		glm::vec3 pos = glm::vec3(a[0]/a[3], a[1]/a[3], a[2]/a[3]);

		float t = (atan(pos.x/(pos.z))/M_PI)*180;

		glm::mat4 mv2 = glm::rotate(
                            glm::rotate(
                                glm::mat4(1.0f),
                                rx, glm::vec3(0, 1, 0)
                                ),
                                ry, glm::vec3(1, 0, 0)
                            );

		glm::vec4 b =  mv2 * glm::vec4(0, 0, -1, 1);

		glm::vec3 pos2 = glm::vec3(b[0]/b[3], b[1]/b[3], b[2]/b[3]);

		float t2 = (atan(pos2.x/(pos2.z))/M_PI)*180;
		t -= t2;

		mv =
			glm::rotate(
						glm::translate(
							glm::rotate(
								glm::rotate(
									glm::translate(
										glm::translate(
											glm::translate(
												glm::mat4(1.0f),
												glm::vec3(0, 0, -pz)
												),
											glm::vec3(-camposx, 0, 0)
											),
										glm::vec3(0, 0, -camposz)
										),
									rx, glm::vec3(0, 1, 0)
									),
								ry, glm::vec3(1, 0, 0)
								),
							glm::vec3(-x,-y,-z)),
						t, glm::vec3(0, 1, 0));

		mvp = projection*mv;

		glLoadMatrixf(glm::value_ptr(mvp));

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
		glEnd();
	}

    glDisable(GL_TEXTURE_2D);

    SDL_GL_SwapBuffers();
}

void onWindowResized(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w; height = h;
}

void onKeyDown(SDLKey key, Uint16 /*mod*/)
{
	holdKey = key;
	isHoldKey = true;
	onWindowRedraw();
}

void onKeyUp(SDLKey key, Uint16 /*mod*/)
{
	isHoldKey = false;
	onWindowRedraw();
}

void onMouseMove(unsigned /*x*/, unsigned /*y*/, int xrel, int yrel, Uint8 buttons)
{
    if(buttons & SDL_BUTTON_LMASK)
    {
        rx += xrel;
        ry += yrel;
        redraw();
    }
    if(buttons & SDL_BUTTON_RMASK)
    {
        pz += yrel;
        redraw();
    }
}

void onMouseDown(Uint8 button, unsigned /*x*/, unsigned /*y*/)
{
    switch(button) {
        case SDL_BUTTON_WHEELUP : break;
        case SDL_BUTTON_WHEELDOWN : break;
        default : return;
    }
}

void onMouseUp(Uint8 /*button*/, unsigned /*x*/, unsigned /*y*/)
{
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{

	if(argc >= 2){
		stringstream seedss(argv[1]);
		seedss >> seed;
		srand(seed);
	}

	if(argc >= 3)
	{
		stringstream depths(argv[2]);
		depths >> depth;
	}

	try {
		// Init SDL - only video subsystem will be used
		if(SDL_Init(SDL_INIT_VIDEO) < 0) throw SDL_Exception();

        // Shutdown SDL when program ends
        atexit(SDL_Quit);

        init(800, 600, 24, 24, 0);

        genTexture();
        mainLoop(100);

    } catch(exception & ex) {
        cout << "ERROR : " << ex.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

