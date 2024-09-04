// BASS Console Test (LL/GPA version), copyright (c) 1999-2006 Ian Luck.

/*
Demonstrates how you can use LoadLibrary and GetProcAddress to import
BASS, instead of using BASS.LIB. Also demonstrates including BASS.DLL
in the EXE as a resource, instead of seperate as a file. It's basically
the same as the CONTEST.C version, with LoadBASS and FreeBASS functions
added, to import and free BASS respectively.
*/

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <conio.h>

#define BASSDEF(f) (WINAPI *f)	// define the functions as pointers
#include "bass.h"

char tempfile[MAX_PATH];	// temporary BASS.DLL
HINSTANCE bass=0;			// bass handle

/* load BASS and the required functions */
void LoadBASS()
{
	BYTE *data;
	HANDLE hres,hfile;
	DWORD len,c;
	char temppath[MAX_PATH];
	/* get the BASS.DLL resource */
	if (!(hres=FindResource(GetModuleHandle(NULL),"BASS_DLL",RT_RCDATA))
		|| !(len=SizeofResource(NULL,hres))
		|| !(hres=LoadResource(NULL,hres))
		|| !(data=LockResource(hres))) {
		printf("Error: Can't get the BASS.DLL resource\n");
		ExitProcess(0);
	}
	/* get a temporary filename */
	GetTempPath(MAX_PATH,temppath);
	GetTempFileName(temppath,"bas",0,tempfile);
	/* write BASS.DLL to the temporary file */
	if (INVALID_HANDLE_VALUE==(hfile=CreateFile(tempfile,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_TEMPORARY,NULL))) {
		printf("Error: Can't write BASS.DLL\n");
		ExitProcess(0);
	}
	WriteFile(hfile,data,len,&c,NULL);
	CloseHandle(hfile);

	/* load the temporary BASS.DLL library */
	if (!(bass=LoadLibrary(tempfile))) {
		printf("Error: Can't load BASS.DLL\n");
		ExitProcess(0);
	}
	/* "load" all the BASS functions that are to be used */
#define LOADBASSFUNCTION(f) *((void**)&f)=GetProcAddress(bass,#f)
	LOADBASSFUNCTION(BASS_ErrorGetCode);
	LOADBASSFUNCTION(BASS_Init);
	LOADBASSFUNCTION(BASS_Free);
	LOADBASSFUNCTION(BASS_GetCPU);
	LOADBASSFUNCTION(BASS_MusicLoad);
	LOADBASSFUNCTION(BASS_MusicGetAttribute);
	LOADBASSFUNCTION(BASS_StreamCreateFile);
	LOADBASSFUNCTION(BASS_StreamCreateURL);
	LOADBASSFUNCTION(BASS_StreamGetFilePosition);
	LOADBASSFUNCTION(BASS_ChannelGetLength);
	LOADBASSFUNCTION(BASS_ChannelGetTags);
	LOADBASSFUNCTION(BASS_ChannelPlay);
	LOADBASSFUNCTION(BASS_ChannelBytes2Seconds);
	LOADBASSFUNCTION(BASS_ChannelIsActive);
	LOADBASSFUNCTION(BASS_ChannelSlideAttributes);
	LOADBASSFUNCTION(BASS_ChannelIsSliding);
	LOADBASSFUNCTION(BASS_ChannelGetPosition);
	LOADBASSFUNCTION(BASS_ChannelGetLevel);
}

/* free the BASS library from memory and delete the temporary file */
void FreeBASS()
{
	if (!bass) return;
	FreeLibrary(bass);
	bass=0;
	DeleteFile(tempfile);
}

/* display error messages */
void Error(char *text) 
{
	printf("Error(%d): %s\n",BASS_ErrorGetCode(),text);
	BASS_Free();
	FreeBASS();
	exit(0);
}

