#include "stuff.hpp"

void antiRotate(Vector3 *x, Vector3 *y, Vector3 *z)
{
	float matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);	
	
	*x = (Vector3(matrix[0], matrix[4], matrix[8]));
	*y = (Vector3(matrix[1], matrix[5], matrix[9]));
	*z = Vector3(matrix[2], matrix[6], matrix[10]);
}

float catmullromValue(float t, float p0, float p1, float p2, float p3)
{
	return 0.5f*((2*p1)+(-p0+p2)*t+(2*p0-5*p1+4*p2-p3)*t*t+(-p0+3*p1-3*p2+p3)*t*t*t);
}

Vector3 lerpVector3(Vector3 &v1, Vector3 &v2, float t)
{
	return v1*(1-t) + v2*t;
}

Vector3 sphereCoord(float radius, float phi, float theta)
{
	float x = radius*(float)(sin(phi)*cos(theta));
	float y = radius*(float)(sin(phi)*sin(theta));
	float z = radius*(float)(cos(phi));

	return Vector3(x, y, z);
}

Vector3 randomSphereVector(float radius)
{
	return sphereCoord(radius, randFloat()*3.141592f, randFloat()*2*3.141592f);
}

Vector3 randomBoxVector(float cubesize)
{
	float x = (randFloat()-0.5f)*cubesize;
	float y = (randFloat()-0.5f)*cubesize;
	float z = (randFloat()-0.5f)*cubesize;

	return Vector3(x, y, z);

}



Vector3 polarCoord(float radius, float phi)
{
	float y = radius*(float)sin(phi);
	float x = radius*(float)cos(phi);

	return Vector3(x, y, 0);

}


//random number generator stolen from
//http://remus.rutgers.edu/~rhoads/Code/code.html

static unsigned int SEED = 93186752;

float randFloat()  
{
/* The following parameters are recommended settings based on research
   uncomment the one you want. */

static unsigned int a = 1588635695, m = 4294967291U, q = 2, r = 1117695901;
/* static unsigned int a = 1223106847, m = 4294967291U, q = 3, r = 625646750;*/
/* static unsigned int a = 279470273, m = 4294967291U, q = 15, r = 102913196;*/
/* static unsigned int a = 1583458089, m = 2147483647, q = 1, r = 564025558; */
/* static unsigned int a = 784588716, m = 2147483647, q = 2, r = 578306215;  */
/* static unsigned int a = 16807, m = 2147483647, q = 127773, r = 2836;      */
/* static unsigned int a = 950706376, m = 2147483647, q = 2, r = 246070895;  */

   SEED = a*(SEED % q) - r*(SEED / q);
   double val = ((double)SEED / (double)m);
   return (float)val;
}


int randInt()
{
	static unsigned int a = 1583458089, m = 2147483647, q = 1, r = 564025558;
	SEED = a*(SEED % q) - r*(SEED / q);
	return SEED;
}

void setSeed(int seed)
{
	SEED = seed;

}

Vector3 randVector3()
{
	return Vector3(randFloat()-0.5f, randFloat()-0.5f, randFloat()-0.5f);

}
