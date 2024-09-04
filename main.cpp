#pragma comment(linker, "/OPT:NOWIN98" )
#pragma comment(linker,"/FILEALIGN:0x200")

#define _NOT_FINAL_BUILD_

#define WIN32_LEAN_AND_MEAN
#define WIN32_LEANER_AND_MEANER

#include <windows.h>

#include <stdio.h>

#include <gl/gl.h>
#include <gl/glu.h>
#include <windows.h>
#include "misc.hpp"
#include "win32.hpp"
#include "sound.hpp"
#include "font.hpp"
#include "texture.hpp"
#include "config.hpp" 
#include "timer.hpp"

#include "ext.hpp"

//efektit
#include "effects/puu.hpp"
#include "effects/tuho.hpp"
#include "effects/taivas.hpp"

//extern "C" int _fltused;
//int _fltused;

Window win;
Ext glExt;	// extensiot
int OPENGL_XRES;
int OPENGL_YRES;
int FSAA;
bool OPENGL_FULLSCREEN;

Vector4 clearcolor;
Vector3 color1;
Vector3 color2;
Vector3 color3;

Timer kello;
CameraHolder *cam;

//TODO: poista

bool runflag=false;
bool musicflag=true;
bool debugflag = false;
bool fsaaflag = false;
bool HRT=true;

bool rotatedPuu = false;
bool valaistus = true;
bool valovarina = true;

float valospeed = 6.0f;
int puurepeats = 3;
float puuheight = 0;

Vector3 valonpaikka;

int tuhokamerastart = 0;
int tuhokameracount = 0;
int taivaskamerastart = 0;
int taivaskameracount = 0;
int puukamerastart = 0;
int puukameracount = 0;

int lopunpituus = 3000;
const int totalsongcount = 11;
const int songcount = 2;
int currentsong = 0;

Sound *sound = NULL;
Song *songs;

const char *songnames[totalsongcount] = 
{
    "data\\1.mp3",
    "data\\2.mp3",
    "data\\3.mp3",
    "data\\4.mp3",
    "data\\5.mp3",
    "data\\6.mp3",
    "data\\7.mp3",
    "data\\8.mp3",
    "data\\9.mp3",
    "data\\10.mp3",
    "data\\11.mp3",
};

const char *titlenames[3] = 
{
    "data\\title1.jpg",
    "data\\title2.jpg",
    "data\\title3.jpg",
};

Texture *text_title;
Texture *text_traction;
Texture *text_code;
Texture *text_music;
Texture *text_party;



static int efektiajat[songcount][2];
static int usedsongs[songcount];

void loadBar(float status);

void efekti(void (*func)(float), int aika, const int ajat[])
{
    if (testaaPos(aika, ajat))
    {
        func(laskePos(aika, ajat));

    }
}


#include "resource.h" 

