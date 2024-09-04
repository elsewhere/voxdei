#ifndef _PRIMITIVES_HPP_
#define _PRIMITIVES_HPP_

#include "stuff.hpp"

void tetrahedron();
void octahedron();
void hexahedron();
void icosahedron();
void dodecahedron();

void wireCube(float size);
void flatTausta(Vector3 &color, float alpha);
void circle(Vector3 &kohta, float radius);
void billboard(Vector3 &kohta, Vector3 &x, Vector3 &y, Vector3 &z, float scale);
void billboard2(Vector3 &kohta, Vector3 &x, Vector3 &y, Vector3 &z, float scale);

#endif