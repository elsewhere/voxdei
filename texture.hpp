#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

#include "image.hpp"

//--------------------------------------------------------------------------------------------
//  This code belongs to our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------
//  Texture class
//--------------------------------------------------------------------------------------------

class Texture
{
	public:


		Texture();
		~Texture();

		bool loadJPG(const char *name);
//		bool loadJPGFromMemory(char *name, unsigned char *fileData, unsigned int fileSize);

		void setID(int i);

		int getID();
		unsigned int getWidth();
		unsigned int getHeight();
		unsigned int getType();
		unsigned int *getImgData();
        char *getName();
        void setName(const char *name);

	private:
        char *filename;

		unsigned int ID;
		unsigned int width, height;
		unsigned int bpp, type;
		Image image;
};

#endif