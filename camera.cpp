#include "camera.hpp"

CameraHolder::CameraHolder()
{
    camcount = 0;
    cameras = 0;
}

CameraHolder::~CameraHolder()
{
    if (cameras)
    {
        delete [] cameras;
    }
}

CameraHolder::CameraHolder(int count)
{
    camcount = count;
    cameras = new Camera[camcount];
}

void CameraHolder::update()
{
    time = (int)kello.getMilliseconds();
}

void CameraHolder::useCamera(int index)
{
    float campos = (time - cameras[index].st) / (1.0f*(cameras[index].et - cameras[index].st));
    if (campos < 0.0f)
        campos = 0.0f;
    if (campos > 1.0f)
        campos = 1.0f;

    Vector3 p = cameras[index].path->getValue(campos);
    Vector3 t = cameras[index].target->getValue(campos);
    Vector3 u = Vector3(0,1,0);  //todo

    glLoadIdentity();
    gluLookAt(p.x, p.y, p.z, t.x, t.y, t.z, u.x, u.y, u.z);

}

void CameraHolder::initCamera(int index, int maxpoints, int starttime, int endtime)
{
    cameras[index].path = new CatmullRom(maxpoints); 
    cameras[index].target = new CatmullRom(maxpoints); 
    cameras[index].st = starttime;
    cameras[index].et = endtime;

}
void CameraHolder::addCameraPoint(int index, Vector3 &point)
{
    cameras[index].path->addPoint(point);

}

void CameraHolder::addCameraTargetPoint(int index, Vector3 &point)
{
    cameras[index].target->addPoint(point);

}

void CameraHolder::setCameraPoint(int index, Vector3 &point)
{
    cameras[index].path->setPoint(index, point);

}

void CameraHolder::setCameraTargetPoint(int index, Vector3 &point)
{
    cameras[index].target->setPoint(index, point);

}
