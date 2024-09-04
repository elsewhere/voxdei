#ifndef _IMAGE_HPP_
#define _IMAGE_HPP_


/*
	class PCXHeader
	{
		public:
			unsigned char manufacturer;
			unsigned char version;        // 5 = version 3.0
			unsigned char encoding;       // 1 = RLE 
			unsigned char bits_per_plane; // per plane                    
			unsigned short xmin,ymin;                                      
			unsigned short xmax,ymax;                                      
			unsigned short hres;
			unsigned short vres;                                           
			unsigned char palette16[48];  // only used for 16colors       
			unsigned char reserved;                                       
			unsigned char color_planes;                                   
			unsigned short bytes_per_line;                                 
			unsigned short palette_type;
			unsigned char filler[58];
	};

	class PCXPaletteEntry
	{
		public:
			unsigned char R, G, B;
	};

	class TGAHeader
	{
		public:
			unsigned char Header[12];
	};

	class TGAData
	{
		public:
			unsigned char header[6];
			unsigned int bytesPerPixel;
			unsigned int imagesize;
			unsigned int type;
			unsigned int height;
			unsigned int width;
			unsigned int bpp;
	};
*/
//--------------------------------------------------------------------------------------------
//  JPEG class
//--------------------------------------------------------------------------------------------

class JPGData
{
	public:
		int row;
		int xsize;
		int	ysize;
		unsigned char *data;
};


class Image
{
public:

	friend class Texture;

	Image(void);
	~Image(void);

//	bool LoadPCX(char *name);
//	bool LoadTGA(char *name);				
//	bool LoadPNG(char *name);
	bool LoadJPG(const char *name);
//	bool LoadMemoryPCX(char *name, unsigned char *fileData, unsigned int fileSize);
//	bool LoadMemoryTGA(char *name, unsigned char *fileData, unsigned int fileSize);
//	bool LoadMemoryJPG(char *name, unsigned char *fileData, unsigned int fileSize);
	float lum(unsigned int x, unsigned int y);
	void release(void);

	bool set(int w, int h, int b);

	unsigned int GetWidth(void);
	unsigned int GetHeight(void);
	unsigned int *GetPtr(void);
	void SetPtr(unsigned int *b);
	unsigned int getType(void);
	unsigned int getBpp(void);

private:					

	unsigned int *imagedata;
	unsigned int bpp;
	unsigned int width, height;
	unsigned int ID, type;
};

#endif