void main(int argc, char **argv)
{
	DWORD chan,act,time,level;
	BOOL ismod;
	QWORD pos;
	int a;

	printf("LoadLibrary/GetProcAddress example : MOD/MPx/OGG/WAV player\n"
			"-----------------------------------------------------------\n");

	if (argc!=2) {
		printf("\tusage: loadnget <file>\n");
		return;
	}

	LoadBASS();

	// setup output - default device
	if (!BASS_Init(-1,44100,0,0,NULL))
		Error("Can't initialize device");

	// try streaming the file/url
	if ((chan=BASS_StreamCreateFile(FALSE,argv[1],0,0,BASS_SAMPLE_LOOP))
		|| (chan=BASS_StreamCreateURL(argv[1],0,BASS_SAMPLE_LOOP,0,0))) {
		pos=BASS_ChannelGetLength(chan);
		if (BASS_StreamGetFilePosition(chan,BASS_FILEPOS_DOWNLOAD)!=-1) {
			// streaming from the internet
			if (pos!=-1)
				printf("streaming internet file [%I64u bytes]",pos);
			else
				printf("streaming internet file");
		} else
			printf("streaming file [%I64u bytes]",pos);
		ismod=FALSE;
	} else {
		// try loading the MOD (with looping, sensitive ramping, and calculate the duration)
		if (!(chan=BASS_MusicLoad(FALSE,argv[1],0,0,BASS_SAMPLE_LOOP|BASS_MUSIC_RAMPS|BASS_MUSIC_PRESCAN|BASS_MUSIC_SURROUND,0)))
			// not a MOD either
			Error("Can't play the file");
		for (a=0;BASS_MusicGetAttribute(chan,BASS_MUSIC_ATTRIB_VOL_CHAN+a)!=-1;a++); // count channels
		printf("playing MOD music \"%s\" [%u chans, %u orders]",
			BASS_ChannelGetTags(chan,BASS_TAG_MUSIC_NAME),a,BASS_MusicGetOrders(chan));
		pos=BASS_ChannelGetLength(chan);
		ismod=TRUE;
	}

	// display the time length
	if (pos!=-1) {
		time=(DWORD)BASS_ChannelBytes2Seconds(chan,pos);
		printf(" %u:%02u\n",time/60,time%60);
	} else // no time length available
		printf("\n");

	BASS_ChannelPlay(chan,FALSE);

	while (!_kbhit() && (act=BASS_ChannelIsActive(chan))) {
		// display some stuff and wait a bit
		level=BASS_ChannelGetLevel(chan);
		pos=BASS_ChannelGetPosition(chan);
		time=BASS_ChannelBytes2Seconds(chan,pos);
		printf("pos %09I64u",pos);
		if (ismod) {
			pos=BASS_MusicGetOrderPosition(chan);
			printf(" (%03u:%03u)",LOWORD(pos),HIWORD(pos));
		}
		printf(" - %u:%02u - L ",time/60,time%60);
		if (act==BASS_ACTIVE_STALLED) { // playback has stalled
			printf("-- buffering : %05u --",
				BASS_StreamGetFilePosition(chan,BASS_FILEPOS_DOWNLOAD)-BASS_StreamGetFilePosition(chan,BASS_FILEPOS_DECODE));
		} else {
			for (a=27204;a>200;a=a*2/3) putchar(LOWORD(level)>=a?'*':'-');
			putchar(' ');
			for (a=210;a<32768;a=a*3/2) putchar(HIWORD(level)>=a?'*':'-');
		}
		printf(" R - cpu %.2f%%  \r",BASS_GetCPU());
		fflush(stdout);
		Sleep(50);
	}
	printf("                                                                             \r");

	// wind the frequency down...
	BASS_ChannelSlideAttributes(chan,1000,-1,-101,500);
	Sleep(300);
	// ...and fade-out to avoid a "click"
	BASS_ChannelSlideAttributes(chan,-1,-2,-101,200);
	while (BASS_ChannelIsSliding(chan)) Sleep(1);

	BASS_Free();
	FreeBASS();
}
