#ifndef _POSTPROC_HPP_
#define _POSTPROC_HPP_

#include "stuff.hpp"

extern class PostProc filter;

void persp2D(int w, int h);
void persp3D();

class PostProc
{
public:

	// Call this before rendering stuff that needs postprocession
	void init(bool clear=true);
	
	// Call this when you want render the glow the screen [darken_alpha -1 = no darken at all]
	void glow(int blur_steps=5, float blur_step_x=0.006f, float blur_step_y=0.006f, float blur_brightness=1.0f, float darken_alpha=-1.0f, float glow_amount=0.50f);

	// Renders blurred scene with given parameters
	void blur(int blur_steps=5, float blur_step_x=0.006f, float blur_step_y=0.006f);

	// Draws overlay on top of everything
	void drawOverlay(GLuint &texid);

	void draw_grid(Texture *t, float alpha, float mx, float my, float r=1.0f, float g=1.0f, float b=1.0f, 
		float scale=1.0f, int type=0);

protected:
};


#endif