#include <stdio.h>
#include "puu.hpp"
#include <vector>

using namespace std;

int treecount = 6;

Tree **trees;
extern bool rotatedPuu;
extern int puurepeats;
extern float puuheight;

extern int puukamerastart;
extern int puukameracount;
extern Sound* sound;
extern Song *songs;
extern int currentsong;
extern bool valaistus;
extern Vector3 valonpaikka;
extern float valospeed;
extern bool valovarina;

bool kolmio = true;
float kolmioituminen = 1.0f;

extern Texture *text_title;
extern Texture *text_traction;
extern Texture *text_code;
extern Texture *text_music;
extern Texture *text_party;

TreeFace::TreeFace(TreeVertex *v1, TreeVertex *v2, TreeVertex *v3, TreeVertex *v4, Vector3 &color)
{
    this->a = v1;
    this->b = v2;
    this->c = v3;
    this->d = v4;

    this->normal = ((b->pos - a->pos).crossProduct(c->pos - a->pos)).normalize();
    this->color = color;

}

void TreeFace::draw(float alpha)
{
    glColor4f(color.x, color.y, color.z, alpha);
    if (wireframe)
    {
       glBegin(GL_LINE_LOOP);
    }
    else
    {
        glBegin(GL_QUADS);
        glNormal3fv((float *)&this->normal);
    }

    glVertex3fv((float *)&a->pos);
    glVertex3fv((float *)&b->pos);
    glVertex3fv((float *)&c->pos);
    glVertex3fv((float *)&d->pos);
    glEnd();
/*
    glColor4f(1,0,0,1);
    glBegin(GL_LINES);
    Vector3 center = (a->pos+b->pos+c->pos+d->pos)*0.25f;
    glVertex3fv((float *)&center);
    glVertex3fv((float *)&(center+this->normal*7));
    glEnd();
*/
}

void TreeFace::drawMove(float alpha, Vector3 &delta)
{
    glEnable(GL_BLEND);
    glColor4f(color.x, color.y, color.z, alpha);

    if (wireframe)
    {
       glBegin(GL_LINE_LOOP);
    }
    else
    {
        glBegin(GL_QUADS);
        glNormal3fv((float *)&this->normal);
    }
    glVertex3fv((float *)&(a->pos+delta));
    glVertex3fv((float *)&(b->pos+delta));
    glVertex3fv((float *)&(c->pos+delta));
    glVertex3fv((float *)&(d->pos+delta));

    glEnd();
}

void TreeSegment::draw(float time)
{
    float a = calcPosFloat(time, st, et);

    if (a < 0.9999f)
    {
        glEnable(GL_BLEND);
    }
    else
    {
        glDisable(GL_BLEND);
    }

    if (a > 0.00001f)
    {
        if (!connected)
        {
//            f1->draw(a);
        }
        f2->draw(a);
        f3->draw(a);
        f4->draw(a);
        f5->draw(a);
        f6->draw(a);
    }
}


void drawTexture(Texture *t, Vector3 &pos, float scale, float alpha)
{
    int w = t->getWidth();
    int h = t->getHeight();

    float sw = w * scale;
    float sh = h * scale;

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, t->getID());

    glColor4f(1,1,1,alpha);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1);
    glVertex3f(pos.x, pos.y, pos.z);
    glTexCoord2f(1, 1);
    glVertex3f(pos.x + sw, pos.y, pos.z);
    glTexCoord2f(1, 0);
    glVertex3f(pos.x + sw, pos.y + sh, pos.z);
    glTexCoord2f(0, 0);
    glVertex3f(pos.x, pos.y + sh, pos.z);

    glEnd();
    
}

void TreeSegment::drawMove(float alpha, Vector3 &delta)
{
    glDisable(GL_BLEND);
    if (alpha > 0.00001f)
    {
        if (!connected)
        {
            f1->drawMove(alpha, delta);
        }
        f2->drawMove(alpha, delta);
        f3->drawMove(alpha, delta);
        f4->drawMove(alpha, delta);
        f5->drawMove(alpha, delta);
        f6->drawMove(alpha, delta);
    }
}

