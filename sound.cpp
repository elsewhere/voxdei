//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

#include <windows.h>
#include <stdio.h>

#ifdef _DEBUG
	#include <stdlib.h>
	#include "mmgr.h"
#endif

#include "sound.hpp"

//--------------------------------------------------------------------------------------------
//  Use our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//  Static members
//--------------------------------------------------------------------------------------------

bool Sound::createdFlag = false;
bool Sound::enabled = false;
bool Sound::syncFlag = false;
Sound *Sound::instance = NULL;
FMOD::System *Sound::fmodSystem = NULL;

//--------------------------------------------------------------------------------------------
//  Sound class code
//--------------------------------------------------------------------------------------------

Sound::~Sound()
{
	
}

Sound *Sound::create()
{
	if(!createdFlag)
	{
		instance = new Sound;
		if(!instance)
		{						
			throw "Sound::create(): Memory allocation error";
			return NULL;
		}

		createdFlag = true;

	}
	else
	{				
		throw "One instance of class Sound allready running";
		return NULL;
	}

	return instance;
}

bool Sound::init()
{		
	FMOD_RESULT result;
	unsigned int version;
	
	result = FMOD::System_Create( &fmodSystem );
	if( result != FMOD_OK )
    {
//        dmsMsg( "Sound::init() error: FMOD error! (%d) %s\n", result, FMOD_ErrorString( result ) );
        return false;
    }

	result = fmodSystem->getVersion( &version );
	if( result != FMOD_OK )
    {
//        dmsMsg( "Sound::init() error: FMOD error! (%d) %s\n", result, FMOD_ErrorString( result ) );
        return false;
    }

	// Check for correct version
	if( version < FMOD_VERSION )
	{
//		dmsMsg( "Sound::init() error: invalid FMOD version!" );
		return false;
	}

	// Init FMOD
	result = fmodSystem->init( 100, FMOD_INIT_NORMAL, 0 );
    if( result != FMOD_OK )
    {
//        dmsMsg( "Sound::init() error: FMOD error! (%d) %s\n", result, FMOD_ErrorString( result ) );
        return false;
    }

	// Aktivoidaan FFT-unit jotta voidaan käyttää GetSpectrumia
//	FSOUND_DSP_SetActive(FSOUND_DSP_GetFFTUnit(), true);

	return true;
}

void Sound::setSync( bool f )
{
	syncFlag = f;	
}

void Sound::setEnabled( bool f )
{
	enabled = f;
}

bool Sound::checkSync()
{		
	return syncFlag;
}

bool Sound::checkEnabled()
{
	return enabled;
}

void Sound::setVolume( Song *music, float i )
{
	if( music )
	{
		music->volume=i;
		
		if( music->channel )
			music->channel->setVolume(music->volume);
		
	}
}

void Sound::getWaveData( Song *music, float *array, int size )
{
	if( music ) 
	{
		music->channel->getWaveData( array, size, 0 );
	}
}

void Sound::getSpectrum( Song *music, float *array, int size )
{
	if( music )
	{
			music->channel->getSpectrum( array, size, 0,  FMOD_DSP_FFT_WINDOW_RECT);
	}
}

void Sound::update()
{
	if( instance && fmodSystem)
	{
		fmodSystem->update();
	}
}

void Sound::close()
{
	if( fmodSystem )
	{
		fmodSystem->close();
		fmodSystem->release();
	}

	if(createdFlag)
	{		
		createdFlag = false;
	
		if(instance)
		{
			delete instance;
			instance = NULL;
		}
		
//		dmsMsg("Sound::close(): Instance deleted\n");
	}
}

//--------------------------------------------------------------------------------------------
//  Sample class code
//--------------------------------------------------------------------------------------------
/*
Sample::Sample()
{
	sample = NULL;
}

Sample::~Sample()
{
	release();
}

bool Sample::load(char *name)
{	
	sample = FSOUND_Sample_Load(FSOUND_UNMANAGED, name, FSOUND_NORMAL, 0);
	if(!sample)
	{		
		return false;
	}
	else
	{		
	}

	return true;
}

void Sample::play()
{
	FSOUND_PlaySound(FSOUND_FREE, sample);
}

void Sample::release()
{
	if(sample != NULL)
	{
		FSOUND_Sample_Free(sample);
		sample = NULL;
	}
}
*/
//--------------------------------------------------------------------------------------------
//  Song class code
//--------------------------------------------------------------------------------------------

Song::Song()
{
	stream = NULL;
	data = NULL;
	channel = NULL;
	isPlaying = false;
	volume=1.0f;
}

Song::~Song()
{

}

bool Song::loadStream( char *name )
{
	FMOD_RESULT result;
	FILE *fp;
	
	data = NULL;
	fp = fopen(name, "rb");

	if (!fp)
	{			
//		dmsMsg( "Song::loadStream() error: File load \"%s\" failed\n" );
		return false;
	}

	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	data = new char[length];
	if(!data) return false;

	fread(data, length, 1, fp);
	fclose(fp);

	result = Sound::fmodSystem->createSound( name, FMOD_DEFAULT, false, &stream );
    if( result != FMOD_OK )
    {
//        dmsMsg( "Sound::init() error: FMOD error! (%d) %s\n", result, FMOD_ErrorString( result ) );
        return false;
    }
		
	stream->getLength( &length, FMOD_TIMEUNIT_MS );

	return true;
}

bool Song::loadStream2(char *memdata, int length)
{
	FMOD_RESULT result;
	FMOD_CREATESOUNDEXINFO exinfo;

	data = new char[length];
	if( !data )
	{
//		dmsMsg( "Sound::loadStream2() error: Memory allocation error!\n" );
		return false;
	}

	memset( &exinfo, 0, sizeof( FMOD_CREATESOUNDEXINFO ) );
    exinfo.cbsize = sizeof( FMOD_CREATESOUNDEXINFO );
    exinfo.length = length;

	memcpy( data, memdata, length );

	result =  Sound::fmodSystem->createSound( data, FMOD_SOFTWARE | FMOD_OPENMEMORY | FMOD_2D, &exinfo, &stream );
    if( result != FMOD_OK )
    {
//        dmsMsg( "Sound::init() error: FMOD error! (%d) %s\n", result, FMOD_ErrorString( result ) );
        return false;
    }

	stream->getLength( &this->length, FMOD_TIMEUNIT_MS );
		
	return true;
}

void Song::start()
{
	Sound::fmodSystem->playSound( FMOD_CHANNEL_FREE, stream, false, &channel );
	if( channel == NULL )
	{
//		dmsMsg( "Song::start() warning: channel is NULL!\n" );
		return;
	}

	channel->setVolume(this->volume);
	
	setPause( false );
	isPlaying = true;
}

void Song::stop()
{
	setPause( true );
}

bool Song::checkPlaying()
{
	return isPlaying;
}

void Song::release()
{
	if(stream)
	{
		stream->release();
		stream = NULL;
	}

	if(data)
	{
		delete [] data;
		data = NULL;
	}

	stream = NULL;
	data = NULL;
}

void Song::setPosition(unsigned int offset)
{
	// Tässä offsetti on millisekunteina
	if( channel )
	{
		channel->setPosition( offset, FMOD_TIMEUNIT_MS );
	}
}

void Song::setPause(bool f)
{
	if( channel )
	{
		(f) ? channel->setPaused( true ) : channel->setPaused( false );
	}
}

int Song::getLength()
{
	return length;
}

int Song::getPosition()
{
	unsigned int time = 0;

	if( channel )
	{					
		channel->getPosition( &time, FMOD_TIMEUNIT_MS );
	}

	return time;
}
