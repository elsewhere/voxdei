#ifndef _MATIKKA_H_
#define _MATIKKA_H_

#include "Vector.hpp"

//satunnaisluvut.. 
float randFloat();
int randInt();
void setSeed(int seed);


//kaivetaan projektiomatriisista tavarat, että voidaan piirtää billboardit
void antiRotate(Vector3 *x, Vector3 *y, Vector3 *z);

float catmullromValue(float t, float p1, float p2, float p3, float p4);

//vektorin lineaarinen interpolaatio, t=0 palauttaa v1, t=1 palauttaa v2
Vector3 lerpVector3(Vector3 &v1, Vector3 &v2, float t);

//pallokoordinaatistomuunnos
//radius = [0..ääretön], phi = [0..pii], theta = [0..2*pii]
Vector3 sphereCoord(float radius, float phi, float theta);

//satunnainen vektori pallokoordinaatistossa
Vector3 randomSphereVector(float radius);

//satunnainen vektori kuution sisällä
Vector3 randomBoxVector(float cubesize);

//napakoordinaattimuunnis
Vector3 polarCoord(float radius, float phi);

Vector3 randVector3();

#endif