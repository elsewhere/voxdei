#include <stdio.h>
#include "taivas.hpp"
#include "tuho.hpp"

extern Sound* sound;
extern Song *songs;
extern int currentsong;

extern int taivaskamerastart;
extern int taivaskameracount;

extern vector <Enkeli*> enkelilista;

MarchCube *mc;
MetaBalls *mb;

int polycount;
Vector3 *metavertices;
Vector3 *metanormals;
Vector3 *metacolors;

int ballCount = 30;
float rotatespeed;
Vector3 rotatekulmat;

void taivas(float aika)
{
    int i = 0;
	const float alpha = calcSaturate(aika, 0, 1, 6);

	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

    flatTausta(Vector3(0, 0, 0), aika);
	glDisable(GL_DEPTH_TEST);
	glLoadIdentity();

    static int currenttaivaskamera = taivaskamerastart;
    static float cumulativespectrum = 0.0f;
    static float spectrumlimit = 70;
    static float prevpos = 0.0f;
    static float cumtime = 0.0f;
    const float limit = 0.01f;

    float dt = aika - prevpos;
    prevpos = aika;
    cam->useCamera(currenttaivaskamera);

    cumtime += dt;
    //palojen siirto listasta toiseen

    const int maxpalatremoved = 3;
    int removed = 0;
/*
    while (cumtime > limit)
    {
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

*/
    float array[1024];

    float spectrum = 0;
    sound->getSpectrum(&songs[currentsong], array, 1024);

    for (i = 0; i < 1023; i++)
    {
        spectrum += array[i];
    }

    cumulativespectrum += spectrum;

    //kameran liikutus
    if (cumulativespectrum > spectrumlimit)
    {
        cumulativespectrum -= spectrumlimit;
        currenttaivaskamera++;
        if (currenttaivaskamera >= taivaskamerastart+ taivaskameracount)
        {
            currenttaivaskamera = taivaskamerastart;
        }
        spectrumlimit = 100 + 150*randFloat();
    }

    //enkeleiden päivitys
    vector<Enkeli*>::iterator ei;
    for (ei = enkelilista.begin(); ei < enkelilista.end(); ei++)
    {
        Enkeli *e = *ei;
        e->a += e->aspeed * dt;
        e->b += e->bspeed * dt;
    }

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

    int index = 0;

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    
    const float valofade = 1-calcPosFloat(aika, 0.8f, 1.0f);
    float valospeed = 3.0f;
    float aval = 0.2f+spectrum*0.04f;
	const float ambientvalo[] = {aval, aval,aval, 1};
	const float specularvalo[] = {1, 1, 1, 1};
	const float diffusevalo[] = {1, 1, 1, 1};
	const float valoposition[] = {50*cosf(aika*11*valospeed), 47*cosf(aika*18*valospeed), 66*sinf(aika*6*valospeed), 1.0f };

	glLightfv(GL_LIGHT1, GL_POSITION, valoposition);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientvalo);
  	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffusevalo);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularvalo);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
    

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    glEnable(GL_COLOR_MATERIAL);

    glPushMatrix();
    glRotatef(aika*rotatespeed, rotatekulmat.x, rotatekulmat.y, rotatekulmat.z);
    glBegin(GL_TRIANGLES);

    for (i = 0; i < polycount; i++)
    {
        glColor3fv((float *)&(metacolors[i]*valofade));
        glNormal3fv((float*)&metanormals[index]);
        glVertex3fv((float*)&metavertices[index++]);
//        glNormal3fv((float*)&metanormals[index]);
        glVertex3fv((float*)&metavertices[index++]);
//        glNormal3fv((float*)&metanormals[index]);
        glVertex3fv((float*)&metavertices[index++]);
    }
    glEnd();
    glPopMatrix();

 	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT1);
	glDisable(GL_NORMALIZE);
    glDisable(GL_COLOR_MATERIAL);
   

}
	
void taivas_init()
{
    ballCount = 30 + randFloat()*10;
	mb = new MetaBalls();
	mc = new MarchCube(-10, 10, 10, 10, -10, -10, 0.5, -0.5f, -0.5);
	mc->setMetaBalls(mb, 0.2f+0.3f*randFloat());
    mc->computeMetaBalls();
    mc->calculate(4.0f+3*randFloat());

    rotatespeed = 200 + 700*randFloat();
    rotatekulmat = randomSphereVector(1);
}

