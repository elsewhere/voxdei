#ifndef _MESHFACTORY_HPP_
#define _MESHFACTORY_HPP_

#include "mesh.hpp"

class MeshFactory
{
public:

    static Mesh* buildSphere(float radius, int res);
	static Mesh* buildPlane(float size, int xres, int yres, Vector &rotation);
    static Mesh* buildTorus(float radius1, float radius2, int xres, int yres);
    static Mesh* buildEmpty(int xres, int yres);


};

#endif