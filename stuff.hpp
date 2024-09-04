#ifndef _STUFF_HPP_
#define _STUFF_HPP_


#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include "win32.hpp"

#include "vector.hpp"
#include "matrix.hpp"
#include "matikka.h"
#include "primitives.hpp"
#include "texture.hpp"
#include "font.hpp"
#include "sync.hpp"
#include "timer.hpp"
#include "sound.hpp"
#include "config.hpp"
#include "ext.hpp"
#include "shader.hpp" 
#include "mesh.hpp"
#include "catmullrom.hpp"
#include "bezier.hpp"
#include "camera.hpp"
#include "lista.hpp"

#include "postproc.hpp"

class Ext;
struct shader;

extern Window win;
extern Ext glExt;

extern shader	*darken;
extern shader	*glowc;
extern shader	*blurx;
extern shader	*blury;
extern shader	*blur2x;
extern shader	*blur2y;
extern shader   *displace;

extern Texture *render512_0;
extern Texture *render512_1;
extern Texture *box_texture;
extern Texture *circle_texture;
extern Texture *circle2_texture;
extern Texture *circle3_texture;
extern Texture *pillars_texture;
extern Texture *particle_texture;
extern Texture *particle2_texture;
extern Texture *ring_texture;
extern Texture *noise_texture;
extern Texture *render256;
extern Texture *render512;

extern class CameraHolder *cam;
extern Timer kello;
extern FILE *loki; 

extern Vector3 color1;
extern Vector3 color2;
extern Vector3 color3;


extern int FSAA;
extern int OPENGL_XRES;
extern int OPENGL_YRES;
extern bool OPENGL_FULLSCREEN;
extern bool runflag;
extern bool musicflag;
extern bool spookyflag;

extern Vector4 clearcolor;
extern int originalaika;

//interpoloi, palauttaa 0 jos value1 < limit1, palauttaa 0..1 jos välissä, 1 jos menee yli
float calcPosInt(int value1, int limit1, int limit2);

//interpoloi, palauttaa 0 jos value1 < limit1, palauttaa 0..1 jos välissä, 1 jos menee yli
float calcPosFloat(float value1, float limit1, float limit2);

//saa maksimin puolivälissä (limit1+limit2)/2, multiplyllä kerrotaan, paluuarvo klipataan [0..1]
float calcSaturate(float value, float limit1, float limit2, float multiply);

const bool testaaPos(const int aika, const int ajat[]);
const float laskePos(const int aika, const int ajat[]);

void perspective2D(float width, float height);
void perspective3D();

void textures_init();
void textures_free();
void unpack(unsigned char *source, unsigned long *dest);

#endif