void taivas_free()
{
}


// ----------
// Cubes
// ----------


MarchCube::MarchCube(	float start_x, float start_y, float start_z, float end_x, float end_y, float end_z, float step_x, float step_y, float step_z)
{
	int idx, x, y, z;
	this->start_x = start_x;
	this->start_y = start_y;
	this->start_z = start_z;

	this->end_x = end_x;
	this->end_y = end_y;
	this->end_z = end_z;

	this->step_x = step_x;
	this->step_y = step_y;
	this->step_z = step_z;

	this->size_x = (int) ((this->end_x - this->start_x) / this->step_x);
	this->size_y = (int) ((this->end_y - this->start_y) / this->step_y);
	this->size_z = (int) ((this->end_z - this->start_z) / this->step_z);

	this->vertices = new MetaVertex[this->size_x * this->size_y * this->size_z];

	for (z = 0; z < this->size_z; z++)
	{
		for (y = 0; y < this->size_y; y++)
		{
			for (x = 0; x < this->size_x; x++)
			{
				idx = x + y*this->size_y + z * this->size_y * this->size_z;

				this->vertices[idx].x_pos = this->start_x + this->step_x * x;
				this->vertices[idx].y_pos = this->start_y + this->step_y * y;
				this->vertices[idx].z_pos = this->start_z + this->step_z * z;
				this->vertices[idx].flux = 0.0;
				this->vertices[idx].inside = false;
				this->vertices[idx].normal_x = 0.0;
				this->vertices[idx].normal_y = 0.0;
				this->vertices[idx].normal_z = 0.0;
			}
		}
	}
}


MarchCube::~MarchCube()
{
	delete [] vertices;
}

void MarchCube::computeMetaBalls()
{
	int idx, x, y, z;
	for (z = 1; z < (this->size_z - 1); z++)
	{
		for (y = 1; y < (this->size_y - 1); y++)
		{
			for (x = 1; x < (this->size_x - 1); x++)
			{
				idx = x + y*this->size_y + z * this->size_y * this->size_z;

				this->vertices[idx].flux = this->mb->get_vertex_value(this->vertices[idx]);
				
				if (this->vertices[idx].flux > this->MetaBallsIsoVal)
					this->vertices[idx].inside = true;
				else
					this->vertices[idx].inside = false;


				this->vertices[idx].normal_x = this->vertices[idx - 1].flux - this->vertices[idx+1].flux;
				this->vertices[idx].normal_y = this->vertices[idx - this->size_y].flux - this->vertices[idx + this->size_y].flux;
				this->vertices[idx].normal_z = this->vertices[idx - (this->size_y * this->size_z)].flux - this->vertices[idx + (this->size_y * this->size_z)].flux; 
			}
		}
	}
}

