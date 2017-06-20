#include <iostream>
#include <stdio.h>
#include <bitset>

#include <al.h>

#include <alc.h>

#include <fftw3.h>

#include "Sound.h"

void audiotoFloat(unsigned char* left, unsigned char* right, float* out, int inSize, int outSize, int bytesPerPoint);
void audiotoFloat(unsigned char* in, float* out, int inSize, int outSize, int bytesPerPoint);


int main() {
	ALCdevice* device = alcOpenDevice(NULL);
	ALCcontext* context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);

	alListener3f(AL_POSITION, 0, 0, 0);
	alListener3f(AL_VELOCITY, 0, 0, 0);

	Sound music("you've been hit by.wav");


	//number of samples (or LR frames) to collect
	const int sampleNum = 16;

	//getting size of a sample's channel
	const int sampleSize = music.getBitsPerSample() / 8;

	const int dataSize = (music.getChannels() == 2) ? 2*sampleSize*sampleNum : sampleSize*sampleNum;

	unsigned char* data = (unsigned char*)malloc(dataSize);

	for (int i = 0; i < dataSize; i++) {
		data[i] = music.getAudioData()[i];
	}

	float *samples;
	fftwf_complex *out;
	fftwf_plan p;

	samples = (float*)fftwf_malloc(sizeof(float) * sampleNum);
	out = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * sampleNum);
	p = fftwf_plan_dft_r2c_1d(sampleNum, samples, out, FFTW_ESTIMATE);

	BytesToFloats(data, samples, dataSize, sampleNum, sampleSize);

	fftwf_execute(p);

	/*for (int i = 0; i < sampleNum; i++) {
		std::cout << samples[i] << std::endl;
	}*/

	fgetc(stdin);

	free(data);
	fftwf_destroy_plan(p);
	fftwf_free(samples);
	fftwf_free(out);

	music.dealloc();

	alcDestroyContext(context);
	alcCloseDevice(device);

	return 0;
}


void audiotoFloat(unsigned char* in, float* out, int inSize, int outSize, int bytesPerPoint, int channels) {
	if (channels == 1) {
		audiotoFloat(in, out, inSize, outSize, bytesPerPoint);
		return;
	}

	for (int i = 0; i < outSize; i++) {
		unsigned char leftBytes[2] = {0, 0};
		unsigned char rightBytes[2] = { 0, 0 };

		for (int j = 0; j < bytesPerPoint * 2; j++) {
			leftBytes[j] = left[(i*bytesPerPoint) + j];
			rightBytes[j] = right[(i*bytesPerPoint) + j];
		}

		if (bytesPerPoint == 1) {
			if (((leftBytes[0] >> 7) & 1) == 1) leftBytes[1] = 0xff;
			if (((rightBytes[0] >> 7) & 1) == 1) rightBytes[1] = 0xff;
		}

		short leftBin = ((long)leftBytes[0])		& 0x00ff |
						((long)leftBytes[1] << 8)	& 0xff00;
		short rightBin= ((long)rightBytes[0])		& 0x00ff |
						((long)rightBytes[1] << 8)	& 0xff00;

		out[i] = ((float)leftBin + (float)rightBin) / 2;
	}
}

void audiotoFloat(unsigned char* in, float* out, int inSize, int outSize, int bytesPerPoint) {

	for (int i = 0; i < outSize; i++) {
		unsigned char bytes[2] = { 0, 0 };

		for (int j = 0; j < bytesPerPoint; j++) {
			bytes[j] = in[(i*bytesPerPoint) + j];
		}

		if (bytesPerPoint == 1) {
			if (((bytes[0] >> 7) & 1) == 1) bytes[1] = 0xff;
		}

		short bin = ((long)bytes[0]) & 0x00ff |
					((long)bytes[1] << 8) & 0xff00;

		out[i] = (float)bin;
	}
}