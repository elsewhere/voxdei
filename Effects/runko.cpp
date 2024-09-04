#include <stdio.h>
#include "runko.hpp"

void runko(float aika)
{
	const float alpha = calcSaturate(aika, 0, 1, 6);

	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glLoadIdentity();

}
	
void runko_init()
{
}

void runko_free()
{
}