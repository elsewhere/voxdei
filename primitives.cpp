#include "primitives.hpp"
#pragma warning (disable : 4305)
/*
void tetrahedron();
void octahedron();
void hexahedron();
void icosahedron();
void dodecahedron();
*/
/*
static float tetravertices[4][3] = 
{
	-1, -1, -1,
	 1,  1, -1,
	 1, -1,  1,
	-1,  1,  1,
};
static int tetrafaces[4][3] = 
{
	1, 2, 3,
	1, 0, 2,
	3, 2, 0,
	0, 1, 3,
};
*/

static Vector3 tetravertices[4] = 
{
	Vector3(-1, -1, -1),
	Vector3(1, 1, -1),
	Vector3(1, -1, 1),
	Vector3(-1, 1, 1)
};

static int tetrafaces[4][3] = 
{
	1,2,3,
	1,0,2,
	3,2,0,
	0,1,3
};

static Vector3 octavertices[6] = 
{
	Vector3(1,  0,  0),
	Vector3(0, -1,  0),
	Vector3(-1,  0,  0),
	Vector3(0,  1,  0),
	Vector3(0,  0,  1),
	Vector3(0,  0, -1)
};

static int octafaces[8][3] = 
{
	4, 0, 1,
	4, 1, 2,
	4, 2, 3,
	4, 3, 0,
	5, 1, 0,
	5, 2, 1,
	5, 3, 2,
	5, 0, 3,
};

static Vector3 cubevertices[8] = 
{
	Vector3(-1, -1, -1),
	Vector3( 1, -1, -1),
	Vector3( 1,  1, -1),
	Vector3(-1,  1, -1),
	Vector3(-1, -1,  1),
	Vector3( 1, -1,  1),
	Vector3( 1,  1,  1),
	Vector3(-1,  1,  1),
};
static int cubefaces[6][4] = 
{
	0, 1, 2, 3,
	5, 4, 7, 6,
	6, 2, 1, 5,
	3, 7, 4, 0,
	7, 3, 2, 6,
	5, 1, 0, 4,
};

static Vector3 icosavertices[12] = 
{
	Vector3(0, -0.52573,  0.85065),
	Vector3(0.85065,  0,  0.52573),
	Vector3(0.85065,  0, -0.52573),
	Vector3(-0.85065,  0, -0.52573),
	Vector3(-0.85065,  0,  0.52573),
	Vector3(-0.52573,  0.85065,  0),
	Vector3(0.52573,  0.85065,  0),
	Vector3(0.52573, -0.85065,  0),
	Vector3(-0.52573, -0.85065,  0),
	Vector3(0, -0.52573, -0.85065),
	Vector3(0,  0.52573, -0.85065),
	Vector3(0,  0.52573,  0.85065)
};

static int icosafaces[20][3] = 
{
	6, 2, 1, 
	2, 7, 1, 
	5, 4, 3, 
	8, 3, 4, 
	11, 5, 6, 
	10, 6, 5, 
	2, 10, 9, 
	3, 9, 10, 
	9, 8, 7, 
	0, 7, 8, 
	1, 0, 11, 
	4, 11, 0, 
	10, 2, 6, 
	11, 6, 1, 
	10, 5, 3, 
	11, 4, 5, 
	9, 7, 2, 
	0, 1, 7, 
	8, 9, 3, 
	0, 8, 4, 
};

static Vector3 dodecavertices[20] = 
{
	Vector3(-0.57735, -0.57735, 0.57735),
	Vector3(0.93417, 0.35682, 0),
	Vector3(0.93417, -0.35682, 0),
	Vector3(-0.93417, 0.35682, 0),
	Vector3(-0.93417, -0.35682, 0),
	Vector3(0, 0.93417, 0.35682),
	Vector3(0, 0.93417, -0.35682),
	Vector3(0.35682, 0, -0.93417),
	Vector3(-0.35682, 0, -0.93417),
	Vector3(0, -0.93417, -0.35682),
	Vector3(0, -0.93417, 0.35682),
	Vector3(0.35682, 0, 0.93417),
	Vector3(-0.35682, 0, 0.93417),
	Vector3(0.57735, 0.57735, -0.57735),
	Vector3(0.57735,0.57735,0.57735),
	Vector3(-0.57735, 0.57735, -0.57735),
	Vector3(-0.57735, 0.57735, 0.57735),
	Vector3(0.57735, -0.57735, -0.57735),
	Vector3(0.57735, -0.57735, 0.57735),
	Vector3(-0.57735, -0.57735, -0.57735)
};
static int dodecafaces[12][5] = 
{
	1, 2, 18, 11, 14, 
	1, 13, 7, 17, 2, 
	3, 4, 19, 8, 15, 
	3, 16, 12, 0, 4, 
	3, 15, 6, 5, 16, 
	1, 14, 5, 6, 13, 
	2, 17, 9, 10, 18, 
	4, 0, 10, 9, 19, 
	7, 8, 19, 9, 17, 
	6, 15, 8, 7, 13, 
	5, 14, 11, 12, 16, 
	10, 0, 12, 11, 18, 
};

