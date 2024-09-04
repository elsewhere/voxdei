// BASS Multiple output example, copyright (c) 2001-2006 Ian Luck.

#include <windows.h>
#include <stdio.h>

#include "bass.h"

HWND win=NULL;

DWORD outdev[2];	// output devices
HSTREAM chan[2];	// the streams

/* display error messages */
void Error(const char *es)
{
	char mes[200];
	sprintf(mes,"%s\n(error code: %d)",es,BASS_ErrorGetCode());
	MessageBox(win,mes,0,0);
}

#define MESS(id,m,w,l) SendDlgItemMessage(win,id,m,(WPARAM)w,(LPARAM)l)

BOOL CALLBACK dialogproc(HWND h,UINT m,WPARAM w,LPARAM l)
{
	static OPENFILENAME ofn;
	static char path[MAX_PATH];

	switch (m) {
		case WM_COMMAND:
			switch (LOWORD(w)) {
				case IDCANCEL:
					DestroyWindow(h);
					break;
				case 10: // open a file to play on device #1
				case 11: // open a file to play on device #2
					{
						int devn=LOWORD(w)-10;
						char file[MAX_PATH]="";
						ofn.lpstrFilter="streamable files\0*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif\0All files\0*.*\0\0";
						ofn.lpstrFile=file;
						if (GetOpenFileName(&ofn)) {
							memcpy(path,file,ofn.nFileOffset);
							path[ofn.nFileOffset-1]=0;
							BASS_StreamFree(chan[devn]);
							BASS_SetDevice(outdev[devn]); // set the device to create stream on
							if (!(chan[devn]=BASS_StreamCreateFile(FALSE,file,0,0,BASS_SAMPLE_LOOP))) {
								MESS(10+devn,WM_SETTEXT,0,"click here to open a file...");
								Error("Can't play the file");
								break;
							}
							MESS(10+devn,WM_SETTEXT,0,file);
							BASS_ChannelPlay(chan[devn],FALSE);
						}
					}
					break;
				case 30: // swap channel devices
					{
						{ // swap handles
							HSTREAM temp=chan[0];
							chan[0]=chan[1];
							chan[1]=temp;
						}
						{ // swap text
							char temp1[MAX_PATH],temp2[MAX_PATH];
							MESS(10,WM_GETTEXT,MAX_PATH,temp1);
							MESS(11,WM_GETTEXT,MAX_PATH,temp2);
							MESS(10,WM_SETTEXT,0,temp2);
							MESS(11,WM_SETTEXT,0,temp1);
						}
						// update the channel devices
						BASS_ChannelSetDevice(chan[0],outdev[0]);
						BASS_ChannelSetDevice(chan[1],outdev[1]);
					}
					break;
			}
			break;

		case WM_INITDIALOG:
			win=h;
			GetCurrentDirectory(MAX_PATH,path);
			memset(&ofn,0,sizeof(ofn));
			ofn.lStructSize=sizeof(ofn);
			ofn.hwndOwner=h;
			ofn.nMaxFile=MAX_PATH;
			ofn.lpstrInitialDir=path;
			ofn.Flags=OFN_HIDEREADONLY|OFN_EXPLORER;
			// setup output devices
			if (!BASS_Init(outdev[0],44100,0,win,NULL)) {
				Error("Can't initialize device 1");
				DestroyWindow(win);
			}
			if (!BASS_Init(outdev[1],44100,0,win,NULL)) {
				Error("Can't initialize device 2");
				DestroyWindow(win);
			}
			MESS(20,WM_SETTEXT,0,BASS_GetDeviceDescription(outdev[0]));
			MESS(21,WM_SETTEXT,0,BASS_GetDeviceDescription(outdev[1]));
			return 1;

		case WM_DESTROY:
			// release both devices
			BASS_SetDevice(outdev[0]);
			BASS_Free();
			BASS_SetDevice(outdev[1]);
			BASS_Free();
			break;
	}
	return 0;
}


// Simple device selector dialog stuff begins here
BOOL CALLBACK devicedialogproc(HWND h,UINT m,WPARAM w,LPARAM l)
{
	switch (m) {
		case WM_COMMAND:
			switch (LOWORD(w)) {
				case 10:
					if (HIWORD(w)!=LBN_DBLCLK) break;
				case IDOK:
					{
						int device=SendDlgItemMessage(h,10,LB_GETCURSEL,0,0)+1;
						EndDialog(h,device);
					}
					break;
			}
			break;

		case WM_INITDIALOG:
			{
				char buf[30],*d;
				int c;
				sprintf(buf,"Select output device #%d",l);
				SetWindowText(h,buf);
				for (c=1;d=BASS_GetDeviceDescription(c);c++) // device 1 = 1st real device
					SendDlgItemMessage(h,10,LB_ADDSTRING,0,(int)d);
				SendDlgItemMessage(h,10,LB_SETCURSEL,0,0);
			}
			return 1;
	}
	return 0;
}
// Device selector stuff ends here

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
		MessageBox(0,"An incorrect version of BASS.DLL was loaded",0,MB_ICONERROR);
		return 0;
	}

	/* Let the user choose the output devices */
	outdev[0]=DialogBoxParam(hInstance,(char*)2000,0,&devicedialogproc,1);
	outdev[1]=DialogBoxParam(hInstance,(char*)2000,0,&devicedialogproc,2);

	/* main dialog */
	DialogBox(hInstance,(char*)1000,0,&dialogproc);

	return 0;
}
