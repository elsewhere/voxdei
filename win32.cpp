#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "glext.h"
#include "win32.hpp"

extern int OPENGL_XRES;
extern int OPENGL_YRES;
extern bool OPENGL_FULLSCREEN;

// FSAA;
// http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=46
// http://www.codeproject.com/opengl/starwars.asp

// Definitions for Multisampling 
typedef const char * (WINAPI * PFNWGLGETEXTENSIONSSTRINGARBPROC) (HDC hdc);
typedef BOOL (WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

#define WGL_NUMBER_PIXEL_FORMATS_ARB   0x2000
#define WGL_DRAW_TO_WINDOW_ARB         0x2001
#define WGL_DRAW_TO_BITMAP_ARB         0x2002
#define WGL_ACCELERATION_ARB           0x2003
#define WGL_NEED_PALETTE_ARB           0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB    0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB     0x2006
#define WGL_SWAP_METHOD_ARB            0x2007
#define WGL_NUMBER_OVERLAYS_ARB        0x2008
#define WGL_NUMBER_UNDERLAYS_ARB       0x2009
#define WGL_TRANSPARENT_ARB            0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB  0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB 0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB 0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB 0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB 0x203B
#define WGL_SHARE_DEPTH_ARB            0x200C
#define WGL_SHARE_STENCIL_ARB          0x200D
#define WGL_SHARE_ACCUM_ARB            0x200E
#define WGL_SUPPORT_GDI_ARB            0x200F
#define WGL_SUPPORT_OPENGL_ARB         0x2010
#define WGL_DOUBLE_BUFFER_ARB          0x2011
#define WGL_STEREO_ARB                 0x2012
#define WGL_PIXEL_TYPE_ARB             0x2013
#define WGL_COLOR_BITS_ARB             0x2014
#define WGL_RED_BITS_ARB               0x2015
#define WGL_RED_SHIFT_ARB              0x2016
#define WGL_GREEN_BITS_ARB             0x2017
#define WGL_GREEN_SHIFT_ARB            0x2018
#define WGL_BLUE_BITS_ARB              0x2019
#define WGL_BLUE_SHIFT_ARB             0x201A
#define WGL_ALPHA_BITS_ARB             0x201B
#define WGL_ALPHA_SHIFT_ARB            0x201C
#define WGL_ACCUM_BITS_ARB             0x201D
#define WGL_ACCUM_RED_BITS_ARB         0x201E
#define WGL_ACCUM_GREEN_BITS_ARB       0x201F
#define WGL_ACCUM_BLUE_BITS_ARB        0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB       0x2021
#define WGL_DEPTH_BITS_ARB             0x2022
#define WGL_STENCIL_BITS_ARB           0x2023
#define WGL_AUX_BUFFERS_ARB            0x2024
#define WGL_NO_ACCELERATION_ARB        0x2025
#define WGL_GENERIC_ACCELERATION_ARB   0x2026
#define WGL_FULL_ACCELERATION_ARB      0x2027
#define WGL_SWAP_EXCHANGE_ARB          0x2028
#define WGL_SWAP_COPY_ARB              0x2029
#define WGL_SWAP_UNDEFINED_ARB         0x202A
#define WGL_TYPE_RGBA_ARB              0x202B
#define WGL_TYPE_COLORINDEX_ARB        0x202C

#define GL_MULTISAMPLE_ARB                0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE_ARB   0x809E
#define GL_SAMPLE_ALPHA_TO_ONE_ARB        0x809F
#define GL_SAMPLE_COVERAGE_ARB            0x80A0
#define GL_SAMPLE_BUFFERS_ARB             0x80A8
#define GL_SAMPLES_ARB                    0x80A9
#define GL_SAMPLE_COVERAGE_VALUE_ARB      0x80AA
#define GL_SAMPLE_COVERAGE_INVERT_ARB     0x80AB
#define GL_MULTISAMPLE_BIT_ARB            0x20000000

/* NV_multisample_filter_hint */
#define GL_MULTISAMPLE_FILTER_HINT_NV     0x8534

#define WGL_SAMPLE_BUFFERS_ARB		 0x2041
#define WGL_SAMPLES_ARB			     0x2042



// next following 2 functions are copypasted from
// codesampler

bool extExist(const char *extension)
{
  // try to find extension from wgl/gl extensions lists
  PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = 0;
  wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
  if(wglGetExtensionsStringARB)
  {
	const char *winsys_extensions = wglGetExtensionsStringARB(wglGetCurrentDC());
	if (winsys_extensions && (strstr(winsys_extensions,extension)))
	  return true;
  }
  const char *glExtensions = (const char*)glGetString(GL_EXTENSIONS);
  if (!glExtensions)
	return false;

  return (strstr(glExtensions,extension) != NULL);
}

int  Window::ValidatePixelFormat(HDC hdc,int suggestedFormat, int requestedSamples)
{
	// PRE: requestedSamples = 0,2,4,8

  if(requestedSamples==0) 
	  return suggestedFormat;

  HDC hDC = wglGetCurrentDC();

  PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = 
	  (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");	
  if (!wglChoosePixelFormatARB)
	return suggestedFormat;

  if (!extExist("WGL_ARB_multisample"))
	return suggestedFormat;

  int pixelFormat;
  BOOL bStatus;
  UINT numFormats;
  float fAttributes[] = {0,0};
  int iAttributes[] = { 
	WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
	WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
	WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
	WGL_COLOR_BITS_ARB,24,
	WGL_ALPHA_BITS_ARB,8,
	WGL_DEPTH_BITS_ARB,16,
	WGL_STENCIL_BITS_ARB,0,
	WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
	WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
	WGL_SAMPLES_ARB,requestedSamples,
	0,0};
  bStatus = wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats);
  if ((bStatus == GL_TRUE) && (numFormats == 1))
  {
	return pixelFormat;	  
  }

  // ok that failed, try using 2 samples now instead of 4
  iAttributes[19] = 2;
  bStatus = wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats);
  if ((bStatus == GL_TRUE) && (numFormats == 1))
  {
	return pixelFormat;	  
  }

  // failed, return the suggested format and continue
  return suggestedFormat;
}


LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

Window::Window()
{
}

Window::~Window()
{
}

bool Window::killWindow() { return 0; }
bool Window::initWindow() { return 0; }

bool Window::create(bool bflag, int fsaa)
{

	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;	

    WNDCLASS wc;
		
	hInstance = GetModuleHandle(NULL);
	
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "OpenGL";

	if(!RegisterClass(&wc)) return false;

	RtlZeroMemory(&screenSettings, sizeof(screenSettings));
	p.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	p.nVersion = 1;
	p.dwFlags=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
	p.iPixelType=PFD_TYPE_RGBA;
	p.cColorBits=32;
	p.cDepthBits=16;
	p.iLayerType=PFD_MAIN_PLANE;

	RtlZeroMemory(&screenSettings, sizeof(screenSettings));
	screenSettings.dmSize = sizeof(screenSettings);
	screenSettings.dmPelsWidth = OPENGL_XRES;
	screenSettings.dmPelsHeight = OPENGL_YRES;
	screenSettings.dmBitsPerPel = 32;
	screenSettings.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
	
	//---------------------
	// DUMMY FSAA WINDOW
	// ikkuna ja ogl auki 
	// ja haetaan tuettu 
	// fsaa moodi (max 4)
	//---------------------

	HWND hStupid =  CreateWindowEx(dwExStyle, "OpenGL", "Dummy FSAA window", dwStyle, 0, 0, 1, 1, NULL, NULL, hInstance, NULL);

	if (!hStupid)
		return NULL;

	  // opengl
	HDC hDC = GetDC(hStupid);
	HGLRC hRC;
	p.dwFlags |= (PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER);
	int requestedFormat;
	int format = ChoosePixelFormat( hDC, &p );
    if (format == 0 || 
		(SetPixelFormat( hDC, format, &p ) == FALSE) || 
		((hRC = wglCreateContext(hDC)) == NULL)
		)
	{
		ReleaseDC(hStupid,hDC);
		DestroyWindow(hStupid);
		return NULL;
	}

	// get pixelformat
	wglMakeCurrent(hDC,hRC);
	requestedFormat = ValidatePixelFormat(hDC,format, fsaa);

	// kill kill dummy dummy 
	wglMakeCurrent(NULL,NULL);
	ReleaseDC(hStupid,hDC);
	wglDeleteContext(hRC);
	DestroyWindow(hStupid);


	// ikkuna vai fullscreen moodi
	if(bflag)
	{
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	}
	if (OPENGL_FULLSCREEN)
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= OPENGL_XRES;			// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= OPENGL_YRES;			// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= 32;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
			return false;

		dwExStyle=WS_EX_APPWINDOW;					// Window Extended Style
		dwStyle=WS_POPUP;							// Windows Style
	}

	// oikea ikkuna
	hwnd = CreateWindowEx(dwExStyle, "OpenGL", "Traction :: Vox Dei", dwStyle, 0, 0, 
						  OPENGL_XRES, OPENGL_YRES, NULL, NULL, hInstance, NULL);
	hdc = GetDC(hwnd);
	
	// set FSAA or normal pixelformat
	pixelformat = ChoosePixelFormat(hdc, &p);
	SetPixelFormat(hdc, requestedFormat, &p);
			
	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	if (OPENGL_FULLSCREEN)
		ShowCursor(FALSE);			// Hide Mouse Pointer
	else
		ShowCursor(true);

	//---------------------
	// vsync extension
	//---------------------
	typedef void (APIENTRY *PFNWGLEXTSWAPCONTROLPROC) (int);
	typedef int (*PFNWGLEXTGETSWAPINTERVALPROC) (void);
	PFNWGLEXTSWAPCONTROLPROC wglSwapIntervalEXT = NULL;
	PFNWGLEXTGETSWAPINTERVALPROC wglGetSwapIntervalEXT = NULL;

   if (strstr((char*)glGetString(GL_EXTENSIONS),"WGL_EXT_swap_control"))
   {
      wglSwapIntervalEXT = (PFNWGLEXTSWAPCONTROLPROC) wglGetProcAddress("wglSwapIntervalEXT");
      wglGetSwapIntervalEXT = (PFNWGLEXTGETSWAPINTERVALPROC) wglGetProcAddress("wglGetSwapIntervalEXT");
	  
	  if(wglSwapIntervalEXT) { 
		  wglSwapIntervalEXT(1); // enable vsync
		  //MessageBox(win.getHWND(),"Vsync enabled","VSYNC!",MB_OK);
	  }
   } else {
	  MessageBox(NULL,"Failed to enable VSYNC.","VSYNC!",MB_TOPMOST|MB_ICONERROR);
   }

	ShowWindow(this->hwnd, SW_SHOW);			// iCmdShow kertoo ikkunan tilasta (koot yms);	
	SetForegroundWindow(hwnd);			// aseta päälimmäiseksi ikkunaksi, lisäämällä sen prioriteettia
	SetFocus(hwnd);						// asettaa näppiksen keskittymään tähän ikkunaan

	return true;
}

void Window::shut()
{
	ChangeDisplaySettings(NULL, 0);
	ReleaseDC(hwnd, hdc);
	DestroyWindow(hwnd);
}

HWND Window::getHWND()
{
	return hwnd;
}


HDC Window::getDC()
{
	return hdc;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{			
	switch(iMsg)	// Ikkuna viestit
	{
		case WM_SYSCOMMAND:						// systeemi käskyt
		{
			switch (wParam)						// tarkistetaan
			{
				case SC_SCREENSAVE:				// yrittääkö ruudunsäästäjä käynnistyä
				case SC_MONITORPOWER:			// monitori yrittää säästää virtaa
				return 0;						// ei saat tapahtua
			}		
			break;									
		} break;

		case WM_CLOSE:		// Ikkuna on suljettu -> tapa se!!!
		{
				PostQuitMessage(0);
				return 0;
		} break;		
	}	

	// ei käsitellyt viestit hoitaa wintoosa... great X)
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
