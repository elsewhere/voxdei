#include <stdio.h>
#include "stuff.hpp"
#include "timer.hpp"

extern int originalaika;

void Timer::init()
{
	LONGLONG freq;
	LONGLONG ticks;	

	//	High Resolution Timerin taajuutta ei voi muuttaa niin kauan
	//	kuin systeemi on p‰‰ll‰. Taajuus haetaan vain kerran.
		
	HRT = true;
	startTime = 0;
	addTime = 0.0f;
	frames = 0;
	lastTime = 0;	
	fps = 0;
	time = 0;	
			
	if( !QueryPerformanceFrequency( ( LARGE_INTEGER * )&freq ) )
	{				
		// Jos ep‰onnistui k‰ytet‰‰n MultimediaTimeria		
		HRT = false;		
		startTime = (float)timeGetTime();
	}
	else
	{		
		QueryPerformanceCounter( ( LARGE_INTEGER * )&ticks );
		startTime = ( float ) ticks;
		ticksPerSec = 1.0f / ( float )freq;
	}		

}

void Timer::update()
{
	if( HRT )
	{
		LONGLONG ticks;

		// onnistuu
		QueryPerformanceCounter( ( LARGE_INTEGER * )&ticks );
		
		// jaetaan taajuudella jotta saadaan sekunnit
		//time = ((float)(__int64)ticks)/((float)(__int64)ticksPerSec);
		//time = ( ( float )ticks ) / ( ( float )ticksPerSec );
		time = ( ( float ) ticks - startTime ) * ticksPerSec;
	}
	else
	{
		// Jos ei ole HRT-ajastinta, k‰ytet‰‰n Multimedia timeria
		// vaatii WINMM.LIB:n				
		time = ( float )timeGetTime() - startTime;		
	}	
	
	deltaTime = ( time - lastTime );
	lastTime = time;
	
	frames++;		

    time += addTime;
	
	fps = ( float ) frames / time;
}

void Timer::add(float ptime)
{
	addTime += ptime;
}

float Timer::getSeconds()
{
	return time;
}
float Timer::getMilliseconds()
{
	return time * 1000;

}
