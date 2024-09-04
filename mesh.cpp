#include "mesh.hpp"
#include "matikka.h"

Mesh::Mesh()
{
	vertexcount = 0;
	facecount = 0;
	edgecount = 0;

	vertices = 0;
	faces = 0;
	edges = 0;
}

Mesh::~Mesh()
{
	destroy();
}
void Mesh::destroy()
{
	if (vertices)
		delete [] vertices;
    if (normals)
        delete [] normals;
    if (uv)
        delete [] uv;
	if (faces)
		delete [] faces;
	if (edges)
		delete [] edges;

	vertexcount = 0;
	edgecount = 0;
	facecount = 0;
}

void Mesh::drawTexturedWithLight(Vector3 &light, Vector3 &color, float ambient)
{
    int i;

    glBegin(GL_TRIANGLES);
    for (i=0;i<facecount;i++)
    {
        Face &f = faces[i];
        Vector3 &v1 = vertices[faces[i].v1];
        Vector3 &v2 = vertices[faces[i].v2];
        Vector3 &v3 = vertices[faces[i].v3];

        TexCoord &uv1 = uv[faces[i].v1];
        TexCoord &uv2 = uv[faces[i].v2];
        TexCoord &uv3 = uv[faces[i].v3];

        Vector3 &n1 = normals[faces[i].v1];
        Vector3 &n2 = normals[faces[i].v2];
        Vector3 &n3 = normals[faces[i].v3];

        //"hieno" valaistusmalli
        float l1 = light.dotProduct(n1);
        float l2 = light.dotProduct(n2);
        float l3 = light.dotProduct(n3);
        if (l1 < ambient)
            l1 = ambient;
        if (l2 < ambient)
            l2 = ambient;
        if (l3 < ambient)
            l3 = ambient;
/*
        l1 = 1;
        l2 = 1;
        l3 = 1;
*/
//        if (l1 > 0 && l2 > 0 && l3 > 0)
//        {
            Vector3 color1 = color*l1;
            Vector3 color2 = color*l2;
            Vector3 color3 = color*l3;

            glColor4f(color1.x, color1.y, color1.z, 1);
            glTexCoord2fv((float *)&uv1);
            glVertex3fv((float *)&v1);
            glColor4f(color2.x, color2.y, color2.z, 1);
            glTexCoord2fv((float *)&uv2);
            glVertex3fv((float *)&v2);
            glColor4f(color3.x, color3.y, color3.z, 1);
            glTexCoord2fv((float *)&uv3);
            glVertex3fv((float *)&v3);
//        }
    }
    glEnd();
}
void Mesh::drawTextured()
{
    int i;

    glBegin(GL_TRIANGLES);
    for (i=0;i<facecount;i++)
    {
        Face &f = faces[i];
        Vector3 &v1 = vertices[faces[i].v1];
        Vector3 &v2 = vertices[faces[i].v2];
        Vector3 &v3 = vertices[faces[i].v3];

        TexCoord &uv1 = uv[faces[i].v1];
        TexCoord &uv2 = uv[faces[i].v2];
        TexCoord &uv3 = uv[faces[i].v3];

        Vector3 &n1 = normals[faces[i].v1];
        Vector3 &n2 = normals[faces[i].v2];
        Vector3 &n3 = normals[faces[i].v3];

            glTexCoord2fv((float *)&uv1);
            glVertex3fv((float *)&v1);
            glTexCoord2fv((float *)&uv2);
            glVertex3fv((float *)&v2);
            glTexCoord2fv((float *)&uv3);
            glVertex3fv((float *)&v3);
    }
    glEnd();
}

void Mesh::drawSolidColorWithLight(Vector3 &light, Vector3 &color, float ambient)
{
    int i;

    glBegin(GL_TRIANGLES);
    for (i=0;i<facecount;i++)
    {
        Face &f = faces[i];
        Vector3 &v1 = vertices[faces[i].v1];
        Vector3 &v2 = vertices[faces[i].v2];
        Vector3 &v3 = vertices[faces[i].v3];

        Vector3 &n1 = normals[faces[i].v1];
        Vector3 &n2 = normals[faces[i].v2];
        Vector3 &n3 = normals[faces[i].v3];

        //"hieno" valaistusmalli

        float l1 = light.dotProduct(n1);
        float l2 = light.dotProduct(n2);
        float l3 = light.dotProduct(n3);
        if (l1 < ambient)
            l1 = ambient;
        if (l2 < ambient)
            l2 = ambient;
        if (l3 < ambient)
            l3 = ambient;

        if (l1 > 0 && l2 > 0 && l3 > 0)
        {
            Vector3 color1 = color*l1;
            Vector3 color2 = color*l2;
            Vector3 color3 = color*l3;

            glColor4f(color1.x, color1.y, color1.z, 1);
            glVertex3fv((float *)&v1);
            glColor4f(color2.x, color2.y, color2.z, 1);
            glVertex3fv((float *)&v2);
            glColor4f(color3.x, color3.y, color3.z, 1);
            glVertex3fv((float *)&v3);
        }
    }
    glEnd();

}

