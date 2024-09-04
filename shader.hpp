#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include "windows.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "glext.h"

#include "ext.hpp"
#include "stuff.hpp"

// -----------------------
// Single shader structure
// -----------------------
struct shader {

	GLhandleARB vertex;
	GLhandleARB fragment;
	// edelliset yhdes
	GLhandleARB program;
};

void debug_shader(GLuint obj);
void init_shader(shader *s, const char *frag, const char *vert);
void free_shader(shader *s);
GLint GetUniLoc(shader *s, const GLcharARB *name);

// -----------------------
//  ADD YOUR SHADERS HERE
// -----------------------

void shaders_init();
void shaders_free();



#endif