void puu(float aika)
{
    int i;
	const float alpha = calcSaturate(aika, 0, 1, 11);

	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(1);


    static bool showtext = true;
    static bool textshown[5] = { false, false, false, false, false };
    static float cumulativespectrum = 0.0f;
    static float spectrumlimit = 70;
    static float prevpos = 0.0f;
    static float cumtime = 0.0f;
    static int currentkamera = 0;
    static int currenttexture = 0;
    const float limit = 0.01f;

    float dt = aika - prevpos;

    prevpos = aika;

    while (cumtime > limit)
    {
        cumtime -= limit;
    }

    float array[1024];

    float spectrum = 0;
    sound->getSpectrum(&songs[currentsong], array, 1024);

    for (i = 0; i < 1023; i++)
    {
        spectrum += array[i];
    }

    cumulativespectrum += spectrum;

    static Texture *text = text_traction;
    static Vector3 textpaikka = Vector3(-8, 0, 0);
    if (cumulativespectrum > spectrumlimit)
    {
        cumulativespectrum -= spectrumlimit;
        currentkamera++;
        if (currentkamera >= puukamerastart + puukameracount)
        {
            currentkamera = puukamerastart;
        }
        spectrumlimit = 100 + 150*randFloat();

        showtext = false;
        for (i = 0; i < 5; i++)
        {
            if (textshown[i] == false)
            {
                showtext = true;
                break;
            }
        }

        currenttexture = rand() % 5;
        switch(currenttexture)
        {
            case 0: text = text_traction; break;
            case 1: text = text_party; break;
            case 2: text = text_title; break;
            case 3: text = text_code; break;
            case 4: text = text_music; break;
        }
        textshown[currenttexture] = true;


        textpaikka = Vector3(-8, 0, 0);

    }
    cam->useCamera(currentkamera);

    float valoposition[4];
    float aval = 0.2f+spectrum*0.04f;
    if (!valovarina)
    {
        aval = 0.2f;
    }
	const float ambientvalo[] = {aval, aval,aval, 1};
	const float specularvalo[] = {1, 1, 1, 1};
	const float diffusevalo[] = {1, 1, 1, 1};

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

	glLightfv(GL_LIGHT1, GL_POSITION, valoposition);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientvalo);
  	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffusevalo);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularvalo);
	glShadeModel(GL_FLAT);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

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

    for (i = 0; i < treecount; i++)
    {
        vector<TreeSegment*>::iterator iterator;

        const int repeats = puurepeats;
        int count = 0;
        for (iterator = trees[i]->treesegments.begin(); iterator != trees[i]->treesegments.end(); iterator++)
        {
            TreeSegment *s = *iterator;
            s->draw(fmodf(aika*repeats, 1));

        }
    }
 	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT1);
	glDisable(GL_NORMALIZE);
    glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_FOG);

    if (showtext)
    {
        glLoadIdentity();
        glTranslatef(0, 0, -16);

        drawTexture(text, textpaikka, 0.01f, 1); 
    }


}
void createSegmentsAroundSpline(Tree *tree, CatmullRom *spline, int count, bool wireframe, bool connected,bool mustavalko, float radiusmod)
{
    int i;

    //voe elämä... .

    TreeSegment *previousSegment;

    float harmaus = 0.4f+0.3f*randFloat();
    for (i = 0; i < count - 1; i++)
    {
        const float lower_t = i / (float)(count-1);
        const float upper_t = (i + 1) / (float)(count - 1);

        const float padding = 0.0000f;
        Vector3 lowercenter = spline->getValue(lower_t - padding);
        Vector3 uppercenter = spline->getValue(upper_t + padding);

        float lowerradius = 1 + 0.5f*randFloat();
        float upperradius = 1 + 0.5f*randFloat();

        //frenet-frame
        Vector3 T = (uppercenter - lowercenter);
        Vector3 N = (uppercenter + uppercenter);
        Vector3 B = T.crossProduct(N);
        N = B.crossProduct(T);

        B.normalize();
        N.normalize();

        //pisteet

        Vector3 *lower = new Vector3[4];
        Vector3 *upper = new Vector3[4];

        const int ymparys = 4;
        for (int j = 0; j < ymparys; j++)
        {
            const float jt = j / (float)ymparys;
            const float u = jt*2*3.141592f;
            Vector3 p = Vector3(sinf(u), cosf(u), 0) * lowerradius * (kolmio?kolmioituminen:1)*radiusmod;
            Vector3 np = N*p.y + B*p.x;
            Vector3 p2 = Vector3(sinf(u), cosf(u), 0) * lowerradius * radiusmod;
            Vector3 np2 = N*p2.y + B*p2.x;
            lower[j] = np + lowercenter;
            upper[j] = np2 + uppercenter;
        }

        TreeSegment *s = new TreeSegment();

        s->st = lower_t;
        s->et = lower_t + randFloat()*0.1f;

        TreeVertex *v1, *v2, *v3, *v4;
        if (i == 0 || !connected)
        {
            v1 = new TreeVertex(lower[0]);
            v2 = new TreeVertex(lower[1]);
            v3 = new TreeVertex(lower[2]);
            v4 = new TreeVertex(lower[3]);
        }
        else
        {
/*
            v1 = previousSegment->v5;
            v2 = previousSegment->v6;
            v3 = previousSegment->v7;
            v4 = previousSegment->v8;
*/
            v1 = new TreeVertex(previousSegment->v5->pos);
            v2 = new TreeVertex(previousSegment->v6->pos);
            v3 = new TreeVertex(previousSegment->v7->pos);
            v4 = new TreeVertex(previousSegment->v8->pos);
            delete [] lower; //ei tarvita
        }

        TreeVertex *v5 = new TreeVertex(upper[0]);
        TreeVertex *v6 = new TreeVertex(upper[1]);
        TreeVertex *v7 = new TreeVertex(upper[2]);
        TreeVertex *v8 = new TreeVertex(upper[3]);

        s->v1 = v1;
        s->v2 = v2;
        s->v3 = v3;
        s->v4 = v4;
        s->v5 = v5;
        s->v6 = v6;
        s->v7 = v7;
        s->v8 = v8;
        s->connected = connected;

        s->movedirection = (v1->pos + v2->pos + v3->pos + v4->pos + v5->pos + v6->pos + v7->pos + v8->pos).normalize();
        if (!rotatedPuu)
        {
            s->movedirection.y = 0;

        }

        const float harmaa = 0.1f+0.5f*randFloat();
        const float vaihtelu = 0.1f;

        if (mustavalko)
        {
            Matrix colorscale;
            const float aberr = 0.1f;
            colorscale.data[0][0] = (1-aberr)+aberr*randFloat();
            colorscale.data[1][1] = (1-aberr)+aberr*randFloat();
            colorscale.data[2][2] = (1-aberr)+aberr*randFloat();
            Vector3 basecolor = ((rand()%2)==0)?Vector3(0,0,0):Vector3(harmaus, harmaus, harmaus);
            Vector3 base = Vector3(vaihtelu,vaihtelu, vaihtelu);

            s->f1 = new TreeFace(v1, v2, v3, v4, basecolor + base*(randFloat())); //alasivu
            s->f2 = new TreeFace(v5, v6, v7, v8, basecolor + base*(randFloat())); //yläsivu
            s->f3 = new TreeFace(v1, v5, v6, v2, basecolor + base*(randFloat())); //oikea
            s->f4 = new TreeFace(v4, v8, v7, v3, basecolor + base*(randFloat())); //vasen
            s->f5 = new TreeFace(v2, v6, v7, v3, basecolor + base*(randFloat())); //taka
            s->f6 = new TreeFace(v1, v5, v8, v4, basecolor + base*(randFloat())); //etu


        }
        else
        {
            Matrix colorscale;
            const float aberr = 0.1f;
            colorscale.data[0][0] = (1-aberr)+aberr*randFloat();
            colorscale.data[1][1] = (1-aberr)+aberr*randFloat();
            colorscale.data[2][2] = (1-aberr)+aberr*randFloat();
            Vector3 basecolor = Vector3(harmaa, harmaa, harmaa)*colorscale;
            Vector3 base = Vector3(vaihtelu,vaihtelu, vaihtelu);

            s->f1 = new TreeFace(v1, v2, v3, v4, basecolor + base*(randFloat())); //alasivu
            s->f2 = new TreeFace(v5, v6, v7, v8, basecolor + base*(randFloat())); //yläsivu
            s->f3 = new TreeFace(v1, v5, v6, v2, basecolor + base*(randFloat())); //oikea
            s->f4 = new TreeFace(v4, v8, v7, v3, basecolor + base*(randFloat())); //vasen
            s->f5 = new TreeFace(v2, v6, v7, v3, basecolor + base*(randFloat())); //taka
            s->f6 = new TreeFace(v1, v5, v8, v4, basecolor + base*(randFloat())); //etu
        }
        s->f1->wireframe = wireframe;
        s->f2->wireframe = wireframe;
        s->f3->wireframe = wireframe;
        s->f4->wireframe = wireframe;
        s->f5->wireframe = wireframe;
        s->f6->wireframe = wireframe;

        s->massa = 0.7f+0.4f*randFloat();

        previousSegment = s;

        tree->treesegments.push_back(s);
    }

}

