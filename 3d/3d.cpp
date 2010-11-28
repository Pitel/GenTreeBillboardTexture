#include "pgr.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_projection.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include "../grammar.h"
#include "../param.h"
#include <queue>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Data
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int width, height;// Window size
float rx = 0.0f, ry = -90.0f, pz = 3.0f;
float sxmin = 0.0, symin = 0.0, szmin = 0.0;
float sxmax = 0.0, symax = 0.0, szmax = 0.0;

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
//
TreeNode *mytree;
queue<TreeNode*> printNodes;

void onInit()
{
}

void onWindowRedraw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    // Calculate ModelViewProjection matrix
	boundingBox bounds = getBoundingBox(mytree);
	sxmin = bounds.minX;
	sxmax = bounds.maxX;
	symin = bounds.minY;
	symax = bounds.maxY;
	szmin = bounds.minZ;
	szmax = bounds.maxZ;

    glm::mat4 projection = glm::perspective(45.0f, (float)width/(float)height, 1.0f, 1000.0f);

	glm::mat4 mv = 
		glm::translate(
				glm::rotate(
					glm::rotate(
						glm::translate(
							glm::mat4(1.0f),
							glm::vec3(0, 0, -pz)
							),
						ry, glm::vec3(1, 0, 0)
						),
					rx, glm::vec3(0, 1, 0)
					),
				glm::vec3(-(sxmin+sxmax)/2.0, -(symin+symax)/2.0, -(szmin+szmax)/2.0)
				);

	glm::mat4 mvp = projection*mv;

    glLoadMatrixf(glm::value_ptr(mvp));

	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	printNodes.push(mytree);
	while(true){
		int size = printNodes.size();
		for(int i = 0;i < size;i++) {
			TreeNode* node = printNodes.front();
			printNodes.pop();
			glVertex3f(node->param.branchEnd.x, node->param.branchEnd.y, node->param.branchEnd.z);
			if(node->parentNode != NULL){
				glVertex3f(node->parentNode->param.branchEnd.x, node->parentNode->param.branchEnd.y, node->parentNode->param.branchEnd.z);
			}else{
				glVertex3f(0.0f, 0.0f, 0.0f);
			}

			for(unsigned int j=0;j<node->childNodes.size();j++) {
				printNodes.push(node->childNodes[j]);
			}
		}

		if(size == 0) {
			break;
		}
	}
	glEnd();

    SDL_GL_SwapBuffers();
}

void onWindowResized(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w; height = h;
}

void onKeyDown(SDLKey key, Uint16 /*mod*/)
{
    switch(key) {
        case SDLK_ESCAPE : quit(); break;
//        case SDLK_3 : mode = 2; redraw(); break;
//        case SDLK_4 : mode = 3; redraw(); break;
        default : break;//nothing-doing defaut to shut up warning
    }
}

void onKeyUp(SDLKey /*key*/, Uint16 /*mod*/)
{
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
	int depth = 40;
	int seed = 23505;

	if(argc >= 2){
		stringstream seedss(argv[1]);
		seedss >> seed;
	}

	if(argc >= 3)
	{
		stringstream depths(argv[2]);
		depths >> depth;
	}

	mytree = GenTreeBillboardTexture_grammar("tree1", depth, seed);  // args: name, iteration, seed

	GenTreeBillboardTexture_parametrize(mytree, PTREE_1, seed);

	try {
		// Init SDL - only video subsystem will be used
		if(SDL_Init(SDL_INIT_VIDEO) < 0) throw SDL_Exception();

        // Shutdown SDL when program ends
        atexit(SDL_Quit);

        init(800, 600, 24, 24, 0);

        mainLoop();

    } catch(exception & ex) {
        cout << "ERROR : " << ex.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
