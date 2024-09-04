#ifndef _BPM_HPP_
#define _BPM_HPP_

#include "stuff.hpp"

extern class BPM *beat;

class BPM
{
public:
	BPM() {};
	~BPM();
	BPM(float value, float offset);

	float getValue();
	int getBeatcount();
	bool getChanged();


private:
	float value;
	float offset;
	int framecount;
	int previousbeatdist;
	bool changed;


};


#endif
