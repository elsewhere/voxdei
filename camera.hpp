#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "stuff.hpp"
#include "catmullrom.hpp"

class Camera
{
public:
    Vector3 position;
    Vector3 direction;
    Vector3 upwards;

    int st;
    int et;

    class CatmullRom *path;
    class CatmullRom *target;
    class CatmullRom *upvector;

};

class CameraHolder
{
public:
    CameraHolder();
    CameraHolder(int count);
    ~CameraHolder();

    void update();

    void initCamera(int index, int maxpoints, int starttime, int endtime);
    void addCameraPoint(int index, Vector3 &point);
    void addCameraTargetPoint(int index, Vector3 &point);
    void setCameraPoint(int index, Vector3 &point);
    void setCameraTargetPoint(int index, Vector3 &point);
    void useCamera(int index);

    Camera& getCamera(int index);

private:
    int time;
    int camcount;
    Camera *cameras;



};

#endif