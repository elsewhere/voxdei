#ifndef _IMAGE_CPP_
#define _IMAGE_CPP_

//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <malloc.h>

#include "image.hpp"
//#include "png.h"

// T‰n kanssa oli ongelmia
// Linkkeriin piti laittaa optio: /NODEFAULTLIB:LIBC jotta ei tullut konflikteja funkkareiden kanssa
#include "jpeglib.h"

//--------------------------------------------------------------------------------------------
//  This code belongs to our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

namespace TRACTION_DEMOTRACTOR
{
	// Customized functions for loading JPG from a memory
	void initJPEGDataSource(j_decompress_ptr cinfo);
	unsigned char fillJPEGInputBuffer(j_decompress_ptr cinfo);
	void skipJPEGInputData(j_decompress_ptr cinfo, long nBytes);
	unsigned char resyncJPEGToRestart(j_decompress_ptr cinfo, int desired);
	void terminateJPEGSource(j_decompress_ptr cinfo);

	unsigned char *jpegData;
	unsigned int jpegSize;
};

//--------------------------------------------------------------------------------------------
//  Use our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

using namespace TRACTION_DEMOTRACTOR;

//--------------------------------------------------------------------------------------------
//  Class code
//--------------------------------------------------------------------------------------------

Image::Image(void)
{
	width=0;
	height=0;
	imagedata=NULL;	
	jpegData = NULL;
	jpegSize = 0;
}

Image::~Image(void)
{
	width=0;
	height=0;
	if(imagedata)
	{
		delete [] imagedata;
		imagedata = NULL;
	}	
}

//--------------------------------------------------------------------------------------------
//
// "LoadTGA(char *name)
// lataa 24bpp tai 32bpp RLE-pakatun tai pakkaamattoman TGA filen
//
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//
// "LoadPCX(char *name)
// lataa 8bpp tai 24bpp RLE-pakatun PCX tiedoston
//
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//
// "LoadJPG(char *name)
// lataa 24bpp JPG tiedoston
//
//--------------------------------------------------------------------------------------------

bool Image::LoadJPG(const char *name)
{
	unsigned char *ptr = NULL, *scanLine = NULL;
	struct jpeg_decompress_struct cinfo;
	JPGData *jpg = NULL;
	FILE *f;
	
	if(!(f = fopen(name, "rb")))	
	{		
		return false;
	}
	
	// JPEG error handle
	jpeg_error_mgr jerr;
	
	cinfo.err = jpeg_std_error(&jerr);
	
	// Inits
	jpeg_create_decompress(&cinfo);	
	jpeg_stdio_src(&cinfo, f);
		
	jpg = new JPGData;

	// Decode JPG
	jpeg_read_header(&cinfo, TRUE);		
	jpeg_start_decompress(&cinfo);
	jpg->row = cinfo.image_width * cinfo.num_components;
	jpg->xsize   = cinfo.image_width;
	jpg->ysize   = cinfo.image_height;
	
	jpg->data = new unsigned char[jpg->row * jpg->ysize];
		
	// Create an array of row pointers
	unsigned char** rowPtr = new unsigned char*[jpg->ysize];
	for (int i = 0; i < jpg->ysize; i++)
		rowPtr[i] = &(jpg->data[i*jpg->row]);
	
	int rowsRead = 0;
	while (cinfo.output_scanline < cinfo.output_height) 
	{		
		rowsRead += jpeg_read_scanlines(&cinfo, &rowPtr[rowsRead], cinfo.output_height - rowsRead);
	}
		
	delete [] rowPtr;

	jpeg_finish_decompress(&cinfo);

	// Release
	jpeg_destroy_decompress(&cinfo);
	
	width = jpg->xsize;
	height = jpg->ysize;	
	bpp = 32;	

	imagedata = new unsigned int[jpg->xsize*jpg->ysize*4];
	scanLine = new unsigned char[jpg->row];
	if(!imagedata)
	{
		return false;
	}		

	// 24bit JPEG
	ptr = (unsigned char *)imagedata;
	for(unsigned int c = 0; c < width*height*3; c+=3)
	{
		*ptr++ = jpg->data[c+0];
		*ptr++ = jpg->data[c+1];
		*ptr++ = jpg->data[c+2];
		*ptr++ = 255;
	}

	fclose(f);

	if(jpg)
	{
		if(jpg->data)
		{
			delete [] jpg->data;
			jpg->data = NULL;
		}

		delete jpg;	
		jpg = NULL;
	}

	if(scanLine)
	{
		delete [] scanLine;
		scanLine = NULL;
	}

	return true;
}

