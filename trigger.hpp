#ifndef _TRIGGER_HPP_
#define _TRIGGER_HPP_

#include "stuff.hpp"
#include "lista.hpp"

class Trigger
{
public:
    Trigger() {};
    Trigger(int starttime, int endtime);
    ~Trigger() {};

    float get(int time);

private:
    int st;
    int et;
};

class TriggerSystem
{
public:
    TriggerSystem();
    ~TriggerSystem();

    void add(int starttime, int endtime);
    bool getChanged();
    float get(int time);

private:
    Lista *triggers;

};



#endif
