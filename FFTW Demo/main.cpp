#include <iostream>
#include <stdio.h>
#include <bitset>

#include <al.h>

#include <alc.h>

#include <fftw3.h>

#include "Sound.h"

void BytesToFloats(unsigned char* in, float* out, int inSize, int outSize, int bytesPerPoint);

int main() {
	ALCdevice* device = alcOpenDevice(NULL);
	ALCcontext* context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);

	alListener3f(AL_POSITION, 0, 0, 0);
	alListener3f(AL_VELOCITY, 0, 0, 0);

	Sound music("you've been hit by.wav");


	const int sampleSize = music.getBitsPerSample() / 8;
	const int sampleNum = 10;
	const int dataSize = sampleSize * sampleNum;

	unsigned char* data = (unsigned char*)malloc(dataSize);

	for (int i = 0; i < dataSize; i++) {
		data[i] = music.getAudioData()[i];
	}

	float *samples = (float*)malloc(sizeof(float) * sampleNum);

	BytesToFloats(data, samples, dataSize, sampleNum, sampleSize);

	for (int i = 0; i < sampleNum; i++) {
		std::cout << samples[i] << std::endl;
	}

	fgetc(stdin);

	free(data);
	free(samples);

	music.dealloc();

	alcDestroyContext(context);
	alcCloseDevice(device);

	return 0;
}


void BytesToFloats(unsigned char* in, float* out, int inSize, int outSize, int bytesPerPoint) {
	for (int i = 0; i < outSize; i++) {
		unsigned char bytes[4] = { 0,0,0,0 };
		for (int j = 0; j < bytesPerPoint; j++) {
			bytes[j] = in[(i*bytesPerPoint) + j];
		}

		if (bytesPerPoint == 8) {
			std::bitset<8> x(bytes[2]);
			std::cout << x << std::endl;
			if (((bytes[3] >> 7) & 1)) bytes[0] = bytes[1] = bytes[2] = 0xff;
		}
		else if (bytesPerPoint == 16) {
			for(int i=7; i>=0)
			if (((bytes[2] >> 7) & 1)) bytes[0] = bytes[1] = 0xff;
		}
		long bin =	((long)bytes[0])		& 0x000000ff |
					((long)bytes[1] << 8)	& 0x0000ff00 |
					((long)bytes[2] << 16)	& 0x00ff0000 |
					((long)bytes[3] << 24)	& 0xff000000;
		out[i] = (float)bin;
	}
}