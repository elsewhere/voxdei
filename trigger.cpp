#include "trigger.hpp"

Trigger::Trigger(int starttime, int endtime)
{
    st = starttime;
    et = endtime;
    

}

float Trigger::get(int time)
{
    if (time < st)
        return 0.0f;
    if (time >= et)
        return 0.0f;

    //välissä 
    return 1 - (float)(time - st) / (float)(et - st);
}
/*

class TriggerSystem
{
public:
    TriggerSystem();
    ~TriggerSystem();

    void add(int starttime, int endtime);
    float get(int time);

private:
    Lista2 *triggers;

};
*/

TriggerSystem::TriggerSystem()
{
    triggers = new Lista();
}
TriggerSystem::~TriggerSystem()
{
    delete triggers;
}

void TriggerSystem::add(int starttime, int endtime)
{
    Trigger *t = new Trigger(starttime, endtime);
    triggers->add(t);
}

float TriggerSystem::get(int time)
{
    int i;
    float value = 0;
    int count = triggers->getNodeCount();
    triggers->goToStart();
    
    for (i=0;i<count;i++)
    {
        Trigger *t = (Trigger *)triggers->getCurrent();
        value += t->get(time);
        triggers->goForward();
    }

    if (value > 1.0f)
        value = 1.0f;

    return value;
}
