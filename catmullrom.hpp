#ifndef _CATMULLROM_HPP_
#define _CATMULLROM_HPP_

#include "stuff.hpp"

class CatmullRom
{
public:
    CatmullRom();
    CatmullRom(int pointcount);
    ~CatmullRom();

    void addPoint(Vector3 point);
    void setPoint(int index, Vector3 point);
    
    Vector3 getValue(float t);
    Vector3 getTangent(float t);

    Vector3 *points;

private:
    int pointindex;
    int pcount;


};



#endif
