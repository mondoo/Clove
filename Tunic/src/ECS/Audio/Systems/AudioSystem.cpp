#include "Tunic/ECS/Audio/Systems/AudioSystem.hpp"

#include "Tunic/ECS/Audio/Components/AudioComponent.hpp"
#include "Tunic/ECS/Core/World.hpp"

//TODO: Clove wrapper for audio
#include <portaudio.h>

#define PACall(x)                                                                        \
	{                                                                                      \
		auto err = x;                                                                        \
		CLV_ASSERT(err == paNoError, /*"Port audio assertion: {0}",*/ Pa_GetErrorText(err)); \
	}

using namespace clv;

namespace tnc::ecs::aud {
	static bool isStreamActive(PaStream* stream) {
		if(stream == nullptr) {
			return false;
		}

		PaError error = Pa_IsStreamActive(stream);
		CLV_ASSERT(error >= 0, /*"Port audio assertion: {0}",*/ Pa_GetErrorText(error));
		return error > 0;
	}

	AudioSystem::AudioSystem() {
		CLV_LOG_TRACE("Portaudio intialised");
		PACall(Pa_Initialize());
	}

	AudioSystem::AudioSystem(AudioSystem&& other) noexcept = default;

	AudioSystem& AudioSystem::operator=(AudioSystem&& other) noexcept = default;

	AudioSystem::~AudioSystem() {
		CLV_LOG_TRACE("Portaudio shutdown");
		PACall(Pa_Terminate());
	}

	void AudioSystem::update(World& world, utl::DeltaTime deltaTime) {
		CLV_PROFILE_FUNCTION();

		for(auto [component] : world.getComponentSets<AudioComponent>()) {
			if(component->requestedPlayback) {
				startSound(component.get(), component->requestedPlayback.value());
				component->requestedPlayback.reset();
			} else if(component->requestedStopMode) {
				switch(component->requestedStopMode.value()) {
					case StopMode::pause:
						pauseSound(component.get());
						break;

					case StopMode::stop:
						stopSound(component.get());
						break;

					default:
						CLV_ASSERT(false, "{0}: Unhandled state", CLV_FUNCTION_NAME);
						break;
				}
				component->requestedStopMode.reset();
			}
		}
	}

	void AudioSystem::onComponentDestroyed(ComponentInterface* component) {
		if(component->getID() == AudioComponent::id()) {
			auto* audioComponent = static_cast<AudioComponent*>(component);
			if(audioComponent->isPlaying()) {
				stopSound(audioComponent);
			}
		}
	}

	void AudioSystem::startSound(AudioComponent* component, PlaybackMode playback) {
		if(component->currentPlayback.has_value() && component->currentPlayback != playback) {
			stopSound(component);
		} else if(isStreamActive(component->stream)) {
			return;
		}

		if(!component->stream) {
			component->playbackPosition = 0;

			PaStreamParameters outputParameters;
			outputParameters.device = Pa_GetDefaultOutputDevice();
			outputParameters.sampleFormat = paInt32;
			outputParameters.channelCount = component->sound.getChannels();
			outputParameters.suggestedLatency = 0.2f;
			outputParameters.hostApiSpecificStreamInfo = nullptr;

			switch(playback) {
				case PlaybackMode::once:
					PACall(Pa_OpenStream(&component->stream, 0, &outputParameters, component->sound.getSamplerate(), paFramesPerBufferUnspecified, paNoFlag, &AudioSystem::soundPlayback_Once, component));
					break;
				case PlaybackMode::repeat:
					PACall(Pa_OpenStream(&component->stream, 0, &outputParameters, component->sound.getSamplerate(), paFramesPerBufferUnspecified, paNoFlag, &AudioSystem::soundPlayback_Loop, component));
					break;
				default:
					CLV_ASSERT(false, "{0} : Invalid playback mode!", CLV_FUNCTION_NAME);
					break;
			}

			component->currentPlayback = playback;
		}

		PACall(Pa_StartStream(component->stream));
		component->playing = true;
	}

	void AudioSystem::pauseSound(AudioComponent* component) {
		if(isStreamActive(component->stream)) {
			PACall(Pa_StopStream(component->stream));
			component->playing = false;
		}
	}

	void AudioSystem::stopSound(AudioComponent* component) {
		if(isStreamActive(component->stream)) {
			PACall(Pa_CloseStream(component->stream));
			component->stream = nullptr;
			component->playing = false;
			component->currentPlayback.reset();
		}
	}

	int AudioSystem::soundPlayback_Loop(const void* inputBuffer, void* outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData) {
		AudioComponent* data = static_cast<AudioComponent*>(userData);
		int32_t* out = static_cast<int32_t*>(outputBuffer);
		int32_t* cursor = out;
		int32_t currentFrameCount = frameCount;
		int32_t frameCountToRead = 0;

		while(currentFrameCount > 0) {
			//Seek to the position
			data->sound.seek(data->playbackPosition, SEEK_SET);

			//Get the amount of frames to read
			if(currentFrameCount > (data->sound.getFrames() - data->playbackPosition)) {
				frameCountToRead = data->sound.getFrames() - data->playbackPosition;
				data->playbackPosition = 0;
			} else {
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

	int AudioSystem::soundPlayback_Once(const void* inputBuffer, void* outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData) {
		AudioComponent* data = static_cast<AudioComponent*>(userData);
		int32_t* out = static_cast<int32_t*>(outputBuffer);
		int32_t* cursor = out;
		int32_t currentFrameCount = frameCount;
		int32_t frameCountToRead = 0;

		while(currentFrameCount > 0) {
			//Seek to the position
			data->sound.seek(data->playbackPosition, SEEK_SET);

			//Get the amount of frames to read
			if(currentFrameCount > (data->sound.getFrames() - data->playbackPosition)) {
				return paComplete;
			} else {
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