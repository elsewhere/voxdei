#include <stdio.h>
#include "tuho.hpp"

extern int tuhokamera; 
extern int treecount;
extern Tree **trees;

using namespace std;

vector<TreeSegment*> tuholista;
extern int tuhokamerastart;
extern int tuhokameracount;

extern Sound* sound;
extern Song *songs;
extern int currentsong;

extern bool valaistus;
extern float valospeed;
extern Vector3 valonpaikka;
extern bool valovarina;
extern bool rotatedPuu;
bool useraadot;

int raatocount;
Vector3 *raatopalat1;
Vector3 *raatopalat2;
float *raatoalphat1;
float *raatoalphat2;
Vector3 raatocolor;
bool neonraato;

bool usemovedeath = true;
bool movedeathsingledirection = false;
Vector3 movedeathdirection;
bool usespectrummove = true;
float spectrummovefactor = 1.0f;

bool useonlydeath = false;
int movedeathdist = 50;
float minstartenergy = 0.4f;
float maxstartenergy = 1.3f;
float lahtokohta = 0.0f;

float enkelimina = 2.0f;
float enkelimaxa = 7.5f;
float enkeliminb = 4.0f;
float enkelimaxb = 15.0f;
float enkeliminrad = 80.0f;
float enkelimaxrad = 140.0f;
float enkelitotalspeed = 0.5f;

vector <Enkeli*> enkelilista;

Enkeli::Enkeli()
{
    this->type = 0; //TODO
    this->a = randFloat()*2*3.141592f;
    this->b = randFloat()*2*3.141592f;
    this->r = enkeliminrad + (enkelimaxrad - enkeliminrad) * randFloat();
    this->aspeed = (enkelimina + (enkelimaxa - enkelimina) * randFloat()) * enkelitotalspeed;
    this->bspeed = (enkeliminb + (enkelimaxb - enkeliminb) * randFloat()) * enkelitotalspeed;
}


