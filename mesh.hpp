#ifndef _MESH_HPP_
#define _MESH_HPP_

//#include "Vector3.hpp"
#include "stuff.hpp"

class Face
{
public:
	int v1, v2, v3; //indeksit verteksitaulukkoon
	Vector3 normal;
};

class Vertex
{
public:

	Vector3 position;
    Vector3 position2;
	Vector3 normal;
	Vector4 color;
	float u, v;
};

class TexCoord
{
public:
    float u;
    float v;
};

class Edge
{
public:
	int v1;
	int v2;
};

class Mesh
{
public:
    Mesh();
    ~Mesh();

    //aksessorit
	Vector3 *getVertices() { return vertices; }
	Vector3 *getNormals() { return normals; }
	Face *getFaces() { return faces; }
	Edge *getEdges() { return edges; }
	TexCoord *getUV() { return uv; }

    int getVertexCount() { return vertexcount; }
	int getFaceCount() { return facecount; }
	int getEdgeCount() { return edgecount; }
        
    //piirto
    //tarttee keksi‰ n‰ihin joku fiksumpi
    void drawTextured();
	void drawTexturedWithLight(Vector3 &light, Vector3 &color, float ambient);
    void drawSolidColor();
    void drawSolidColorWithLight(Vector3 &light, Vector3 &color, float ambient);

    //inittijutut
    void calculateNormals(); //rikkoo pallon normaalit - investigate
    void makeFaces(int xres, int yres); //linkitt‰‰ tason faceihin
    void scale(float max); //skaalaa mallin niin, ett‰ kauimpana keskustasta olevan verteksin et‰isyys on max

    //TODO: kunnolliset mappaukset :)
    void makeUVBox(float scale);  //u = x, v = y
    void makeUVSphere(float scale); //envymap

    //rakentaa mallin
    void buildSphere(float radius, int xres, int yres, float texturescale=1.0f);
	void buildPlane(float size, int xres, int yres, Vector3 &rotation);
    void buildTorus(float radius1, float radius2, int xres, int yres);
    void buildEmpty(int xres, int yres);
    void buildAsteroid(int seed, int xres, int yres, float size, float texturescale);
	void emptyCopy(Mesh *object);

	void destroy();


    int vertexcount;
    int facecount;
    int edgecount;

    Vector3 *vertices;
    Vector3 *normals;
    Vector3 *colors;
    TexCoord *uv;
    
    Face *faces;
	Edge *edges;



};


#endif
