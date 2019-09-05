#include "clvpch.hpp"
#include "SoundPlayer.hpp"

#include <portaudio.h>
#include <sndfile.h>

#define PACall(x) { auto err = x; if(err != paNoError) { CLV_ASSERT(false, /*"Port audio assertion: {0}",*/ Pa_GetErrorText(err)); } }

//TODO:
//keep track of open streams???
//Stop playing sounds?

namespace clv::aud{
	SoundPlayer::SoundPlayer(){
		CLV_LOG_TRACE("Sound player intialised");
		PACall(Pa_Initialize());
	}

	SoundPlayer::~SoundPlayer(){
		CLV_LOG_TRACE("Sound player shutdown");
		for (auto& [key, val] : openStreams){
			PACall(Pa_CloseStream(val));
		}
		
		PACall(Pa_Terminate());
	}

	void SoundPlayer::playSound(Sound& sound){
		PaStream* stream;

		theSound = sound; //TODO: we still need to copy here

		PaStreamParameters outputParameters;
		outputParameters.device = Pa_GetDefaultOutputDevice();
		outputParameters.sampleFormat = paInt32;
		outputParameters.channelCount = sound.getChannels();
		outputParameters.suggestedLatency = 0.2f;
		outputParameters.hostApiSpecificStreamInfo = nullptr;

		PACall(Pa_OpenStream(&stream, 0, &outputParameters, sound.getSampleRate(), paFramesPerBufferUnspecified, paNoFlag, &SoundPlayer::soundPlayback_Once, &theSound));
		PACall(Pa_StartStream(stream));

		sound.streamID = generateNextID();

		//TODO: Attach to delegate for when a sound is deconstructed, so it can close
	}

	bool SoundPlayer::isSoundPlaying(const Sound& sound){
		//PACall(const bool isActive = Pa_IsStreamActive(openStreams[sound.streamID.ID]));
		return false; //Temp
	}

	void SoundPlayer::stopSound(const Sound& sound){
		PACall(Pa_CloseStream(openStreams[sound.streamID.ID]));
	}

	AudioStreamID SoundPlayer::generateNextID(){
		static int32 ID = 0; //TODO: Make a proper ID system
		return { ID };
	}

	int SoundPlayer::soundPlayback_Loop(const void* inputBuffer, void* outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData){
		Sound* data = static_cast<Sound*>(userData);
		int32* out = static_cast<int32*>(outputBuffer);
		int32* cursor = out;
		int32 currentFrameCount = frameCount;
		int32 frameCountToRead = 0;
		
		while(currentFrameCount > 0){
			//Seek to the position
			sf_seek(data->getFile(), data->position, SEEK_SET);
		
			//Get the amount of frames to read
			if (currentFrameCount > (data->getFrames() - data->position)){
				frameCountToRead = data->getFrames() - data->position;
				data->position = 0;
			}else{
				frameCountToRead = currentFrameCount;
				data->position += frameCountToRead;
			}
			
			//Read X amount of frames into cursor
			sf_readf_int(data->getFile(), cursor, frameCountToRead);
			
			cursor += frameCountToRead;
			currentFrameCount -= frameCountToRead;
		}
		
		return paContinue;
	}

	int SoundPlayer::soundPlayback_Once(const void* inputBuffer, void* outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData){
		Sound* data = static_cast<Sound*>(userData);
		int32* out = static_cast<int32*>(outputBuffer);
		int32* cursor = out;
		int32 currentFrameCount = frameCount;
		int32 frameCountToRead = 0;
		
		while(currentFrameCount > 0){
			//Seek to the position
			sf_seek(data->getFile(), data->position, SEEK_SET);
		
			//Get the amount of frames to read
			if (currentFrameCount > (data->getFrames() - data->position)){
				return paComplete;
			}else{
				frameCountToRead = currentFrameCount;
				data->position += frameCountToRead;
			}
			
			//Read X amount of frames into cursor
			sf_readf_int(data->getFile(), cursor, frameCountToRead);
			
			cursor += frameCountToRead;
			currentFrameCount -= frameCountToRead;
		}
		
		return paContinue;
	}
}