void puu_init()
{
    int i;
    
    bool rotated = rotatedPuu;//true;//(rand()%2)==0?true:false;

    treecount = 7 + (rand() % 4) + rotated?8:0;

    trees = new Tree*[treecount];

    bool mustavalko = ((rand()%4)==0)?true:false;
    kolmio =  ((rand()%4)==0)?true:false;
    kolmioituminen = 1.0f + 1.0f*randFloat();
    
    const int treesplinepoints = 10 + rand()%8;
    const int treesegmentcount = 150 + rand()%50 - rotated?25:0;

    bool wireframeallowed = ((rand()%3)==0)?true:false;
    bool connectedallowed = ((rand()%4)!=0)?true:false;
    for (int j = 0; j < treecount; j++)
    {
        bool globalwireframe = wireframeallowed?((rand()%4)==0?true:false):false;
        bool connected = false;//connectedallowed?((rand()%2)==0?true:false):false;
        trees[j] = new Tree();
        trees[j]->wireframe = globalwireframe;

        CatmullRom *spline = new CatmullRom(treesplinepoints);
 
        float xmod = (randFloat()-0.5f)*70;
        float ymod = (randFloat()-0.5f)*70;
        float randpower = 5.0f + 50*randFloat();

        float a = 2*3.141592f*randFloat();
        float r = 5*randFloat();

        Vector3 splinestart = Vector3(cosf(a), 0, sinf(a)) * r;
        Vector3 splineend = splinestart + Vector3(0, puuheight, 0);

        Matrix rotate;
        if (rotated)
        {
            rotate.makeRotation(randFloat()*2*3.141592f, randFloat()*2*3.141592f, randFloat()*2*3.141592f);
        }
        else
        {
            const float k1 = (randFloat()-0.5f)*0.5f;
            rotate.makeRotation(k1, randFloat()*2*3.141592f, 0);
        }
        for (i = 0; i < treesplinepoints; i++)
        {
            const float it = i / (float)treesplinepoints;
            Vector3 delta = Vector3(cosf(it*xmod), 0, sinf(it*ymod)) * 2 + randomSphereVector(randpower*randFloat());
            delta.y *= 0.1f;
            Vector3 splinepos = splinestart*(1-it) + splineend*it + delta;
            spline->addPoint(splinepos*rotate);
        }

        createSegmentsAroundSpline(trees[j], spline, 40, globalwireframe, connected, mustavalko, 1.2 + randFloat()*0.5f);

        int oksacount = 60 + rand()%50;

        for (int k = 0; k < oksacount; k++)
        {
            if (rand()%2)
            {
                int branchsplinepoints = 8;
                CatmullRom *spline2 = new CatmullRom(branchsplinepoints);

                Vector3 branchstart = spline->getValue(0.2f+0.6f*randFloat());
                spline2->addPoint(branchstart);
                spline2->addPoint(branchstart);

                const float twistscale = 0.32f;
                const float twistscale2 = 0.3f;
                const float mintwist = 0.23f;
                const float radiusmod = 0.4f + randFloat()*0.45f;

                float a = randFloat()*2*3.141592f;
                float b = randFloat()*2*3.141592f;
                float da = (randFloat()-0.5f) * twistscale;
                float db = (randFloat()-0.5f) * twistscale;
                float da2 = (randFloat() - 0.5f)*twistscale2;
                float db2 = (randFloat() - 0.5f)*twistscale2;

                if (fabsf(da) < mintwist)
                {
                    da *= mintwist / fabsf(da);
                }
                if (fabsf(db) < mintwist)
                {
                    db *= mintwist / fabsf(db);
                }

                const float speed = 1.5f + 8.5f*randFloat();

                for (i = 0; i < branchsplinepoints; i++)
                {
                    spline2->addPoint(branchstart);
                    branchstart += sphereCoord(speed, a, b);

                    a += da;
                    b += db;
                    da += da2;
                    db += db2;
                }
                createSegmentsAroundSpline(trees[j], spline2, 15, globalwireframe, connected, mustavalko, radiusmod);

                delete spline2;
            }
        }

        delete spline;
    }
}

void puu_free()
{
}