// BASS spectrum analyser example, copyright (c) 2002-2006 Ian Luck.

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "bass.h"

#define SPECWIDTH 368	// display width
#define SPECHEIGHT 127	// height (changing requires palette adjustments too)

HWND win=NULL;
DWORD timer=0;

DWORD chan;

HDC specdc=0;
HBITMAP specbmp=0;
BYTE *specbuf;

int specmode=0,specpos=0; // spectrum mode (and marker pos for 2nd mode)

/* display error messages */
void Error(const char *es)
{
	char mes[200];
	sprintf(mes,"%s\n(error code: %d)",es,BASS_ErrorGetCode());
	MessageBox(win,mes,0,0);
}

/* select a file to play, and play it */
BOOL PlayFile()
{
	char file[MAX_PATH]="";
	OPENFILENAME ofn={0};
	ofn.lStructSize=sizeof(ofn);
	ofn.hwndOwner=win;
	ofn.nMaxFile=MAX_PATH;
	ofn.lpstrFile=file;
	ofn.Flags=OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_EXPLORER;
	ofn.lpstrTitle="Select a file to play";
	ofn.lpstrFilter="playable files\0*.mo3;*.xm;*.mod;*.s3m;*.it;*.mtm;*.umx;*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif\0All files\0*.*\0\0";
	if (!GetOpenFileName(&ofn)) return FALSE;

	if (!(chan=BASS_StreamCreateFile(FALSE,file,0,0,BASS_SAMPLE_LOOP))
		&& !(chan=BASS_MusicLoad(FALSE,file,0,0,BASS_MUSIC_RAMP|BASS_SAMPLE_LOOP,0))) {
		Error("Can't play file");
		return FALSE; // Can't load the file
	}

	BASS_ChannelPlay(chan,FALSE);

	return TRUE;
}

/* update the spectrum display - the interesting bit :) */
void CALLBACK UpdateSpectrum(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	HDC dc;
	int x,y,y1;

	if (specmode==3) { // waveform
		int c;
		float *buf;
		BASS_CHANNELINFO ci;
		memset(specbuf,0,SPECWIDTH*SPECHEIGHT);
		BASS_ChannelGetInfo(chan,&ci); // get number of channels
		buf=alloca(ci.chans*SPECWIDTH*sizeof(float)); // allocate buffer for data
		BASS_ChannelGetData(chan,buf,(ci.chans*SPECWIDTH*sizeof(float))|BASS_DATA_FLOAT); // get the sample data (floating-point to avoid 8 & 16 bit processing)
		for (c=0;c<ci.chans;c++) {
			for (x=0;x<SPECWIDTH;x++) {
				int v=(1-buf[x*ci.chans+c])*SPECHEIGHT/2; // invert and scale to fit display
				if (v<0) v=0;
				else if (v>=SPECHEIGHT) v=SPECHEIGHT-1;
				if (!x) y=v;
				do { // draw line from previous sample...
					if (y<v) y++;
					else if (y>v) y--;
					specbuf[y*SPECWIDTH+x]=c&1?127:1; // left=green, right=red (could add more colours to palette for more chans)
				} while (y!=v);
			}
		}
	} else {
		float fft[1024];
		BASS_ChannelGetData(chan,fft,BASS_DATA_FFT2048); // get the FFT data

		if (!specmode) { // "normal" FFT
			memset(specbuf,0,SPECWIDTH*SPECHEIGHT);
			for (x=0;x<SPECWIDTH/2;x++) {
#if 1
				y=sqrt(fft[x+1])*3*SPECHEIGHT-4; // scale it (sqrt to make low values more visible)
#else
				y=fft[x+1]*10*SPECHEIGHT; // scale it (linearly)
#endif
				if (y>SPECHEIGHT) y=SPECHEIGHT; // cap it
				if (x && (y1=(y+y1)/2)) // interpolate from previous to make the display smoother
					while (--y1>=0) specbuf[y1*SPECWIDTH+x*2-1]=y1+1;
				y1=y;
				while (--y>=0) specbuf[y*SPECWIDTH+x*2]=y+1; // draw level
			}
		} else if (specmode==1) { // logarithmic, acumulate & average bins
			int b0=0;
			memset(specbuf,0,SPECWIDTH*SPECHEIGHT);
#define BANDS 28
			for (x=0;x<BANDS;x++) {
				float sum=0;
				int sc,b1=pow(2,x*10.0/(BANDS-1));
				if (b1>1023) b1=1023;
				if (b1<=b0) b1=b0+1; // make sure it uses at least 1 FFT bin
				sc=10+b1-b0;
				for (;b0<b1;b0++) sum+=fft[1+b0];
				y=(sqrt(sum/log10(sc))*1.7*SPECHEIGHT)-4; // scale it
				if (y>SPECHEIGHT) y=SPECHEIGHT; // cap it
				while (--y>=0)
					memset(specbuf+y*SPECWIDTH+x*(SPECWIDTH/BANDS),y+1,SPECWIDTH/BANDS-2); // draw bar
			}
		} else { // "3D"
			for (x=0;x<SPECHEIGHT;x++) {
				y=sqrt(fft[x+1])*3*127; // scale it (sqrt to make low values more visible)
				if (y>127) y=127; // cap it
				specbuf[x*SPECWIDTH+specpos]=128+y; // plot it
			}
			// move marker onto next position
			specpos=(specpos+1)%SPECWIDTH;
			for (x=0;x<SPECHEIGHT;x++) specbuf[x*SPECWIDTH+specpos]=255;
		}
	}

	// update the display
	dc=GetDC(win);
	BitBlt(dc,0,0,SPECWIDTH,SPECHEIGHT,specdc,0,0,SRCCOPY);
	ReleaseDC(win,dc);
}