void tuho(float aika)
{
    int i = 0;
    static int currenttuhokamera = tuhokamerastart;
	const float alpha = calcSaturate(aika, 0, 1, 6);

    static float cumulativespectrum = 0.0f;
    static float spectrumlimit = 70;
    static float prevpos = 0.0f;
    static float cumtime = 0.0f;
    static bool moveyet = false;
    const float limit = 0.005f;

    float basicdt;
    
    float array[1024];

    float spectrum = 0;
    sound->getSpectrum(&songs[currentsong], array, 1024);

    for (i = 0; i < 1023; i++)
    {
        spectrum += array[i];
    }

    cumulativespectrum += spectrum;
    
    float dt;

    if (usespectrummove)
    {
        float moveamount = (0.8f+spectrum*spectrummovefactor);
        if (moveamount > 1.3f)
            moveamount = 1.3f;
        dt = (aika - prevpos)*moveamount;
    }
    else
    {
        dt = aika - prevpos;
    }
    basicdt = aika - prevpos;
    prevpos = aika;

    cumtime += dt;

    //liikkeelle
    if (aika > lahtokohta && !moveyet)
    {
        moveyet = true;
        cumtime = 0;
    }

    //palojen siirto listasta toiseen

    const int maxpalatremoved = 3;
    int removed = 0;
    while (cumtime > limit && moveyet)
    {
        for (i = 0; i < treecount; i++)
        {
            vector<TreeSegment*>::iterator iterator;
            int l = trees[i]->treesegments.size();

            if (l > 15)
            {
                for (int j = 0; j < ((spectrummovefactor>0.3f)?3:5); j++)
                {
                    iterator = trees[i]->treesegments.begin() + (rand()%(l/2));

                    TreeSegment *s = *iterator;
                    s->startenergy = minstartenergy + (maxstartenergy - minstartenergy)*randFloat();
                    s->energy = s->startenergy;
                    s->energyscale = 1;// + randFloat()*4;

                    //lisätään raato
                    if (useraadot)
                    {
                        Vector3 r1 = (s->v1->pos + s->v2->pos + s->v3->pos + s->v4->pos) / 4;
                        Vector3 r2 = (s->v5->pos + s->v6->pos + s->v7->pos + s->v8->pos) / 4;

                        raatopalat1[raatocount] = r1;
                        raatopalat2[raatocount] = r2;

                        raatoalphat1[raatocount] = 0.4f+0.6f*randFloat();
                        raatoalphat2[raatocount] = 0.4f+0.6f*randFloat();

                        raatocount++;
                    }

                    tuholista.push_back(*iterator);
                    iterator = trees[i]->treesegments.erase(iterator);
                }
            }
        }
        for (int j = 0; j < 1; j++)
        {
            Enkeli *e = new Enkeli();
            enkelilista.push_back(e);

        }

        cumtime -= limit;
        removed++;
        if (removed >= maxpalatremoved)
        {
            break;
        }
    }

    //kameran liikutus
    if (cumulativespectrum > spectrumlimit)
    {
        cumulativespectrum -= spectrumlimit;
        currenttuhokamera++;
        if (currenttuhokamera >= tuhokamerastart + tuhokameracount)
        {
            currenttuhokamera = tuhokamerastart;
        }
        spectrumlimit = 100 + 150*randFloat();

    }

    //enkeleiden päivitys
    vector<Enkeli*>::iterator ei;
    for (ei = enkelilista.begin(); ei < enkelilista.end(); ei++)
    {
        Enkeli *e = *ei;
        e->a += e->aspeed * basicdt;
        e->b += e->bspeed * basicdt;
    }

	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

    glLoadIdentity();
    cam->useCamera(currenttuhokamera);

    int kokolistankoko = 0;

	float fogcol[] = {1,1,1,1};
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fogcol);
	glFogf(GL_FOG_DENSITY, 0.15f);//+0.4f*(float)cos(aika*10));
	glHint(GL_FOG_HINT, GL_DONT_CARE);
    if (rotatedPuu)
    {
	    glFogf(GL_FOG_START, 120);
	    glFogf(GL_FOG_END, 210);
    }
    else
    {
	    glFogf(GL_FOG_START, 100);
	    glFogf(GL_FOG_END, 190);
    }
	glEnable(GL_FOG);




    if (useraadot)
    {
        Vector3 tempraatocolor;
        if (neonraato)
        {
            tempraatocolor = raatocolor * spectrum;   
        }
        else
        {
            tempraatocolor = raatocolor;

        }
        //raadot
        glEnable(GL_DEPTH_TEST);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        for (i = 0; i < raatocount; i++)
        {
            glColor4f(tempraatocolor.x, tempraatocolor.y, tempraatocolor.z, alpha*raatoalphat1[i]);
            glVertex3fv((float *)&raatopalat1[i]);
            glColor4f(tempraatocolor.x, tempraatocolor.y, tempraatocolor.z, alpha*raatoalphat2[i]);
            glVertex3fv((float *)&raatopalat2[i]);
        }
        glEnd();
    }

    float valoposition[4];

    if (valaistus)
    {
	    valoposition[0] = 50*cosf(aika*11*valospeed);
        valoposition[1] = 47*cosf(aika*18*valospeed);
        valoposition[2] = 66*sinf(aika*6*valospeed);
        valoposition[3] = 1.0f;
    }
    else
    {
        valoposition[0] = valonpaikka.x;
        valoposition[1] = valonpaikka.y;
        valoposition[2] = valonpaikka.z;
        valoposition[3] = 1.0f;
    }
    float aval = 0.2f+spectrum*0.04f;
    if (!valovarina)
    {
        aval = 0.2f;
    }
	const float ambientvalo[] = {aval, aval,aval, 1};
	const float specularvalo[] = {1, 1, 1, 1};
	const float diffusevalo[] = {1, 1, 1, 1};

	glLightfv(GL_LIGHT1, GL_POSITION, valoposition);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientvalo);
  	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffusevalo);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularvalo);
	glShadeModel(GL_FLAT);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);




    for (i = 0; i < treecount; i++)
    {
        if (trees[i]->wireframe)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glEnable(GL_DEPTH_TEST);
        }
        glDepthMask(1);

        vector<TreeSegment*>::iterator iterator;

        //elossa olevat palat
        if (!useonlydeath)
        {
            for (iterator = trees[i]->treesegments.begin(); iterator != trees[i]->treesegments.end(); iterator++)
            {
                TreeSegment *s = *iterator;
                s->draw(1);
            }
        }
        kokolistankoko += trees[i]->treesegments.size();
    }
    vector<TreeSegment*>::iterator iterator;

    for (iterator = tuholista.begin(); iterator < tuholista.end(); iterator++)
    {
        TreeSegment *s = *iterator;

        s->energy -= s->energyscale * dt;

        float t = s->energy / s->startenergy;

        if (t > 0.00001f)
        {
            if (usemovedeath)
            {
                if (t < 0)
                {
                    t = 0.0f;
                }
                if (t > 1)
                    t = 1;

                if (movedeathsingledirection)
                {
                    s->drawMove(t*alpha, movedeathdirection*(1-t*s->massa)*movedeathdist);
                }
                else
                {
                    s->drawMove(t*alpha, s->movedirection*(1-t*s->massa)*movedeathdist);
                }
            }
        }
        else
        {
            iterator = tuholista.erase(iterator);
        }
    }
    glDisable(GL_FOG);
 	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT1);
	glDisable(GL_NORMALIZE);
    glDisable(GL_COLOR_MATERIAL);
