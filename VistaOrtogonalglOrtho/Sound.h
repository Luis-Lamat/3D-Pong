#ifndef __SoundF__Sound__
#define __SoundF__Sound__

#include <iostream>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
using namespace std;

struct RIFF_Header {
	char chunkID[4];
	int chunkSize;//size not including chunkSize or chunkID
	char format[4];
};

struct WAVE_Format {
	char subChunkID[4];
	int subChunkSize;
	short audioFormat;
	short numChannels;
	int sampleRate;
	int byteRate;
	short blockAlign;
	short bitsPerSample;
};

struct WAVE_Data {
	char subChunkID[4]; //should contain the word data
	int subChunk2Size; //Stores the size of the data block
};

class Sound {
public:
	Sound(char*  filePath);
	void PlaySound();
	~Sound();

private:
	char *path;
	FILE *fp = NULL;
	WAVE_Format wave_format;
	RIFF_Header riff_header;
	WAVE_Data wave_data;
	unsigned char* data;
	char type[4];
	int dataSize;
	char *errors[10] = {(char*)"Failed to open file", (char*)"No RIFF", (char*)"not WAVE", (char*)"not fmt ", (char*)"Missing DATA", (char*)"no sound device", (char*)"no sound context", (char*)"Wrong BitPerSample", (char*)"Error loading ALBuffer", (char*)"Error playing sound"};//Create FILE pointer for the WAVE file

	ALuint source;          //Is the name of source (where the sound come from)
	ALuint buffer;          //Stores the sound data
	ALuint frequency;       //The Sample Rate of the WAVE file
	ALenum format=0;
	ALCdevice *device;      //Create an OpenAL Device
	ALCcontext *context;    //And an OpenAL Context
	unsigned char* buf;
};

#endif