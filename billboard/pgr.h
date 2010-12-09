#ifndef PGR_H
#define PGR_H

#include <string>
#include <fstream>
#include <iterator>
#include <exception>
#include <stdexcept>
#include <iostream>

#include <SDL/SDL.h>

#include <GL/GLee.h>
#include <GL/glu.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Exception classes
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct SDL_Exception : public std::runtime_error
{
    SDL_Exception() throw() : std::runtime_error(std::string("SDL : ") + SDL_GetError()) {}
};

struct GL_Exception : public std::runtime_error
{
    GL_Exception(const GLenum error = glGetError()) throw()
        : std::runtime_error(std::string("OpenGL : ") + (const char*)gluErrorString(error)) {}
    GL_Exception(const char * text, const GLenum error = glGetError()) throw()
        : std::runtime_error(std::string("OpenGL : ") + text + " - " + (const char*)gluErrorString(error)) {}
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Utility functions
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Send quit event
inline void quit()
{
    SDL_Event event;
    event.type = SDL_QUIT;
    if(SDL_PushEvent(&event) < 0) throw SDL_Exception();
}

// Send redraw event
inline void redraw()
{
    SDL_Event event;
    event.type = SDL_VIDEOEXPOSE;
    if(SDL_PushEvent(&event) < 0) throw SDL_Exception();
}

// Load whole file and return it as std::string
std::string loadFile(const char * const file);

// Info log contains errors and warnings from shader compilation
std::string getShaderInfoLog(const GLuint shader);

// Info log contains errors and warnings from shader linking
std::string getProgramInfoLog(const GLuint program);

GLuint CompileShader(const GLenum type, const char * source);

GLuint LinkShader(size_t count, ...);

void SurfaceImage2D(GLenum target, GLint level, GLint internalformat, SDL_Surface * surface);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Event handlers
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void onInit();
void onWindowRedraw();
void onWindowResized(int width, int height);
void onKeyDown(SDLKey key, Uint16 mod);
void onKeyUp(SDLKey key, Uint16 mod);
void onMouseMove(unsigned x, unsigned y, int xrel, int yrel, Uint8 buttons);
void onMouseDown(Uint8 button, unsigned x, unsigned y);
void onMouseUp(Uint8 button, unsigned x, unsigned y);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Initialization + event loop
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SDL_Surface * init(unsigned width, unsigned height, unsigned color, unsigned depth, unsigned stencil);

// Simple main loop
void mainLoop();

// Animation main loop
// period - rough time between redraws in ms
void mainLoop(unsigned period);

#undef main

#endif
