#include "Bezier.hpp"

Bezier::Bezier()
{
	points = 0;
	tangents = 0;
}

Bezier::Bezier(int pointcount)
{
	points = 0;
	tangents = 0;
    if (pointcount >= 4)
    {
        this->pcount = pointcount;
        this->pointindex = 0;
        points = new Vector3[pointcount];
        tangents= new Vector3[pointcount];
    }
    else
    {
    }
}
Bezier::~Bezier()
{
    if (points != 0)
    {
        delete [] points;
    }
    if (tangents != 0)
    {
        delete [] tangents;
    }

}

void Bezier::setPoint(int index, Vector3 &point, Vector3 &tangent)
{
	if(index >= pcount || index<0)
	{
	} 
    else 
    {
		points[index] = point;
		tangents[index] = point + tangent;
	}
}

void Bezier::addPoint(Vector3 &point, Vector3 &tangent)
{
	if(pointindex>=0 && pointindex < pcount) 
	{
		points[pointindex] = point;
		tangents[pointindex] = point + tangent;
		pointindex++;
	} 
    else
    {
    }
}

Vector3 Bezier::getValue(float pos)
{

//    float val = (pos * (pcount-3)) + 1;
//    const int index = ((int)floorf(pos * (pcount-3))) + 1;

    if (pos > 1.0000f)
    {
        return Vector3(0, 0, 0);
    }
    float val = pos * (pcount-1);
    const int index = (int)floorf(pos * (pcount-1));
    
    float t = val - index;

    float t2 = t*t;
    float t3 = t2*t;

    Vector3 p0 = points[index];
    Vector3 p1 = tangents[index];
    Vector3 p2 = tangents[index + 1];
    Vector3 p3 = points[index + 1];

    Vector3 result = p0*(1-t)*(1-t)*(1-t) + p1*3*t*(1-t)*(1-t) + p2*3*t2*(1-t)+p3*t3; 
    return result;

}
