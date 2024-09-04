#ifndef _WIN32_HPP_
#define _WIN32_HPP_

class Window
{
	public:

		Window();
		~Window();

		bool create(bool bflag, int fsaa);
		void shut();
		HDC getDC();
		HWND getHWND();

	protected:

		int  ValidatePixelFormat(HDC hdc,int suggestedFormat, int requestedSamples);
		bool killWindow();
		bool initWindow();

	private:

		HWND hwnd;
		HDC hdc;
		HGLRC hrc;
		PIXELFORMATDESCRIPTOR p;
		int pixelformat;

		DEVMODE screenSettings;
		HINSTANCE hInstance;
};

#endif