void Mesh::drawSolidColor()
{
    int i;

    glBegin(GL_TRIANGLES);
    for (i=0;i<facecount;i++)
    {
        Face &f = faces[i];
        Vector3 &v1 = vertices[faces[i].v1];
        Vector3 &v2 = vertices[faces[i].v2];
        Vector3 &v3 = vertices[faces[i].v3];

        glVertex3fv((float *)&v1);
        glVertex3fv((float *)&v2);
        glVertex3fv((float *)&v3);
    }
    glEnd();

}

void Mesh::calculateNormals()
{
	int i;
	//facenormaalit
	for (i=0;i<facecount;i++)
	{
		Vector3 v1 = vertices[faces[i].v1];
		Vector3 v2 = vertices[faces[i].v2];
		Vector3 v3 = vertices[faces[i].v3];
		Vector3 normal = (v2-v1).crossProduct(v3-v1);

		faces[i].normal = normal.normalize();
	}

    //verteksinormaalit
	for (i=0;i<vertexcount;i++)
	{
		normals[i]= Vector3(0, 0, 0);
	}
	for (i=0;i<facecount;i++)
	{
		Vector3 normal = faces[i].normal;
		normals[faces[i].v1] += normal;
		normals[faces[i].v2] += normal;
		normals[faces[i].v3] += normal;
	}
	for (i=0;i<vertexcount;i++)
	{
		normals[i].normalize();
	}
}

void Mesh::scale(float max)
{
    int i;

    float m = 0.0f;
    for (i=0;i<vertexcount;i++)
    {
        float l = vertices[i].length();
        if (l > m)
            m = l;
    }

    if (m > 0.0001f)
    {
        for (i=0;i<vertexcount;i++)
        {
            vertices[i] *= (1.0f/m)*max;
        }
    }
}

void Mesh::buildEmpty(int xres, int yres)
{
	vertexcount = xres*yres;
	facecount = xres*yres*2;

    vertices = new Vector3[vertexcount];
    normals = new Vector3[vertexcount];
    uv = new TexCoord[vertexcount];

	faces = new Face[facecount];
}

void Mesh::buildAsteroid(int seed, int xres, int yres, float size, float texturescale)
{
	int x, y;

    vertexcount = xres*yres;
    facecount = vertexcount*2;

    vertices = new Vector3[vertexcount];
    normals = new Vector3[vertexcount];
    faces = new Face[facecount];
    uv = new TexCoord[vertexcount];

	for (y=0;y<yres;y++)
	{
		for (x=0;x<xres;x++)
		{
			float a1 = x/(float)(xres-1);
			float a2 = y/(float)(yres-1);
			float r = size + (randFloat()-0.5f)*(size*0.5f);
            Vector3 pos = sphereCoord(1, a1*3.141592f, a2*2*3.141592f);
            normals[x+y*xres] = pos;
            vertices[x+y*xres] = pos*r;
			uv[x+y*xres].u = a1*texturescale;
			uv[x+y*xres].v = a2*texturescale;
		}
	}
    makeFaces(xres, yres);
//    calculateNormals();
}
void Mesh::buildSphere(float radius, int xres, int yres, float texturescale)
{
	vertexcount = xres*yres;
	facecount = xres*yres*2;
	vertices = new Vector3[vertexcount];
    normals = new Vector3[vertexcount];
    uv = new TexCoord[vertexcount];
	faces = new Face[facecount];

	int x, y;

	for (y=0;y<yres;y++)
	{
		for (x=0;x<xres;x++)
		{
			const float xkohta = x / (float)(xres-1);
			const float ykohta = y / (float)(yres-1);

			Vector3 pos = sphereCoord(1, ykohta*3.141592f, xkohta*2*3.141592f);
			vertices[x+y*xres] = pos*radius;
            normals[x+y*xres] = pos;
			uv[x+y*xres].u = xkohta*texturescale;
			uv[x+y*xres].v = ykohta*texturescale;
		}
	}

    makeFaces(xres, yres);
}

