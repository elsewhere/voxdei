//--------------------------------------------------------------------------------------------
//  
//	Format description:
//
//	4 bytes : Vertex count
//  4 bytes : Face count
//  
//  Vertex count * 32 bytes : Vertex data (float pos[3], float norm[3], float u, float v)
//  Face count * 24 bytes : Face data (float norm[3], int a, int b, int c)
//
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "t3d.hpp"

//--------------------------------------------------------------------------------------------
//  Code
//--------------------------------------------------------------------------------------------

T3D::T3D()
{
	vertex = NULL;
	face = NULL;
}

T3D::~T3D()
{
	if(face)
	{
		delete [] face;
		face = NULL;
	}

	if(vertex)
	{
		delete [] vertex;
		vertex = NULL;
	}

}

bool T3D::initVertices(unsigned int count)
{
	vertex = new T3DVertex[count];
	if(!vertex)
	{
		return false;
	}

	return true;
}

bool T3D::initFaces(unsigned int count)
{
	face = new T3DFace[count];
	if(!face)
	{
		return false;
	}

	return true;
}

