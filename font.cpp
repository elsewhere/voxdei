#include "font.hpp"

GLuint fontbase;
GLuint fontbase2;
GLYPHMETRICSFLOAT gmf[256];

void debugprint(char *string, int line)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glTranslatef(0, 0, -5.0f);
	font_write(-2.7f, 1.45f-line*0.35f, string);
}


void font_write(float xpos, float ypos, const char *string)
{
	glRasterPos2f(xpos, ypos);
	glPushAttrib(GL_LIST_BIT);  //**TURHA? jos muita display listoja ei käytetä... 
	glListBase(fontbase-32);
	glCallLists(strlen(string), GL_UNSIGNED_BYTE, string);
	glPopAttrib();
}

void font_write3d(const char *string)
{
	glPushAttrib(GL_LIST_BIT);
	glListBase(fontbase2);
	glCallLists(strlen(string), GL_UNSIGNED_BYTE, string);
	glPopAttrib();
}

void font_init()
{
	HFONT font, font2;
	HFONT oldfont;

	fontbase = glGenLists(96);

	font = CreateFont((int)(-60*OPENGL_XRES/1024.0f), //height
						0, //width
						0, //jotain kulmia, skippaa
						0, 
						FW_HEAVY, //font weight 0-1000
						FALSE,	//italic
						FALSE,	//underline
						FALSE,	//strikeout
						//ANSI_CHARSET,
						DEFAULT_CHARSET,
						OUT_TT_PRECIS, //output precision
						CLIP_DEFAULT_PRECIS,
						ANTIALIASED_QUALITY,
						FF_DONTCARE|DEFAULT_PITCH,
						"Trebuchet MS"
						);

	HDC myHDC = win.getDC();

	oldfont = (HFONT)SelectObject(myHDC, font);           // Selects The Font We Want
	wglUseFontBitmaps(myHDC, 32, 96, fontbase);				// Builds 96 Characters Starting At Character 32
	SelectObject(myHDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font

	fontbase2 = glGenLists(256);
	font2 = CreateFont(	-160, 
						0,
						0, 
						0,
						FW_BOLD, 
						FALSE, 
						FALSE, 
						FALSE, 
						ANSI_CHARSET, 
						OUT_TT_PRECIS,
						CLIP_DEFAULT_PRECIS,
						ANTIALIASED_QUALITY,
						FF_DONTCARE|DEFAULT_PITCH,
						"Tahoma");

	SelectObject(myHDC, font2);
	wglUseFontOutlines(	myHDC,				// Select The Current DC
				0,				// Starting Character
				255,				// Number Of Display Lists To Build
				fontbase2,				// Starting Display Lists
				0.0f,				// Deviation From The True Outlines
				0.0f,				// Font Thickness In The Z Direction
				WGL_FONT_POLYGONS,		// Use Polygons, Not Lines
				gmf);				// Address Of Buffer To Recieve Data
}


void font_free()
{
	glDeleteLists(fontbase, 96);
	glDeleteLists(fontbase2, 256);
}