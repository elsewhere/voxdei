#ifndef _Bezier_HPP_
#define _Bezier_HPP_

#include "stuff.hpp"

class Bezier
{
public:
    Bezier();
    Bezier(int pointcount);
    ~Bezier();

    //point = piste, tangent = suunta (sis‰lt‰‰ jo pisteen paikan)
    void addPoint(Vector3 &point, Vector3 &tangent);
    void setPoint(int index, Vector3 &point, Vector3 &tangent);
    
    Vector3 getValue(float t);

    Vector3 *points;
    Vector3 *tangents;

private:
    int pointindex;
    int pcount;


};



#endif
