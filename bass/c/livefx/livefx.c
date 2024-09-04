// BASS Full-duplex test, copyright (c) 2002-2006 Ian Luck.

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <math.h>
#include "bass.h"

HWND win=NULL;

#define MESS(id,m,w,l) SendDlgItemMessage(win,id,m,(WPARAM)w,(LPARAM)l)

HRECORD rchan;	// recording channel
HSTREAM chan;	// playback stream
HFX fx[4]={0};	// FX handles
int chunk;		// recording chunk size
int input;		// current input source
int latency=0;	// current latency

void Error(const char *es)
{
	char mes[200];
	sprintf(mes,"%s\n(error code: %d)",es,BASS_ErrorGetCode());
	MessageBox(win,mes,0,0);
}

// stream function - play the recording data
DWORD CALLBACK stream(HSTREAM handle, char *buffer, int length, DWORD user)
{
	int c;
	// check how much recorded data is buffered
	c=BASS_ChannelGetData(rchan,0,BASS_DATA_AVAILABLE);
	c-=length;
	if (c>2*chunk+1764) { // buffer has gotten pretty large so remove some
		c-=chunk; // leave a single 'chunk'
		BASS_ChannelGetData(rchan,0,c); // remove it
	}
	// fetch recorded data into stream
	c=BASS_ChannelGetData(rchan,buffer,length);
	if (c<length) memset(buffer+c,0,length-c); // short of data
	return length;
}

static BOOL Initialize()
{
	BASS_INFO i;
	// 10ms update period
	BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD,10);
	// setup output - get device latency
	if (!BASS_Init(-1,44100,BASS_DEVICE_LATENCY,win,NULL)) {
		Error("Can't initialize device");
		return FALSE;
	}

	BASS_GetInfo(&i);
	// buffer size = update period + 'minbuf'
	BASS_SetConfig(BASS_CONFIG_BUFFER,10+i.minbuf);

	if (i.dsver<8) { // no DX8, so disable effect buttons
		EnableWindow(GetDlgItem(win,20),FALSE);
		EnableWindow(GetDlgItem(win,21),FALSE);
		EnableWindow(GetDlgItem(win,22),FALSE);
		EnableWindow(GetDlgItem(win,23),FALSE);
	}

	// start recording - default device, 44100hz, stereo, 16 bits, no callback function
	if (!BASS_RecordInit(-1) || !(rchan=BASS_RecordStart(44100,2,0,NULL,0))) {
		BASS_Free();
		Error("Can't initialize recording device");
		return FALSE;
	}
	// wait for recorded data to start arriving (calculate the latency)
	while (!(chunk=BASS_ChannelGetData(rchan,0,BASS_DATA_AVAILABLE))) ;

	// create a stream to play the recording data, and start it
	chan=BASS_StreamCreate(44100,2,0,(STREAMPROC*)stream,0);
	BASS_ChannelPlay(chan,FALSE);

	{ // get list of inputs
		int c;
		const char *i;
		for (c=0;i=BASS_RecordGetInputName(c);c++) {
			MESS(10,CB_ADDSTRING,0,i);
			if (!(BASS_RecordGetInput(c)&BASS_INPUT_OFF)) { // this 1 is currently "on"
				input=c;
				MESS(10,CB_SETCURSEL,input,0);
				MESS(11,TBM_SETPOS,TRUE,LOWORD(BASS_RecordGetInput(input))); // get level
			}
		}
	}

	return TRUE;
}

BOOL CALLBACK dialogproc(HWND h,UINT m,WPARAM w,LPARAM l)
{
	switch (m) {
		case WM_TIMER:
			{ // display current latency (averaged)
				char buf[20];
				latency=(latency*3+BASS_ChannelGetData(chan,0,BASS_DATA_AVAILABLE)
					+BASS_ChannelGetData(rchan,0,BASS_DATA_AVAILABLE))/4;
				sprintf(buf,"%d",latency*1000/176400);
				MESS(15,WM_SETTEXT,0,buf);
			}
			break;
		case WM_COMMAND:
			switch (LOWORD(w)) {
				case IDCANCEL:
					DestroyWindow(h);
					break;
				case 10:
					if (HIWORD(w)==CBN_SELCHANGE) { // input selection changed
						int i;
						input=MESS(10,CB_GETCURSEL,0,0); // get the selection
						for (i=0;BASS_RecordSetInput(i,BASS_INPUT_OFF);i++) ; // 1st disable all inputs, then...
						BASS_RecordSetInput(input,BASS_INPUT_ON); // enable the selected input
						MESS(11,TBM_SETPOS,TRUE,LOWORD(BASS_RecordGetInput(input))); // get the level
					}
					break;
				case 20: // toggle chorus
					if (fx[0]) {
						BASS_ChannelRemoveFX(chan,fx[0]);
						fx[0]=0;
					} else
						fx[0]=BASS_ChannelSetFX(chan,BASS_FX_CHORUS,0);
					break;
				case 21: // toggle gargle
					if (fx[1]) {
						BASS_ChannelRemoveFX(chan,fx[1]);
						fx[1]=0;
					} else
						fx[1]=BASS_ChannelSetFX(chan,BASS_FX_GARGLE,0);
					break;
				case 22: // toggle reverb
					if (fx[2]) {
						BASS_ChannelRemoveFX(chan,fx[2]);
						fx[2]=0;
					} else
						fx[2]=BASS_ChannelSetFX(chan,BASS_FX_REVERB,0);
					break;
				case 23: // toggle flanger
					if (fx[3]) {
						BASS_ChannelRemoveFX(chan,fx[3]);
						fx[3]=0;
					} else
						fx[3]=BASS_ChannelSetFX(chan,BASS_FX_FLANGER,0);
					break;
			}
			break;
		case WM_HSCROLL:
			if (l) { // set input source level
				int level=SendMessage((HWND)l,TBM_GETPOS,0,0);
				BASS_RecordSetInput(input,BASS_INPUT_LEVEL|level);
			}
			break;
		case WM_INITDIALOG:
			win=h;
			MESS(11,TBM_SETRANGE,FALSE,MAKELONG(0,100)); // initialize input level slider
			MessageBox(win,
				"Do not set the input to 'WAVE' / 'What you hear' (etc...) with\n"
				"the level set high, as that is likely to result in nasty feedback.\n",
				"Feedback warning",MB_ICONWARNING);
			if (!Initialize()) {
				DestroyWindow(win);
				break;
			}
			SetTimer(h,1,250,NULL);
			return 1;

		case WM_DESTROY:
			KillTimer(h,1);
			// release it all
			BASS_RecordFree();
			BASS_Free();
			break;
	}
	return 0;
}

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
		MessageBox(0,"An incorrect version of BASS.DLL was loaded",0,MB_ICONERROR);
		return 0;
	}

	{ // enable trackbar support (for the level control)
		INITCOMMONCONTROLSEX cc={sizeof(cc),ICC_BAR_CLASSES};
		InitCommonControlsEx(&cc);
	}

	DialogBox(hInstance,(char*)1000,0,&dialogproc);

	return 0;
}