void tetrahedron()
{
	int i;
	int facecount = 4;
	glBegin(GL_TRIANGLES);

	for (i=0;i<facecount;i++)
	{
		Vector3 v1 = tetravertices[tetrafaces[i][0]];
		Vector3 v2 = tetravertices[tetrafaces[i][1]];
		Vector3 v3 = tetravertices[tetrafaces[i][2]];

		Vector3 n = (v3-v1).crossProduct(v2-v1).normalize();
		glNormal3fv((float *)&n);
		glVertex3fv((float *)&v1);
		glVertex3fv((float *)&v2);
		glVertex3fv((float *)&v3);
	}
	glEnd();
}

void octahedron()
{
	int i;
	int facecount = 8;
	glBegin(GL_TRIANGLES);

	for (i=0;i<facecount;i++)
	{
		Vector3 v1 = octavertices[octafaces[i][0]];
		Vector3 v2 = octavertices[octafaces[i][1]];
		Vector3 v3 = octavertices[octafaces[i][2]];

		Vector3 n = (v3-v1).crossProduct(v2-v1).normalize();
		glNormal3fv((float *)&n);
		glVertex3fv((float *)&v1);
		glVertex3fv((float *)&v2);
		glVertex3fv((float *)&v3);
	}
	glEnd();
}

void hexahedron()
{
	int i;
	int facecount = 6;
	glBegin(GL_QUADS);

	for (i=0;i<facecount;i++)
	{
		Vector3 v1 = cubevertices[cubefaces[i][0]];
		Vector3 v2 = cubevertices[cubefaces[i][1]];
		Vector3 v3 = cubevertices[cubefaces[i][2]];
		Vector3 v4 = cubevertices[cubefaces[i][3]];

		Vector3 n = (v3-v1).crossProduct(v2-v1).normalize();
		glNormal3fv((float *)&n);
		glVertex3fv((float *)&v1);
		glVertex3fv((float *)&v2);
		glVertex3fv((float *)&v3);
		glVertex3fv((float *)&v4);
	}
	glEnd();
}

void icosahedron()
{
	int i;
	int facecount = 20;
	glBegin(GL_TRIANGLES);

	for (i=0;i<facecount;i++)
	{
		Vector3 v1 = icosavertices[icosafaces[i][0]];
		Vector3 v2 = icosavertices[icosafaces[i][1]];
		Vector3 v3 = icosavertices[icosafaces[i][2]];

		Vector3 n = (v3-v1).crossProduct(v2-v1).normalize();
		glNormal3fv((float *)&n);
		glVertex3fv((float *)&v1);
		glVertex3fv((float *)&v2);
		glVertex3fv((float *)&v3);
	}
	glEnd();
}

void dodecahedron()
{
	int i;
	int facecount = 12;

	for (i=0;i<facecount;i++)
	{
		Vector3 v1 = dodecavertices[dodecafaces[i][0]];
		Vector3 v2 = dodecavertices[dodecafaces[i][1]];
		Vector3 v3 = dodecavertices[dodecafaces[i][2]];
		Vector3 v4 = dodecavertices[dodecafaces[i][3]];
		Vector3 v5 = dodecavertices[dodecafaces[i][4]];

		Vector3 n = (v3-v1).crossProduct(v2-v1).normalize();
		glBegin(GL_POLYGON);
		glNormal3fv((float *)&n);
		glVertex3fv((float *)&v1);
		glVertex3fv((float *)&v2);
		glVertex3fv((float *)&v3);
		glVertex3fv((float *)&v4);
		glVertex3fv((float *)&v5);
		glEnd();
	}
	glEnd();
}


