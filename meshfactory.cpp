#include "meshfactory.hpp"

Mesh *MeshFactory::buildTorus(float radius1, float radius2, int xres, int yres)
{
    int x, y;
    Mesh *m = new Mesh();

    m->vertexcount = xres*yres;
	m->facecount = xres*yres*2;
	m->vertices = new Vertex[m->vertexcount];
	m->faces = new Face[m->facecount];

    //ulompi kehä
    for (y=0;y<yres;y++)
    {
        float ykohta = y / (float)yres;
        float yangle = ykohta*2*3.141592f;

        //torus tulee xz-tasoon
        Matrix rotation;
        rotation.makeRotation(0, yangle, 0);

        Vector centerpoint = Vector(0, 0, radius1);

        //sisempi kehä
        for (x=0;x<xres;x++)
        {
            float xkohta = x / (float)xres;
            float xangle = xkohta*2*3.141592f;

            Vector edgepoint = Vector(0, (float)sin(xangle), (float)cos(xangle))*radius2;
            m->vertices[x+y*xres].position = (centerpoint + edgepoint) * rotation;
        }
    }
    m->makeFaces(xres, yres);
    return m;
}