void MarchCube::calculate(float scale)
{
	int idx, x, y, z;
	short lookup = 0;

    vector<Vector3*> tempVertices;
    vector<Vector3*> tempNormals;

	for (z = 0; z < (this->size_z - 1); z++)
	{
		for (y = 0; y < (this->size_y - 1); y++)
		{
			for (x = 0; x < (this->size_x - 1); x++)
			{
				idx = x + y*this->size_y + z * this->size_y * this->size_z;
				if (this->vertices[idx].inside) lookup |= 128;
				if (this->vertices[idx+1].inside) lookup |= 64;
				if (this->vertices[idx+1+this->size_y].inside) lookup |= 4;
				if (this->vertices[idx + this->size_y].inside) lookup |= 8;
				if (this->vertices[idx + (this->size_y * this->size_z)].inside) lookup |= 16;
				if (this->vertices[idx + 1 + (this->size_y * this->size_z)].inside) lookup |= 32;
				if (this->vertices[idx + 1 + this->size_y + (this->size_y * this->size_z)].inside) lookup |= 2;
				if (this->vertices[idx + this->size_y + (this->size_y * this->size_z)].inside) lookup |= 1;

				if ((lookup != 0) && (lookup != 255))
				{
					// 0 - 1
					if (edgeTable[lookup] & 1) 
						this->verts[0] = this->mb->interpolate(	this->vertices[idx + this->size_y + (this->size_y * this->size_z)],
											this->vertices[idx + 1 + this->size_y + (this->size_y * this->size_z)]);

					// 1 - 2
					if (edgeTable[lookup] & 2) 
						this->verts[1] = this->mb->interpolate(	this->vertices[idx + 1 + this->size_y + (this->size_y * this->size_z)],
											this->vertices[idx + 1 + this->size_y]);

					// 2 - 3
					if (edgeTable[lookup] & 4) 
						this->verts[2] = this->mb->interpolate(	this->vertices[idx + 1 + this->size_y],
											this->vertices[idx + this->size_y]);

					// 3 - 0
					if (edgeTable[lookup] & 8) 
						this->verts[3] = this->mb->interpolate(	this->vertices[idx + this->size_y],
											this->vertices[idx + this->size_y + (this->size_y * this->size_z)]);
			
					// 4 - 5
					if (edgeTable[lookup] & 16) 
						this->verts[4] = this->mb->interpolate(	this->vertices[idx + (this->size_y * this->size_z)],
											this->vertices[idx + 1 + (this->size_y * this->size_z)]);

					// 5 - 6
					if (edgeTable[lookup] & 32) 
						this->verts[5] = this->mb->interpolate(	this->vertices[idx + 1 + (this->size_y * this->size_z)],
											this->vertices[idx + 1]);

					// 6 - 7
					if (edgeTable[lookup] & 64) 
						this->verts[6] = this->mb->interpolate(	this->vertices[idx + 1],
											this->vertices[idx]);

					// 7 - 4
					if (edgeTable[lookup] & 128) 
						this->verts[7] = this->mb->interpolate(	this->vertices[idx],
											this->vertices[idx + (this->size_y * this->size_z)]);

					// 0 - 4
					if (edgeTable[lookup] & 256)
						this->verts[8] = this->mb->interpolate(	this->vertices[idx + this->size_y + (this->size_y * this->size_z)],
											this->vertices[idx + (this->size_y * this->size_z)]);

					// 1 - 5
					if (edgeTable[lookup] & 512) 
						this->verts[9] = this->mb->interpolate(	this->vertices[idx + 1 + this->size_y + (this->size_y * this->size_z)],
											this->vertices[idx + 1 + (this->size_y * this->size_z)]);

					// 2 - 6
					if (edgeTable[lookup] & 1024) 
						this->verts[10] = this->mb->interpolate(	this->vertices[idx + 1 + this->size_y],
											this->vertices[idx + 1]);

					// 3 - 7
					if (edgeTable[lookup] & 2048) 
						this->verts[11] = this->mb->interpolate(	this->vertices[idx + this->size_y],
											this->vertices[idx]);

					glColor3f(randFloat(),randFloat(),randFloat());
						
					int i, j;
					for (i = 0; triTable[lookup][i] != -1; i+=3)
					{
						for (j = i; j < (i+3); j++)
						{
/*
								glNormal3f( (float) this->verts[triTable[lookup][j]].normal_x, 
											(float) this->verts[triTable[lookup][j]].normal_y, 
											(float) this->verts[triTable[lookup][j]].normal_z);

								glVertex3f(	(float) this->verts[triTable[lookup][j]].x_pos,
											(float) this->verts[triTable[lookup][j]].y_pos,
											(float) this->verts[triTable[lookup][j]].z_pos
											);
*/

                            Vector3 *tV = new Vector3(this->verts[triTable[lookup][j]].x_pos, 
                                                           this->verts[triTable[lookup][j]].y_pos,
                                                           this->verts[triTable[lookup][j]].z_pos);
                            Vector3 *tN = new Vector3(this->verts[triTable[lookup][j]].normal_x, 
                                                           this->verts[triTable[lookup][j]].normal_y,
                                                           this->verts[triTable[lookup][j]].normal_z);

                            tempVertices.push_back(tV);
                            tempNormals.push_back(tN);
						}
					}
				}
				lookup = 0;
			}
		}
	}

    vector<Vector3*>::iterator it;

    int count = tempVertices.size();
    polycount = count / 3;

    metavertices = new Vector3[count];
    metanormals = new Vector3[count];
    metacolors = new Vector3[polycount];

    int index = 0;
    for (it = tempVertices.begin(); it < tempVertices.end(); it++)
    {
        Vector3 *t = *it;
//        fprintf(loki, "laitetaan sinne %f %f %f\n", t->x, t->y, t->z);
        metavertices[index] = (*t)*scale;
        index++;
    }

    index = 0;
    for (it = tempNormals.begin(); it < tempNormals.end(); it++)
    {
        Vector3 *t = *it;
        metanormals[index] = *t;
        index++;
    }


    int varitys = rand()%3;

    float varianssi = 0.1f+0.3f*powf(randFloat(), 0.4f);
    float variscale = 0.2f+randFloat()*0.2f;

    bool suunta = ((rand()%2)==0)?true:false;
    for (int i = 0; i < polycount; i++)
    {
        float harmaa = 0.0f;
        
        Vector3 total = metavertices[(i*3)]+metavertices[(i*3)+1]+metavertices[(i*3)+2];
        total *= 0.33333f;

        switch(varitys)
        {
            case 0: harmaa = 0.5f - varianssi*0.5f + randFloat()*varianssi; break;
            case 1: harmaa = ((rand()%2)==0)?1:0; break;
            case 2: harmaa = 0.5f+0.5f*cosf((suunta?total.y:total.x)*variscale); break;
        }
        Vector3 normal = metanormals[(i*3)];

        metacolors[i] = Vector3(harmaa, harmaa,harmaa);
    }
    for (it = tempVertices.begin(); it < tempVertices.end(); it++)
    {
        Vector3 *t = *it;
        delete t;
    }
    for (it = tempNormals.begin(); it < tempNormals.end(); it++)
    {
        Vector3 *t = *it;
        delete t;
    }
}

