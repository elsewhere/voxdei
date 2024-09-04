// BASS Simple Test, copyright (c) 1999-2006 Ian Luck.

#include <windows.h>
#include <stdlib.h>
#include "bass.h"
#include "basstest.h"

HWND win=NULL;

HSTREAM *strs=NULL;
int strc=0;
HMUSIC *mods=NULL;
int modc=0;
HSAMPLE *sams=NULL;
int samc=0;

/* Display error messages */
void Error(const char *es)
{
	char mes[200];
	sprintf(mes,"%s\n(error code: %d)",es,BASS_ErrorGetCode());
	MessageBox(win,mes,0,0);
}

/* Messaging macros */
#define MESS(id,m,w,l) SendDlgItemMessage(win,id,m,(WPARAM)w,(LPARAM)l)
#define MLM(m,w,l) MESS(ID_MODLIST,m,w,l)
#define SLM(m,w,l) MESS(ID_SAMLIST,m,w,l)
#define STLM(m,w,l) MESS(ID_STRLIST,m,w,l)
#define GETMOD() MLM(LB_GETCURSEL,0,0)
#define GETSAM() SLM(LB_GETCURSEL,0,0)
#define GETSTR() STLM(LB_GETCURSEL,0,0)

BOOL CALLBACK dialogproc(HWND h,UINT m,WPARAM w,LPARAM l)
{
	static OPENFILENAME ofn;
	static char path[MAX_PATH];
	switch (m) {
		case WM_TIMER:
			{ /* update the CPU usage % display */
				char text[10];
				sprintf(text,"%.2f",BASS_GetCPU());
				MESS(ID_CPU,WM_SETTEXT,0,text);
			}
			break;

		case WM_COMMAND:
			switch (LOWORD(w)) {
				case IDCANCEL:
					DestroyWindow(h);
					break;

				case ID_STRADD:
					{
						char file[MAX_PATH]="";
						HSTREAM str;
						ofn.lpstrFilter="Streamable files (wav/aif/mp3/mp2/mp1/ogg)\0*.wav;*.aif;*.mp3;*.mp2;*.mp1;*.ogg\0All files\0*.*\0\0";
						ofn.lpstrFile=file;
						if (GetOpenFileName(&ofn)) {
							memcpy(path,file,ofn.nFileOffset);
							path[ofn.nFileOffset-1]=0;
							if (str=BASS_StreamCreateFile(FALSE,file,0,0,0)) {
								strc++;
								strs=(HSTREAM*)realloc((void*)strs,strc*sizeof(*strs));
								strs[strc-1]=str;
								STLM(LB_ADDSTRING,0,strrchr(file,'\\')+1);
							} else
								Error("Can't open stream");
						}
					}
					break;
				case ID_STRREMOVE:
					{
						int s=GETSTR();
						if (s!=LB_ERR) {
							STLM(LB_DELETESTRING,s,0);
							/* Free the stream from memory */
							BASS_StreamFree(strs[s]);
							strc--;
							memcpy(strs+s,strs+s+1,(strc-s)*sizeof(*strs));
						}
					}
					break;
				case ID_STRPLAY:
					{
						int s=GETSTR();
						/* Play the stream (continue from current position) */
						if (s!=LB_ERR)
							if (!BASS_ChannelPlay(strs[s],FALSE)) Error("Can't play stream");
					}
					break;
				case ID_STRSTOP:
					{
						int s=GETSTR();
						/* Stop the stream */
						if (s!=LB_ERR) BASS_ChannelStop(strs[s]);
					}
					break;
				case ID_STRRESTART:
					{
						int s=GETSTR();
						/* Play the stream from the start */
						if (s!=LB_ERR) BASS_ChannelPlay(strs[s],TRUE);
					}
					break;

				case ID_MODADD:
					{
						char file[MAX_PATH]="";
						HMUSIC mod;
						ofn.lpstrFilter="MOD music files (mo3/xm/mod/s3m/it/mtm/umx)\0*.mo3;*.xm;*.mod;*.s3m;*.it;*.mtm;*.umx\0All files\0*.*\0\0";
						ofn.lpstrFile=file;
						if (GetOpenFileName(&ofn)) {
							memcpy(path,file,ofn.nFileOffset);
							path[ofn.nFileOffset-1]=0;
							/* Load a music from "file" and make it use ramping */
							if (mod=BASS_MusicLoad(FALSE,file,0,0,BASS_MUSIC_RAMP |BASS_MUSIC_POSRESET |BASS_SAMPLE_FLOAT |BASS_MUSIC_FT2MOD,0)) {
								modc++;
								mods=(HMUSIC*)realloc((void*)mods,modc*sizeof(*mods));
								mods[modc-1]=mod;
								MLM(LB_ADDSTRING,0,strrchr(file,'\\')+1);
							} else
								Error("Can't load music");
						}
					}
					break;
				case ID_MODREMOVE:
					{
						int s=GETMOD();
						if (s!=LB_ERR) {
							MLM(LB_DELETESTRING,s,0);
							/* Free the music from memory */
							BASS_MusicFree(mods[s]);
							modc--;
							memcpy(mods+s,mods+s+1,(modc-s)*sizeof(*mods));
						}
					}
					break;
				case ID_MODPLAY:
					{
						int s=GETMOD();
						/* Play the music (continue from current position) */
						if (s!=LB_ERR)
							if (!BASS_ChannelPlay(mods[s],FALSE)) Error("Can't play music");
					}
					break;
				case ID_MODSTOP:
					{
						int s=GETMOD();
						/* Stop the music */
						if (s!=LB_ERR) BASS_ChannelStop(mods[s]);
					}
					break;
				case ID_MODRESTART:
					{
						int s=GETMOD();
						/* Play the music from the start */
						if (s!=LB_ERR) BASS_ChannelPlay(mods[s],TRUE);
					}
					break;

				case ID_SAMADD:
					{
						char file[MAX_PATH]="";
						HSAMPLE sam;
						ofn.lpstrFilter="Sample files (wav/aif)\0*.wav;*.aif\0All files\0*.*\0\0";
						ofn.lpstrFile=file;
						if (GetOpenFileName(&ofn)) {
							memcpy(path,file,ofn.nFileOffset);
							path[ofn.nFileOffset-1]=0;
							/* Load a sample from "file" and give it a max of 3 simultaneous
							playings using playback position as override decider */
							if (sam=BASS_SampleLoad(FALSE,file,0,0,3,BASS_SAMPLE_OVER_POS)) {
								samc++;
								sams=(HSAMPLE*)realloc((void*)sams,samc*sizeof(*sams));
								sams[samc-1]=sam;
								SLM(LB_ADDSTRING,0,strrchr(file,'\\')+1);
							} else
								Error("Can't load sample");
						}
					}
					break;
				case ID_SAMREMOVE:
					{
						int s=GETSAM();
						if (s!=LB_ERR) {
							SLM(LB_DELETESTRING,s,0);
							/* Free the sample from memory */
							BASS_SampleFree(sams[s]);
							samc--;
							memcpy(sams+s,sams+s+1,(samc-s)*sizeof(*sams));
						}
					}
					break;
				case ID_SAMPLAY:
					{
						int s=GETSAM();
						// Play the sample at default rate, volume=50, random pan position
						if (s!=LB_ERR) {
							HCHANNEL ch=BASS_SampleGetChannel(sams[s],FALSE);
							BASS_ChannelSetAttributes(ch,-1,50,(rand()%201)-100);
							if (!BASS_ChannelPlay(ch,FALSE)) Error("Can't play sample");
						}
					}
					break;

				case ID_STOP:
					/* Pause output */
					BASS_Pause();
					break;
				case ID_RESUME:
					/* Resume output */
					BASS_Start();
					break;
			}
			break;

		case WM_INITDIALOG:
			win=h;
			/* Initialize default output device */
			if (!BASS_Init(-1,44100,0,win,NULL))
				Error("Can't initialize device");
			SetTimer(h,1,250,NULL);
			GetCurrentDirectory(MAX_PATH,path);
			memset(&ofn,0,sizeof(ofn));
			ofn.lStructSize=sizeof(ofn);
			ofn.hwndOwner=h;
			ofn.nMaxFile=MAX_PATH;
			ofn.lpstrInitialDir=path;
			ofn.Flags=OFN_HIDEREADONLY|OFN_EXPLORER;
			return 1;

		case WM_DESTROY:
			KillTimer(h,1);
			/* Close output */
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

	/* display the window */
	DialogBox(hInstance,MAKEINTRESOURCE(1000),NULL,&dialogproc);

	return 0;
}