#ifdef _DEBUG
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmd, int n)
#else
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmd, int n)
#endif 
{
	MSG msg = {0};
	bool done = false;

	//bring up the dialog	
	if(DialogBox(hInstance, MAKEINTRESOURCE(IDD_SETUPDIALOG), NULL, (DLGPROC)ConfigProc) == 1)
	{		
		return false;
	}	
	//if we want to run the intro
	if (runflag==true)
	{

		//create window and sound if necessary
		if(!win.create(false,FSAA)) return 0;
		if (musicflag==true)
		{
            sound = Sound::create();
            sound->setEnabled(true);
            if (!sound->init())
            {
                return false;
            };
            
//			if(!sound.init()) 
//				return 0;
		}

		//**************ENGINEN INITIT TÄHÄN********************

        setSeed(GetTickCount());
        srand(GetTickCount());

        int i = 0;
		font_init();
			
		loadBar(0.0f);

        color1 = Vector3(0.4f, 0, 0.4f);
        color2 = Vector3(0,0,0);
        color3 = Vector3(1,1,1);

        clearcolor = color3;

		glClearColor(clearcolor.x, clearcolor.y, clearcolor.z, 1);
		glViewport(0, 0, OPENGL_XRES, OPENGL_YRES);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		loadBar(0.05f);

		glEnable(GL_POINT_SMOOTH);
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glLineWidth(1.0f);

//		glExt.init();
//		shaders_init();

		loadBar(0.2f);

        //tehdään timeline

        songs = new Song[songcount]; //varsinaiset biisit

        //arvotaan mitkä biisit tulee demoon
        for (i = 0; i < songcount; i++)
        {
            songloop:
            usedsongs[i] = rand()%totalsongcount;

            for (int j = 0; j < i; j++)
            {
                if (usedsongs[i] == usedsongs[j])
                {
                    goto songloop;
                }
            }
        }

        //lataus
        for (i = 0; i < songcount; i++)
        {
            songs[i].loadStream((char *)songnames[usedsongs[i]]);
        }
        //timeline

        int offset = 0;
        for (i = 0;i < songcount; i++)
        {
            int songlength = songs[i].getLength();
            int tyhja = 500 + rand()%1000;

            if (i == songcount - 1)
            {
                tyhja += 1500;
            }

            efektiajat[i][0] = offset;
            efektiajat[i][1] = offset + songlength + tyhja;

            offset += songlength;
            offset += tyhja;

        }

        lopunpituus = 1000 + (rand()%1000);
        currentsong = 0;

        loadBar(0.5f);

        puurepeats = 3 + (rand()%3);
        puuheight = 100 + 150*randFloat();

        rotatedPuu = (rand()%2)==0?true:false;
        valaistus = (rand()%4)!=0?true:false;
        valospeed = 5+randFloat()*10;
        valovarina = ((rand()%2)==0)?true:false;
        valonpaikka = randomSphereVector(40*randFloat());

        const int puucampoints = 10;

        tuhokamerastart = puurepeats;
        tuhokameracount = 3 + (rand()%4);

        taivaskamerastart = tuhokamerastart + tuhokameracount;
        taivaskameracount = 4;

        cam = new CameraHolder(puurepeats+tuhokameracount+taivaskameracount);//kuinka monta kameraa

        //tuhokamerat
        for (i = tuhokamerastart; i < tuhokamerastart + tuhokameracount; i++)
        {
            int tuhokamerapoints = 7 + rand()%4;
            cam->initCamera(i, tuhokamerapoints, efektiajat[1][0], efektiajat[1][1]);

            if (rotatedPuu)
            {

                float tuhokamrad = 140 + 40*randFloat();
                float tuhokama = 6*randFloat();
                float tuhokamb = 5*randFloat();
                float tuhokamraddelta = 4+5*randFloat();

                float tuhokamadelta = (randFloat()-0.5f)*0.7f;
                float tuhokambdelta = (randFloat()-0.5f)*1.2f;

                for (int j = 0; j < tuhokamerapoints; j++)
                {
                    Vector3 tuhokampoint = sphereCoord(tuhokamrad, tuhokama, tuhokamb);
                    Vector3 tuhokamtgt = randomSphereVector(randFloat()*15);//tuhokampoint * 0.1f;
                    cam->addCameraPoint(i, tuhokampoint);
                    cam->addCameraTargetPoint(i, tuhokamtgt);

                    tuhokamrad += tuhokamraddelta;
                    tuhokama += tuhokamadelta;
                    tuhokamb += tuhokambdelta;
                }

            }
            else
            {
                float a = randFloat()*2*3.141592f;
                float r1 = 100+50*randFloat();
                float r2 = r1*(1.1f+0.57f*randFloat());

                float power = (randFloat()-0.5f)*(1.9f+randFloat()*3.1f);
                for (int j = 0; j < tuhokamerapoints; j++)
                {
                    float jt = j / (float)tuhokamerapoints;
                    float r = r1 * (1-jt) + r2*jt; 
                    Vector3 tuhokampoint = Vector3(cosf(a)*r, puuheight/2 + (randFloat()-0.5f)*(puuheight/2), sinf(a)*r);
                    Vector3 tuhokamtgt = Vector3(cosf(a)*r*0.3f, puuheight/2, sinf(a)*r*0.3f);
                    cam->addCameraPoint(i, tuhokampoint);
                    cam->addCameraTargetPoint(i, tuhokamtgt);
                    a += (0.6f+0.3f*randFloat())*power;

                }

            }
        }
        //taivaskamerat
        
        for (i = taivaskamerastart; i < taivaskamerastart + taivaskameracount; i++)
        {
            int taivaskamerapoints = 13 + rand()%7;
            cam->initCamera(i, taivaskamerapoints, efektiajat[2][0], efektiajat[2][1]);

            float taivaskamrad = 140 + 40*randFloat();
            float taivaskama = 6*randFloat();
            float taivaskamb = 5*randFloat();
            float taivaskamraddelta = 4+5*randFloat();

            float taivaskamadelta = (randFloat()-0.5f)*0.7f;
            float taivaskambdelta = (randFloat()-0.5f)*1.2f;

            for (int j = 0; j < taivaskamerapoints; j++)
            {
                Vector3 taivaskampoint = sphereCoord(taivaskamrad, taivaskama, taivaskamb);
                Vector3 taivaskamtgt = randomSphereVector(randFloat()*15);//taivaskampoint * 0.1f;
                cam->addCameraPoint(i, taivaskampoint);
                cam->addCameraTargetPoint(i, taivaskamtgt);

                taivaskamrad += taivaskamraddelta;
                taivaskama += taivaskamadelta;
                taivaskamb += taivaskambdelta;
            }
        }
        
        const int puutime = efektiajat[0][1]; //puu aina ekana
        const int puuvaihe = puutime / puurepeats;
        puukamerastart = 0;
        puukameracount = puurepeats;

        //puukamera
        for (int j = 0; j < puurepeats; j++)
        {
            int alkuaika = j * puuvaihe;
            int loppuaika = (j+1) * puuvaihe;
            cam->initCamera(j, puucampoints, efektiajat[0][0], efektiajat[0][1]);
            //koordinaatit

            if (!rotatedPuu)
            {
                float camdist = 20 + 90*randFloat();
                float camdist2 = 60 + 90*randFloat();
                for (int i = 0; i < puucampoints; i++)
                { 
                    const float it = i / (float)puucampoints;
                    Vector3 camstart = Vector3(camdist, 4, camdist2);
                    Vector3 camend = Vector3(camdist, puuheight + (randFloat()-0.5f)*30, camdist2);
                    
                    Vector3 targetstart = Vector3(0, 16*randFloat(), 0);
                    Vector3 targetend = Vector3(0, puuheight*(0.8f+0.1f*randFloat()), 0);
                    Vector3 campos = camstart*(1-it) + camend*it + randomSphereVector(5*randFloat());
                    Vector3 camtarget = targetstart*(1-it) + targetend*it + randomSphereVector(2*randFloat());

                    cam->addCameraPoint(j, campos);
                    cam->addCameraTargetPoint(j, camtarget);
                }
            }
            else
            {
                float camdist = 120 + 90*randFloat();
                float camdist2 = 120 + 90*randFloat();
                const float tgt = 0.2f+0.1f*randFloat();

                const float rotspeed = 0.3f+randFloat()*0.5f;

                float a = randFloat()*3.141592f;
                float b = randFloat()*2*3.141592f;
                for (int i = 0; i < puucampoints; i++)
                { 
                    float camt = randFloat();
                    float r = camdist*camt + camdist2*(1-camt);

                    Vector3 campos = sphereCoord(r, a, b);
                    Vector3 camtarget = campos*tgt;

                    a += randFloat()*rotspeed;
                    b += randFloat()*rotspeed*2;

                    cam->addCameraPoint(j, campos);
                    cam->addCameraTargetPoint(j, camtarget);
               }
            }
        }



        text_title = new Texture();
        text_traction = new Texture();
        text_code = new Texture();
        text_music = new Texture();
        text_party = new Texture();
        text_title->loadJPG(titlenames[rand()%3]);
        text_traction->loadJPG("data\\traction.jpg");
        text_code->loadJPG("data\\code.jpg");
        text_music->loadJPG("data\\music.jpg");
        text_party->loadJPG("data\\party.jpg");
/*
        Texture *text_traction;
        Texture *text_code;
        Texture *text_music;
        Texture *text_party;
*/

		//***************EFEKTIEN INITIT TÄHÄN******************

/*
		alku_init();
        tree_init();
        queue_init();
*/

        //puu_init täytyy tehdä ensimmäisenä! (tuhon initti pohjautuu siihen)
        puu_init();
        tuho_init();
        taivas_init();

		loadBar(1.0f);
        //********************************************************
	
		if (musicflag==true)
        {
            songs[currentsong].start();
        }

		kello.init();
		while(!done)
		{
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{ 
			if(msg.message == WM_QUIT)
				{
					done = true;
				} else if(msg.message == WM_KEYDOWN) {
					const float jump = 1000.0f;
					if(msg.wParam == VK_ESCAPE)		done = true;
					if(msg.wParam == VK_SPACE)	debugflag^=1;
				} else 				
				{
					TranslateMessage(&msg);
			 		DispatchMessage(&msg);
				}   
			}
			else
			{

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				float kerroin = 1.0f;
				if (GetAsyncKeyState(VK_SHIFT))
					kerroin = 10.0f;

				if (GetAsyncKeyState(VK_RIGHT))
					kello.add(0.05f*kerroin);

                if (GetAsyncKeyState(VK_LEFT))
					kello.add(-0.05f*kerroin);


				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				gluPerspective(45.0f, 1024.0f/768.0f, 0.01f, 1000.0f);
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();

                kello.update();
                cam->update();
				int aika = (int)kello.getMilliseconds();
                
                //efektit tässä                
                efekti(puu, aika, efektiajat[0]);
                efekti(tuho, aika, efektiajat[1]);

                if (aika > efektiajat[1][1]-2000) //lopun ohi
                {
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    float fade = calcPosFloat(aika, efektiajat[1][1]-2000, efektiajat[1][1]);
                    flatTausta(Vector3(1,1,1), fade);
                }

                for (i = 0; i < songcount - 1; i++)
                {
                    if (currentsong == i)
                    {
                        //ollaan ohitettu loppu
                        if (aika > efektiajat[i][1])
                        {
                            songs[currentsong].stop();
                        }
                        //ollaan ohitettu seuraavan alku
                        if (aika > efektiajat[(i+1)%songcount][0])
                        {
                            currentsong++;
                            songs[currentsong].start();
                        }
                    }
                }

				if (debugflag)
				{
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_BLEND);
					glColor3f(1,0,0);
					char str[256];

                    float array[1024];

                    float spectrum = 0;
                    sound->getSpectrum(&songs[currentsong], array, 1024);

                    for (int i = 0; i < 1023; i++)
                    {
                        spectrum += array[i];
                    }
					sprintf(str, "aika = %d spectrum = %f",  aika, spectrum);

					glLoadIdentity();
					glDisable(GL_DEPTH_TEST);
					glTranslatef(0, 0, -5.0f);
					font_write(-2.7f, 1.45f, str);

                    sprintf(str, "e1 alkaa = %d", efektiajat[1][0]);
					font_write(-2.7f, 1.15f, str);
                    sprintf(str, "demo loppuu = %d", efektiajat[1][1]);
					font_write(-2.7f, 0.85f, str);
//                    sprintf(str, "demo loppuu = %d", efektiajat[2][1] + lopunpituus);
//					font_write(-2.7f, 0.55f, str);
					glColor3f(1,1,1);
				}

				SwapBuffers(win.getDC());

                float volume = calcSaturate(aika,efektiajat[currentsong][0], efektiajat[currentsong][1], 8);
                sound->setVolume( &songs[currentsong], volume);


                //loppu
				if (aika >= efektiajat[songcount-1][1] + lopunpituus)
					done = true;

			}
        }

        if (musicflag==true)
		{
            songs[currentsong].stop();
            for (i=0;i<songcount;i++)
            {
                songs[i].release();
            }
            sound->close();
            sound = NULL;
		}

        //efektien vapautus

        puu_free();
        tuho_free();
        taivas_free();
		return 0;	
	}
	else
	{
		return 0;
	}

}



