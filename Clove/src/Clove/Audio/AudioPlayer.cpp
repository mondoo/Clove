#include "clvpch.hpp"
#include "AudioPlayer.hpp"

#include <portaudio.h>
#include <sndfile.h>

#define PACall(x) { auto err = x; if(err != paNoError) { CLV_ASSERT(false, "PortAudio assertion: {0}", Pa_GetErrorText(err)) } }

namespace clv::sfx{
	static int soundPlayback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData){
		float* out = static_cast<float*>(outputBuffer);
		Sound* data = static_cast<Sound*>(userData);
		sf_count_t num_read;

		memset(out, 0, sizeof(float) * framesPerBuffer * data->getChannels());

		num_read = sf_read_float(data->getFile(), out, framesPerBuffer * data->getChannels());

		if(num_read < framesPerBuffer){
			return paComplete;
		}

		return paContinue;
	}

	AudioPlayer::AudioPlayer(){
		PACall(Pa_Initialize());
	}

	AudioPlayer::~AudioPlayer(){
		PACall(Pa_Terminate());
	}

	void AudioPlayer::playSound(/*const*/ Sound& sound){
		PaStream* stream;

		PACall(Pa_OpenDefaultStream(&stream, 0, sound.getChannels(), paFloat32, sound.getSampleRate(), paFramesPerBufferUnspecified, soundPlayback, &sound));
		PACall(Pa_StartStream(stream));

		Pa_Sleep(5 * 1000);

		PACall(Pa_CloseStream(stream));
	}
}