void Mesh::makeFaces(int xres, int yres)
{
    int x, y;
	int index=0;
	for (y=0;y<yres;y++)
	{
		for (x=0;x<xres;x++)
		{
			faces[index].v1 = (x%xres)+(y%yres)*xres;
			faces[index].v2 = ((x+1)%xres)+(y%yres)*xres;
			faces[index].v3 = ((x+1)%xres)+((y+1)%yres)*xres;
			index++;
			faces[index].v1 = (x%xres)+(y%yres)*xres;
			faces[index].v2 = ((x+1)%xres)+((y+1)%yres)*xres;
			faces[index].v3 = ((x)%xres)+((y+1)%yres)*xres;
			index++;
		}
	}
}

void Mesh::makeUVBox(float scale)
{
    int i;
    calculateNormals();
    for (i=0;i<vertexcount;i++)
    {
        Vector3 v = vertices[i];
        v.normalize();
        uv[i].u = v.x*scale;
        uv[i].v = v.y*scale;
    }
}
void Mesh::makeUVSphere(float scale)
{
    int i;
    calculateNormals();
    for (i=0;i<vertexcount;i++)
    {
        Vector3 n = normals[i];
        n.normalize();
        uv[i].u = n.x*scale;
        uv[i].v = n.y*scale;
    }
}
void Mesh::buildPlane(float size, int xres, int yres, Vector3 &rotation)
{
	int x, y;
	Matrix rotate;

	vertexcount = xres*yres;
	facecount = xres*yres*2;
	vertices = new Vector3[vertexcount];
    normals = new Vector3[vertexcount];
    uv = new TexCoord[vertexcount];
	faces = new Face[facecount];
	rotate.makeRotation(rotation.x, rotation.y, rotation.z);

	for (y=0;y<yres;y++)
	{
		for (x=0;x<xres;x++)
		{
			const float xkohta = x / (float)xres;
			const float ykohta = y / (float)yres;

			vertices[x+y*xres] = (Vector3(xkohta-0.5f, 0, ykohta-0.5f)*size)*rotate;
		}
	}
	int index=0;
	for (y=0;y<yres;y++)
	{
		for (x=0;x<xres;x++)
		{
			faces[index].v1 = (x%xres)+(y%yres)*xres;
			faces[index].v2 = ((x+1)%xres)+(y%yres)*xres;
			faces[index].v3 = ((x+1)%xres)+((y+1)%yres)*xres;
			index++;
			faces[index].v1 = (x%xres)+(y%yres)*xres;
			faces[index].v2 = ((x+1)%xres)+((y+1)%yres)*xres;
			faces[index].v3 = ((x)%xres)+((y+1)%yres)*xres;
			index++;
		}
	}
    calculateNormals();
}
void Mesh::buildTorus(float radius1, float radius2, int xres, int yres)
{
    int x, y;
	vertexcount = xres*yres;
	facecount = xres*yres*2;
	vertices = new Vector3[vertexcount];
    normals = new Vector3[vertexcount];
    uv = new TexCoord[vertexcount];
	faces = new Face[facecount];

    //ulompi kehä
    for (y=0;y<yres;y++)
    {
        float ykohta = y / (float)yres;
        float yangle = ykohta*2*3.141592f;

        //torus tulee xz-tasoon
        Matrix rotation;
        rotation.makeRotation(0, yangle, 0);

        Vector3 centerpoint = Vector3(0, 0, radius1);

        //sisempi kehä
        for (x=0;x<xres;x++)
        {
            float xkohta = x / (float)xres;
            float xangle = xkohta*2*3.141592f;

            Vector3 edgepoint = Vector3(0, (float)sin(xangle), (float)cos(xangle))*radius2;
            vertices[x+y*xres] = (centerpoint + edgepoint) * rotation;
        }
    }
    makeFaces(xres, yres);
    calculateNormals();

}

void Mesh::emptyCopy(Mesh *Mesh)
{
	vertexcount = Mesh->getVertexCount();
	edgecount = Mesh->getEdgeCount();
	facecount = Mesh->getFaceCount();

	vertices = new Vector3[vertexcount];
    normals = new Vector3[vertexcount];
    uv = new TexCoord[vertexcount];
	edges = new Edge[edgecount];
	faces = new Face[facecount];
}
