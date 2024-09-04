#ifndef _SOUND_HPP_
#define _SOUND_HPP_

//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

//#include "fmod.h"
//#include "fmod_errors.h"
#include "fmod/fmod.hpp"
#include "fmod/fmod_errors.h"

//--------------------------------------------------------------------------------------------
//  This code belongs to our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//  Callback prototypes
//--------------------------------------------------------------------------------------------

//	signed char endcallback(FSOUND_STREAM *stream, void *buff, int len, int param);

//--------------------------------------------------------------------------------------------
//
//  Singleton Sound class
//
//  Initializes FMOD 
//
//--------------------------------------------------------------------------------------------

	class Sound
	{
		friend class Song;

		public:

			~Sound();

			static Sound *create();
			static bool checkEnabled();
			bool init();		
	
			void setVolume( Song *music, float i );
			void setVolumeNow( Song *music, float i);
			void close();
			void update();

			
			// Palauttaa taulukon PCM:ta (pulse-code modulation) 
			void getWaveData( Song *music, float *array, int size );
			// Palauttaa taulukon jossa ‰‰nen spektri
			void getSpectrum( Song *music, float *array, int size );
			static bool checkSync();
			static void setEnabled(bool f);
			static void setSync(bool f);

		private:

			static bool createdFlag;
			static bool enabled;
			static Sound *instance;
			static bool syncFlag;
			static FMOD::System *fmodSystem;
			
			Sound() {}
			Sound(const Sound &) {}
			Sound& operator = (const Sound &) {}
	};

//--------------------------------------------------------------------------------------------
//  Sample class (WAV)
//--------------------------------------------------------------------------------------------
/*
	class Sample
	{
		public:

			Sample();
			~Sample();

			bool load(char *name);
			void play();
			void release();

		private:

			FSOUND_SAMPLE *sample;
	};
*/
//--------------------------------------------------------------------------------------------
//
//  Song class
//
//  Handles stream and module supperted in FMOD
//
//--------------------------------------------------------------------------------------------

	class Song
	{
		friend class Sound;

		public:

			Song();
			~Song();
			
			bool loadStream(char *name);
			bool loadStream2(char *memdata, int length);
			
			void start();
			void stop();
			void release();		
			
			void setPosition(unsigned int offset);
			void setPause(bool f);

			int getLength();
			int getPosition();
			bool checkPlaying();

		private:

			bool isPlaying;

			FMOD::Sound *stream;

			float volume;

			char *data;
			
			unsigned int length;
			FMOD::Channel *channel;
	};


#endif