/* window procedure */
long FAR PASCAL SpectrumWindowProc(HWND h, UINT m, WPARAM w, LPARAM l)
{
	switch (m) {
		case WM_PAINT:
			if (GetUpdateRect(h,0,0)) {
				PAINTSTRUCT p;
				HDC dc;
				if (!(dc=BeginPaint(h,&p))) return 0;
				BitBlt(dc,0,0,SPECWIDTH,SPECHEIGHT,specdc,0,0,SRCCOPY);
				EndPaint(h,&p);
			}
			return 0;

		case WM_LBUTTONUP:
			specmode=(specmode+1)%4; // swap spectrum mode
			memset(specbuf,0,SPECWIDTH*SPECHEIGHT);	// clear display
			return 0;

		case WM_CREATE:
			win=h;
			// initialize BASS
			if (!BASS_Init(-1,44100,0,win,NULL)) {
				Error("Can't initialize device");
				return -1;
			}
			if (!PlayFile()) { // start a file playing
				BASS_Free();
				return -1;
			}
			{ // create bitmap to draw spectrum in - 8 bit for easy updating :)
				BYTE data[2000]={0};
				BITMAPINFOHEADER *bh=(BITMAPINFOHEADER*)data;
				RGBQUAD *pal=(RGBQUAD*)(data+sizeof(*bh));
				int a;
				bh->biSize=sizeof(*bh);
				bh->biWidth=SPECWIDTH;
				bh->biHeight=SPECHEIGHT; // upside down (line 0=bottom)
				bh->biPlanes=1;
				bh->biBitCount=8;
				bh->biClrUsed=bh->biClrImportant=256;
				// setup palette
				for (a=1;a<128;a++) {
					pal[a].rgbGreen=256-2*a;
					pal[a].rgbRed=2*a;
				}
				for (a=0;a<32;a++) {
					pal[128+a].rgbBlue=8*a;
					pal[128+32+a].rgbBlue=255;
					pal[128+32+a].rgbRed=8*a;
					pal[128+64+a].rgbRed=255;
					pal[128+64+a].rgbBlue=8*(31-a);
					pal[128+64+a].rgbGreen=8*a;
					pal[128+96+a].rgbRed=255;
					pal[128+96+a].rgbGreen=255;
					pal[128+96+a].rgbBlue=8*a;
				}
				// create the bitmap
				specbmp=CreateDIBSection(0,(BITMAPINFO*)bh,DIB_RGB_COLORS,&specbuf,NULL,0);
				specdc=CreateCompatibleDC(0);
				SelectObject(specdc,specbmp);
			}
			// setup update timer (40hz)
			timer=timeSetEvent(25,25,(LPTIMECALLBACK)&UpdateSpectrum,0,TIME_PERIODIC);
			break;

		case WM_DESTROY:
			if (timer) timeKillEvent(timer);
			BASS_Free();
			if (specdc) DeleteDC(specdc);
			if (specbmp) DeleteObject(specbmp);
			PostQuitMessage(0);
			break;
	}
	return DefWindowProc(h, m, w, l);
}

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc={0};
    MSG msg;

	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
		MessageBox(0,"An incorrect version of BASS.DLL was loaded",0,MB_ICONERROR);
		return 0;
	}

	// register window class and create the window
	wc.lpfnWndProc = SpectrumWindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "BASS-Spectrum";
	if (!RegisterClass(&wc) || !CreateWindow("BASS-Spectrum",
			"BASS spectrum example (click to toggle mode)",
			WS_POPUPWINDOW|WS_CAPTION|WS_VISIBLE, 200, 200,
			SPECWIDTH+2*GetSystemMetrics(SM_CXDLGFRAME),
			SPECHEIGHT+GetSystemMetrics(SM_CYCAPTION)+2*GetSystemMetrics(SM_CYDLGFRAME),
			NULL, NULL, hInstance, NULL)) {
		Error("Can't create window");
		return 0;
	}
	ShowWindow(win, SW_SHOWNORMAL);

	while (GetMessage(&msg,NULL,0,0)>0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
