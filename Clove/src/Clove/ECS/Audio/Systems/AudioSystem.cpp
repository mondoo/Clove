#include "clvpch.hpp"
#include "AudioSystem.hpp"

//#include <portaudio.h>

#define PACall(x) { auto err = x; if(err != paNoError) { CLV_ASSERT(false, /*"Port audio assertion: {0}",*/ Pa_GetErrorText(err)); } }

namespace clv::ecs::aud{
	AudioSystem::AudioSystem(){
		CLV_LOG_TRACE("Portaudio intialised");
		PACall(Pa_Initialize()); //TODO: Initialise in ECS system?
	}

	AudioSystem::~AudioSystem(){
		CLV_LOG_TRACE("Portaudio shutdown");
		PACall(Pa_Terminate());
	}

	void AudioSystem::update(utl::DeltaTime deltaTime){
		for(auto& componentTuple : components){
			AudioComponent* component = std::get<AudioComponent*>(componentTuple);
			if(component->currentPlaybackMode){
				startStream(component, component->currentPlaybackMode.value());
				component->currentPlaybackMode.reset();
			}
		}
	}

	void AudioSystem::startStream(AudioComponent* component, PlaybackMode playback){
		if(!component->stream){
			component->playbackPosition = 0;
			//activeStreamData.file = file;

			PaStreamParameters outputParameters;
			outputParameters.device = Pa_GetDefaultOutputDevice();
			outputParameters.sampleFormat = paInt32;
			outputParameters.channelCount = component->sound.getChannels();
			outputParameters.suggestedLatency = 0.2f;
			outputParameters.hostApiSpecificStreamInfo = nullptr;

			//currentPlaybackMode = playback;

			switch(playback){
				case PlaybackMode::once:
					PACall(Pa_OpenStream(&component->stream, 0, &outputParameters, component->sound.getSamplerate(), paFramesPerBufferUnspecified, paNoFlag, &AudioSystem::soundPlayback_Once, component));
					break;
				case PlaybackMode::repeat:
					PACall(Pa_OpenStream(&component->stream, 0, &outputParameters, component->sound.getSamplerate(), paFramesPerBufferUnspecified, paNoFlag, &AudioSystem::soundPlayback_Loop, component));
					break;
				default:
					CLV_ASSERT(false, "{0} : Invalid playback mode!", CLV_FUNCTION_NAME);
					//currentPlaybackMode.reset();
					break;
			}
		}

		PACall(Pa_StartStream(component->stream));
	}

	void AudioSystem::pauseStream(PaStream* stream){
		PACall(Pa_StopStream(stream));
	}

	void AudioSystem::stopStream(PaStream* stream){
		PACall(Pa_CloseStream(stream));
	}
	
	int AudioSystem::soundPlayback_Loop(const void* inputBuffer, void* outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData){
		AudioComponent* data = static_cast<AudioComponent*>(userData);
		int32* out = static_cast<int32*>(outputBuffer);
		int32* cursor = out;
		int32 currentFrameCount = frameCount;
		int32 frameCountToRead = 0;

		while(currentFrameCount > 0){
			//Seek to the position
			data->sound.seek(data->playbackPosition, SEEK_SET);

			//Get the amount of frames to read
			if(currentFrameCount > (data->sound.getFrames() - data->playbackPosition)){
				frameCountToRead = data->sound.getFrames() - data->playbackPosition;
				data->playbackPosition = 0;
			} else{
				frameCountToRead = currentFrameCount;
				data->playbackPosition += frameCountToRead;
			}

			//Read X amount of frames into cursor
			data->sound.readf(cursor, frameCountToRead);

			cursor += frameCountToRead;
			currentFrameCount -= frameCountToRead;
		}

		return paContinue;
	}
	
	int AudioSystem::soundPlayback_Once(const void* inputBuffer, void* outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData){
		AudioComponent* data = static_cast<AudioComponent*>(userData);
		int32* out = static_cast<int32*>(outputBuffer);
		int32* cursor = out;
		int32 currentFrameCount = frameCount;
		int32 frameCountToRead = 0;

		while(currentFrameCount > 0){
			//Seek to the position
			data->sound.seek(data->playbackPosition, SEEK_SET);

			//Get the amount of frames to read
			if(currentFrameCount > (data->sound.getFrames() - data->playbackPosition)){
				return paComplete;
			} else{
				frameCountToRead = currentFrameCount;
				data->playbackPosition += frameCountToRead;
			}

			//Read X amount of frames into cursor
			data->sound.readf(cursor, frameCountToRead);

			cursor += frameCountToRead;
			currentFrameCount -= frameCountToRead;
		}

		return paContinue;
	}
}