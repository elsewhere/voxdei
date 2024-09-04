#ifndef _puu_HPP_
#define _puu_HPP_

#include "../stuff.hpp"
#include <vector>

using namespace std;

class TreeVertex
{
public:
    TreeVertex(Vector3 &p):pos(p){}
    Vector3 pos;

};

class TreeFace
{
public:

    
    TreeFace(TreeVertex *v1, TreeVertex *v2, TreeVertex *v3, TreeVertex *v4, Vector3 &color);
    
    TreeVertex *a;
    TreeVertex *b;
    TreeVertex *c;
    TreeVertex *d;

    Vector3 color; 
    Vector3 normal;
    void draw(float alpha);
    void drawMove(float alpha, Vector3 &delta);
    bool wireframe;
};

class TreeSegment
{
public:

    float st;
    float et;

    bool connected;
    void draw(float time);
    TreeVertex *v1;
    TreeVertex *v2;
    TreeVertex *v3;
    TreeVertex *v4;
    TreeVertex *v5;
    TreeVertex *v6;
    TreeVertex *v7;
    TreeVertex *v8;

    TreeFace *f1;
    TreeFace *f2;
    TreeFace *f3;
    TreeFace *f4;
    TreeFace *f5;
    TreeFace *f6;

    Vector3 movedirection;
    float massa;

    //liikkumisjutut
    void drawMove(float time, Vector3 &delta);
    float startenergy;
    float energy;
    float energyscale;
    


};

class Tree
{
public:
    bool wireframe;
    vector<TreeSegment*> treesegments;



};

void createSegmentsAroundSpline(Tree *tree, CatmullRom *spline, int count, bool wireframe, bool connected, bool mustavalko, 
                                float radiusmod);


void puu(float aika);
void puu_init();
void puu_free();

#endif