void MarchCube::setMetaBalls(MetaBalls *mb, float iso_value)
{
	this->mb = mb;
	this->mb->set_iso_value(iso_value);
	this->MetaBallsIsoVal = iso_value;
}

// ----------
// Balls
// ----------

MetaBalls::MetaBalls()
{
	numBalls = ballCount;
	meta_points = new MetaBall[numBalls];
	for(int i=0; i<ballCount; i++)
	{
		meta_points[i].power = 0.15+(rand()%10)*0.1f;
		meta_points[i].x_pos = -8.0+rand()%16;
		meta_points[i].y_pos = -8.0+rand()%16;
		meta_points[i].z_pos = -8.0+rand()%16;
		meta_points[i].move_mod = -4.0+(rand()%100)*0.08f;
		meta_points[i].actpos = Vector3();//Vector3(meta_points[i].x_pos, meta_points[i].y_pos, meta_points[i].z_pos);
	}
}

MetaBalls::~MetaBalls()
{
	if(meta_points!=0)
		delete [] meta_points;
}

void MetaBalls::set_iso_value(float iv) 
{
	this->iso_value = iv;
}

void MetaBalls::move_ball(int idx, float x, float y, float z)
{
	meta_points[idx].x_pos = meta_points[idx].actpos.x+x;
	meta_points[idx].y_pos = meta_points[idx].actpos.y+y;
	meta_points[idx].z_pos = meta_points[idx].actpos.z+z;
}

MetaVertex MetaBalls::interpolate(MetaVertex v1, MetaVertex v2)
{
	MetaVertex v;
	float diff;

	diff = (this->iso_value - v1.flux) / (v2.flux - v1.flux);

	v.x_pos = v1.x_pos + (v2.x_pos - v1.x_pos) * diff;
	v.y_pos = v1.y_pos + (v2.y_pos - v1.y_pos) * diff;
	v.z_pos = v1.z_pos + (v2.z_pos - v1.z_pos) * diff;
	v.flux = (v1.flux + v2.flux) * 0.5f;

	v.normal_x = v1.normal_x + (v2.normal_x - v1.normal_x) * diff;
	v.normal_y = v1.normal_y + (v2.normal_y - v1.normal_y) * diff;
	v.normal_z = v1.normal_z + (v2.normal_z - v1.normal_z) * diff;

	return v;
}

float MetaBalls::get_vertex_value(MetaVertex v)
{
	float flux = 0.0f;

	for (i = 0; i < numBalls; i++)
	{
		length_x = meta_points[i].x_pos - v.x_pos;
		length_y = meta_points[i].y_pos - v.y_pos;
		length_z = meta_points[i].z_pos - v.z_pos;
	
		flux += fabs(meta_points[i].power) * meta_points[i].power / (
					length_x * length_x + 
					length_y * length_y +
					length_z * length_z + 1.0f);
	}

	return flux;
}

