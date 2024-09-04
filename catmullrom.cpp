#include "catmullrom.hpp"

CatmullRom::CatmullRom()
{
	points = 0;
	this->pcount = 0;
}

CatmullRom::CatmullRom(int pointcount)
{
	points = 0;
    if (pointcount >= 4)
    {
        this->pcount = pointcount;
        this->pointindex = 0;
        this->points = new Vector3[pointcount];
		//int i; for(i=0; i<pcount; i++) points[i] = Vector4();
    }
    else
    {
        points = 0;
		this->pcount = 0;
    }
}
CatmullRom::~CatmullRom()
{
    if (points != 0 || pcount!=0)
    {
		delete [] this->points;
    }

}

void CatmullRom::setPoint(int index, Vector3 point)
{
	if(index >= pcount || index<0)
	{
	} 
    else
    {
	   points[index] = point;
    }
}

void CatmullRom::addPoint(Vector3 point)
{
	if(pointindex < pcount) 
	{
		points[pointindex] = point;
		pointindex++;
	} 
    else
    {
    }
}

Vector3 CatmullRom::getValue(float pos)
{

    //t = 0 -> index = 1
    //t = 1.0f -> index = count - 2

    if (pos > 1.000f)
    {
        pos = 1.0f;
    }
    if (pos < 0.000f)
    {
        pos = 0.0f;
    }

    float val = (pos * (pcount-3)) + 1;
    const int index = ((int)floorf(pos * (pcount-3))) + 1;

//    dmsMsg("val = %f - index = %d\n", val, index);

    float t = val - index;

    float t2 = t*t;
    float t3 = t2*t;

    Vector3 p0 = points[index - 1];
    Vector3 p1 = points[index];
    Vector3 p2 = points[index + 1];
    Vector3 p3 = points[index + 2];

    Vector3 result = ((p1*2) + (p2 - p0) * t + (p0*2 - p1 * 5 + p2*4 - p3)*t2 + (p1*3 - p0- p2*3 + p3) * t3)*0.5f;
    return result;

}

Vector3 CatmullRom::getTangent(float pos)
{
    
    return Vector3(0, 0, 0);
}