void wireCube(float size)
{
	if (size <= 0) return;
	Vector3 upleftback = Vector3( -size, -size, -size);
	Vector3 upleftfront = Vector3(-size, -size, +size);
	Vector3 uprightback = Vector3( size, -size, -size);
	Vector3 uprightfront = Vector3(size, -size, +size);

	Vector3 downleftback = Vector3( -size, size, -size);
	Vector3 downleftfront = Vector3(-size, size, +size);
	Vector3 downrightback = Vector3( size, size, -size);
	Vector3 downrightfront = Vector3(size, size, +size);
	
	glBegin(GL_LINES);

	//yläsivu vasen
	glVertex3fv((float *)&upleftfront);
	glVertex3fv((float *)&upleftback);
	//yläsivu oikea
	glVertex3fv((float *)&uprightfront);
	glVertex3fv((float *)&uprightback);
	//yläsivu etu
	glVertex3fv((float *)&uprightfront);
	glVertex3fv((float *)&upleftfront);
	//yläsivu taka
	glVertex3fv((float *)&uprightback);
	glVertex3fv((float *)&upleftback);

	//alasivu vasen
	glVertex3fv((float *)&downleftfront);
	glVertex3fv((float *)&downleftback);
	//alasivu oikea
	glVertex3fv((float *)&downrightfront);
	glVertex3fv((float *)&downrightback);
	//alasivu etu
	glVertex3fv((float *)&downrightfront);
	glVertex3fv((float *)&downleftfront);
	//alasivu taka
	glVertex3fv((float *)&downrightback);
	glVertex3fv((float *)&downleftback);

	//oikea sivu ylä
	glVertex3fv((float *)&uprightfront);
	glVertex3fv((float *)&uprightback);
	//oikea sivu ala
	glVertex3fv((float *)&downrightfront);
	glVertex3fv((float *)&downrightback);
	//oikea sivu etu
	glVertex3fv((float *)&uprightfront);
	glVertex3fv((float *)&downrightfront);
	//oikea sivu taka
	glVertex3fv((float *)&uprightback);
	glVertex3fv((float *)&downrightback);

	//vasen sivu ylä
	glVertex3fv((float *)&upleftfront);
	glVertex3fv((float *)&upleftback);
	//vasen sivu ala
	glVertex3fv((float *)&downleftfront);
	glVertex3fv((float *)&downleftback);
	//vasen sivu etu
	glVertex3fv((float *)&upleftfront);
	glVertex3fv((float *)&downleftfront);
	//vasen sivu taka
	glVertex3fv((float *)&upleftback);
	glVertex3fv((float *)&downleftback);

	//etusivu ylä
	glVertex3fv((float *)&uprightfront);
	glVertex3fv((float *)&upleftfront);
	//etusivu ala
	glVertex3fv((float *)&downrightfront);
	glVertex3fv((float *)&downleftfront);
	//etusivu oikea
	glVertex3fv((float *)&uprightfront);
	glVertex3fv((float *)&downrightfront);
	//etusivu vasen
	glVertex3fv((float *)&upleftfront);
	glVertex3fv((float *)&downleftfront);

	//takasivu ylä
	glVertex3fv((float *)&uprightback);
	glVertex3fv((float *)&upleftback);
	//takasivu ala
	glVertex3fv((float *)&downrightback);
	glVertex3fv((float *)&downleftback);
	//takasivu oikea
	glVertex3fv((float *)&uprightback);
	glVertex3fv((float *)&downrightback);
	//takasivu vasen
	glVertex3fv((float *)&upleftback);
	glVertex3fv((float *)&downleftback);

	glEnd();
}

void flatTausta(Vector3 &color, float alpha)
{
	glDisable(GL_DEPTH_TEST);
    glDepthMask(0);
	glDisable(GL_TEXTURE_2D);

	perspective2D(1,1);
	glLoadIdentity();

	glBegin(GL_QUADS);
    glColor4f(color.x, color.y, color.z, alpha);

	glVertex2f(0, 0);
	glVertex2f(1, 0);
	glVertex2f(1, 1);
	glVertex2f(0, 1);

	glEnd();

	perspective3D();
	glLoadIdentity();
    glDepthMask(1);
}

void circle(Vector3 &kohta, float radius)
{
	int j;
	const int ympyrastrips = 16;
	glBegin(GL_LINE_STRIP);
	for (j=0;j<ympyrastrips;j++)
	{
		const float ympkohta = 2*3.141592f*j / (ympyrastrips-1);
		Vector3 v = kohta + Vector3((float)sin(ympkohta), (float)cos(ympkohta), 0)*radius;
		glVertex3fv((float *)&v);

	}
	glEnd();
}


void billboard(Vector3 &kohta, Vector3 &x, Vector3 &y, Vector3 &z, float scale)
{
	glBegin(GL_QUADS);
	Vector3 v1 = kohta - x*scale - y*scale;
	Vector3 v2 = kohta + x*scale - y*scale;
	Vector3 v3 = kohta + x*scale + y*scale;
	Vector3 v4 = kohta - x*scale + y*scale;

	glTexCoord2f(0, 0);
	glVertex3fv((float *)&v1);
	glTexCoord2f(1, 0);
	glVertex3fv((float *)&v2);
	glTexCoord2f(1, 1);
	glVertex3fv((float *)&v3);
	glTexCoord2f(0, 1);
	glVertex3fv((float *)&v4);
	glEnd();
}
// eri tex coordit
void billboard2(Vector3 &kohta, Vector3 &x, Vector3 &y, Vector3 &z, float scale)
{
	glBegin(GL_QUADS);
	Vector3 v1 = kohta - x*scale - y*scale;
	Vector3 v2 = kohta + x*scale - y*scale;
	Vector3 v3 = kohta + x*scale + y*scale;
	Vector3 v4 = kohta - x*scale + y*scale;

	glTexCoord2f(0, 0);
	glVertex3fv((float *)&v1);
	glTexCoord2f(0, 1);
	glVertex3fv((float *)&v2);
	glTexCoord2f(1, 1);
	glVertex3fv((float *)&v3);
	glTexCoord2f(1, 0);
	glVertex3fv((float *)&v4);
	glEnd();
}