//--------------------------------------------------------------------------------------------
//
// "LoadMemoryJPG(char *name, unsigned char *fileData, unsigned int fileSize)
// lataa 24bpp JPG tiedston suoraan muistista
//
//--------------------------------------------------------------------------------------------
/*
bool Image::LoadMemoryJPG(char *name, unsigned char *fileData, unsigned int fileSize)
{
	struct jpeg_source_mgr mySourceManager;
	struct jpeg_error_mgr jpegErrorHandler;
	struct jpeg_decompress_struct cinfo;
	unsigned char *data = NULL, *ptr = NULL, *scanLine = NULL;
	unsigned int row;

	jpegData = fileData;
	jpegSize = fileSize;

	// Virheiden k‰sittelij‰
	cinfo.err = jpeg_std_error(&jpegErrorHandler);

	// Initti	
	jpeg_create_decompress(&cinfo);

	// T‰ytet‰‰n rakenteet k‰sin customointia varten:
	//
	// Kerrotaan ett‰ k‰ytet‰‰n omaa manageria lukemiseen, jotta saadaan
	// shitti ulos muistista eik‰ I/O streamist‰
	cinfo.src = &mySourceManager;
	mySourceManager.bytes_in_buffer = fileSize;
	mySourceManager.next_input_byte = fileData;

	// Funktio-osoittimet omiin funkkareihin	
	mySourceManager.init_source = initJPEGDataSource;
	mySourceManager.fill_input_buffer = fillJPEGInputBuffer;
	mySourceManager.skip_input_data = skipJPEGInputData;
	mySourceManager.resync_to_restart = resyncJPEGToRestart;
	mySourceManager.term_source = terminateJPEGSource;	

	// Decode JPG
	jpeg_read_header(&cinfo, TRUE);		
	jpeg_start_decompress(&cinfo);

	row = cinfo.image_width * cinfo.num_components;
	width = cinfo.image_width;
	height = cinfo.image_height;
	bpp = 32;
		
	data = new unsigned char[height*row];
	if(!data)
	{
		dmsMsg("LoadMemoryJPG() error: Couldn't allocate memory\n");
		return false;
	}

	// Create an array of row pointers
	unsigned char** rowPtr = new unsigned char*[height];
	for (unsigned int i = 0; i < height; i++)
		rowPtr[i] = &(data[i*row]);
	
	int rowsRead = 0;
	while (cinfo.output_scanline < cinfo.output_height) 
	{		
		rowsRead += jpeg_read_scanlines(&cinfo, &rowPtr[rowsRead], cinfo.output_height - rowsRead);
	}
		
	delete [] rowPtr;

	jpeg_finish_decompress(&cinfo);

	jpeg_destroy_decompress(&cinfo);	

	// Fill in our imagedata
	imagedata = new unsigned int[width*height*4];
	scanLine = new unsigned char[row];
	if(!imagedata)
	{
		return false;
	}		

	// 24bit JPEG
	ptr = (unsigned char *)imagedata;
	for(unsigned int c = 0; c < width*height*3; c+=3)
	{
		*ptr++ = data[c+0];
		*ptr++ = data[c+1];
		*ptr++ = data[c+2];
		*ptr++ = 255;
	}

	if(data)
	{
		delete [] data;	
		data = NULL;
	}

	if(scanLine)
	{
		delete [] scanLine;
		scanLine = NULL;
	}

	return true;
}
*/
/*
bool Image::LoadPNG(char *name)
{
	return true;
}
*/
bool Image::set(int w, int h, int b)
{
	width = w;
	height = h;
	bpp = b;

	imagedata = new unsigned int[w*h];
	if(!imagedata)
	{	
		return false;
	}

	memset(imagedata, 0, w*h*4);

	return true;
}

unsigned int Image::GetWidth(void)
{
	return width;
}

unsigned int Image::GetHeight(void)
{
	return height;
}

unsigned int *Image::GetPtr(void)
{
	return imagedata;
}

void Image::SetPtr(unsigned int *b)
{
	memcpy(imagedata, b, width*height*4);	
}


unsigned int Image::getType(void)
{
	return type;
}

unsigned int Image::getBpp(void)
{
	return bpp;
}

float Image::lum(unsigned int x, unsigned int y)
{
	int r, g, b;
	if((x<0) || (x>=width) || (y<0) || (y>=height)) return 0;

	r = imagedata[(y*width+x)];
	g = imagedata[(y*width+x)];
	b = imagedata[(y*width+x)];
	//r = imagedata[3*(y*width+x)];
	//g = imagedata[3*(y*width+x)+1];
	//b = imagedata[3*(y*width+x)+2];
	return float(sqrt(r*r+g*g+b*b) / sqrt(3));
}

void Image::release(void)
{	
	if (imagedata != NULL) 
	{
		delete [] imagedata;
		imagedata = NULL;
	}
	
}

//--------------------------------------------------------------------------------------------
//  This code belongs to our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------
//
// Kustomoidut rutiinit JPEG:n lataamiseksi muistista
// 
//--------------------------------------------------------------------------------------------

void initJPEGDataSource(j_decompress_ptr cinfo)
{
}

unsigned char fillJPEGInputBuffer(j_decompress_ptr cinfo)
{
	cinfo->src->next_input_byte	= jpegData;
	cinfo->src->bytes_in_buffer	= jpegSize;

	return true;
}

void skipJPEGInputData(j_decompress_ptr cinfo, long nBytes)
{
	cinfo->src->next_input_byte	+= nBytes;
	cinfo->src->bytes_in_buffer	-= nBytes;
}

unsigned char resyncJPEGToRestart(j_decompress_ptr cinfo, int desired)
{
	jpeg_resync_to_restart(cinfo, desired);

	return true;
}

void terminateJPEGSource(j_decompress_ptr cinfo)
{
}

#endif
