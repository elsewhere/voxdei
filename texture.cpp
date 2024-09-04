#include <windows.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "texture.hpp"

Texture::Texture()
{
	width = 0;
	height = 0;
	bpp = 0;
	ID = 0;
}

Texture::~Texture()
{
    delete [] filename;
	image.release();
}
bool Texture::loadJPG(const char *name)
{
	if(!image.LoadJPG(name))
	{		
		return false;
	}

    int pituus = strlen(name);
    filename = new char[pituus+1];
    strcpy(filename, name);

	width = image.GetWidth();
	height = image.GetHeight();
	type = image.getType();

    glGenTextures(1, &this->ID);

    glBindTexture(GL_TEXTURE_2D, this->ID);
	if((width == 512 && height == 512) || 
       (width == 256 && height == 256) || 
       (width == 128 && height == 128) || 
       (width == 64 && height == 64))
	{										
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		// tyyppi RGBA, eli oletetaan että kaikki textuurit ovat 32bit
		glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.GetPtr());//getImgData());		
	}
	else
	{
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// tyyppi RGBA, eli oletetaan että kaikki textuurit ovat 32bit
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image.GetPtr());		
	}
	return true;
}
void Texture::setID(int i)
{
	ID = i;
}

int Texture::getID()
{
	return ID;
}

unsigned int Texture::getHeight()
{
	return height;
}

unsigned int Texture::getWidth()
{
	return width;
}

unsigned int Texture::getType()
{
	return type;
}

unsigned int *Texture::getImgData()
{
	return image.GetPtr();
}
char *Texture::getName()
{
	return filename;
}
void Texture::setName(const char *name)
{
    if (filename)
    {
        delete[] filename;
    }
    int pituus = strlen(name);
    filename = new char[pituus+1];
    strcpy(filename, name);
}