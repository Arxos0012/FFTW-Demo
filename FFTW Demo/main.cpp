#include <iostream>
#include <stdio.h>
#include <bitset>

#include <al.h>
#include <alc.h>

#include <fftw3.h>

#include "Sound.h"

float *BytesToFloats(unsigned char* in, float* out, int inSize, int outSize, int bytesPerPoint);

int main() {
	ALCdevice* device = alcOpenDevice(NULL);
	ALCcontext* context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);

	alListener3f(AL_POSITION, 0, 0, 0);
	alListener3f(AL_VELOCITY, 0, 0, 0);

	Sound music("you've been hit by.wav");

	
	const int sampleSize = music.getBitsPerSample() / 8;
	const int sampleNum = 1;
	const int dataSize = sampleSize * sampleNum;

	unsigned char* data = (unsigned char*)malloc(dataSize);

	for (int i = 0; i < dataSize; i++) {
		data[i] = music.getAudioData()[i];
	}

	float *samples = (float*)malloc(sizeof(float) * sampleNum);

	fgetc(stdin);

	free(data);
	free(samples);

	music.dealloc();

	alcDestroyContext(context);
	alcCloseDevice(device);

	return 0;
}


float *BytesToFloats(unsigned char* in, float* out, int inSize, int outSize, int bytesPerPoint) {
	return 0;
}