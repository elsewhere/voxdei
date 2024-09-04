#ifndef _tuho_HPP_
#define _tuho_HPP_

#include "../stuff.hpp"
#include "puu.hpp"

class Enkeli
{
public:
    Enkeli();
    int type;

    float a;
    float b;
    float r;

    float aspeed;
    float bspeed;


};

void tuho(float aika);
void tuho_init();
void tuho_free();

#endif