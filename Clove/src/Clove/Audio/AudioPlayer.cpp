#include "clvpch.hpp"
#include "AudioPlayer.hpp"

#include <portaudio.h>

namespace clv::sfx{
	struct paTestData{ //TODO: Needed?
		float left_phase;
		float right_phase;
	};

	static int patestCallback(const void* inputBuffer, void* outputBuffer,
							  unsigned long framesPerBuffer,
							  const PaStreamCallbackTimeInfo* timeInfo,
							  PaStreamCallbackFlags statusFlags,
							  void* userData){
		float* out;
		Sound* p_data = (Sound*)userData;
		sf_count_t       num_read;

		out = (float*)outputBuffer;
		p_data = (Sound*)userData;

		/* clear output buffer */
		memset(out, 0, sizeof(float) * framesPerBuffer * p_data->info.channels);

		/* read directly into output buffer */
		num_read = sf_read_float(p_data->file, out, framesPerBuffer * p_data->info.channels);

		/*  If we couldn't read a full frameCount of samples we've reached EOF */
		if(num_read < framesPerBuffer){
			return paComplete;
		}

		return paContinue;
	}

	AudioPlayer::AudioPlayer(){
		//TODO: ERROR CHECK
		auto err = Pa_Initialize();
	}

	AudioPlayer::~AudioPlayer(){
		//TODO: ERROR CHECK
		Pa_Terminate();
	}

	void AudioPlayer::playSound(/*const*/ Sound& sound){
		//TODO: ERROR CHECK
		PaStream* stream;
			PaError err;
			/* Open an audio I/O stream. */
			err = Pa_OpenDefaultStream(&stream,
									   0,          /* no input channels */
									   sound.info.channels,          /* stereo output */
									   paFloat32,  /* 32 bit floating point output */
									   sound.info.samplerate,
									   512,        /* frames per buffer, i.e. the number
														  of sample frames that PortAudio will
														  request from the callback. Many apps
														  may want to use
														  paFramesPerBufferUnspecified, which
														  tells PortAudio to pick the best,
														  possibly changing, buffer size.*/
									   patestCallback, /* this is your callback function */
									   &sound); /*This is a pointer that will be passed to
														  your callback*/

			err = Pa_StartStream(stream);

			Pa_Sleep(5 * 1000);

		//CLV_LOG_ERROR("Port audio error: {0} ", Pa_GetErrorText(err));
		
	}
}