/*
    for (ei = enkelilista.begin(); ei < enkelilista.end(); ei++)
    {
        Enkeli *e = *ei;
        glColor4f(0,0,0,0.2f);
        glPushMatrix();
        Vector3 p = sphereCoord(e->r, e->a, e->b);
        glTranslatef(p.x, p.y, p.z);
        wireCube(1);
        glPopMatrix();
    }
*/
}
	
void tuho_init()
{
    int i;
    int maxpalat = 0;

    lahtokohta = 0.01f+0.07f*randFloat();

    movedeathdist = 150 + randFloat()*60;
    minstartenergy = 0.1f+randFloat()*0.1f;
    maxstartenergy = 0.2f+randFloat()*0.4f;

    usespectrummove = ((rand()%3)!=0)?true:false;
    spectrummovefactor = 0.2f + 0.5f*randFloat();

    useraadot = ((rand()%4)!=0)?true:false;
    usemovedeath = ((rand()%5)!=0)?true:false;

    if (!useraadot && !usemovedeath)
    {
        if ((rand()%2)==0)
        {
            useraadot = true;
        }
        else
        {
            usemovedeath = true;
        }
    }

    int raatokomponent = rand()%5;
    float raatoshade = 0.2f+randFloat()*0.4f;

    neonraato = ((rand()%3)!=0)?true:false;

    raatocolor = Vector3(0, 0, 0);
    switch(raatokomponent)
    {
        case 0: raatocolor = Vector3(raatoshade, 0, 0); break;
        case 1: raatocolor = Vector3(0, raatoshade, 0); break;
        case 2: raatocolor = Vector3(0, 0, raatoshade); break;
        case 3: raatocolor = Vector3(0, 0, 0); break;
        case 4: raatocolor = Vector3(0, 0, 0); break;
    }

    if (usemovedeath)
    {
        //lottovoitto
        useonlydeath = ((rand()%70)==0)?true:false;
        movedeathsingledirection = ((rand()%3)==0)?true:false;
        movedeathdirection = ((rand()%2)==0)?Vector3(0, 1, 0):Vector3(1, 0, 0);

        useraadot = true;
    }
    else
    {
        useonlydeath = false;
    }


    for (i = 0; i < treecount; i++)
    {
        maxpalat += trees[i]->treesegments.size();
    } 

    raatopalat1 = new Vector3[maxpalat];
    raatopalat2 = new Vector3[maxpalat];
    raatoalphat1 = new float[maxpalat];
    raatoalphat2 = new float[maxpalat];
        
    raatocount = 0;

}

void tuho_free()
{
}