void loadBar(float status) {

	// Process messages while loading
	MSG msg = {0};
	PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	// käsittele msg tässä, jos halutaan että käyttäjä voi quitata kesken loaderin
	TranslateMessage(&msg);
	DispatchMessage(&msg);

	float i = status;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();	
	glOrtho(0, 800, 600, 0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); 
	glLoadIdentity();

		glClearColor(clearcolor.x, clearcolor.y, clearcolor.z, clearcolor.a);	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			

		const float sx = 300;			// start x
		const float sy = 298;			// start y
		const float ex = sx+200*i;	// nyt - x
		const float maxx = 500;		// maksimi - x 
		const float ey = 302;			// end y


		float fade_out = 1-(float)(status-0.9f)/(1.0f-0.9f); 
		if(fade_out > 1.0f) fade_out = 1.0f;

		glColor3f(fade_out*0.6f,fade_out*0.6f,fade_out*0.6f);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINE_STRIP);
			glVertex3f( sx-5, sy-5 , 0);
			glVertex3f( maxx+5, sy-5 , 0);
			glVertex3f( maxx+5, ey+5 , 0);
			glVertex3f( sx-5, ey+5 , 0);
			glVertex3f( sx-5, sy-5 , 0);
		glEnd();
		glEnable(GL_TEXTURE_2D);

		glColor3f(fade_out*0.3f,fade_out*0.3f,fade_out*0.3f);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
			glVertex3f( sx, sy , 0);
			glVertex3f( ex, sy , 0);
			glVertex3f( ex, ey , 0);
			glVertex3f( sx, ey , 0);
		glEnd();
		glEnable(GL_TEXTURE_2D);

		glColor3f(1,1,1);
		
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	SwapBuffers(win.getDC());
}

