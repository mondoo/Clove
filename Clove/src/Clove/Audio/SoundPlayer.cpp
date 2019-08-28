#include "clvpch.hpp"
#include "SoundPlayer.hpp"

#include <portaudio.h>
#include <sndfile.h>

#define PACall(x) { auto err = x; if(err != paNoError) { CLV_ASSERT(false, /*"Port audio assertion: {0}",*/ Pa_GetErrorText(err)); } }

//TODO:
//keep track of open streams???
//Stop playing sounds?

namespace clv::aud{
	//TODO: Rename this one to the loopy boy
	//Nonb looping one will just put the whole data into the output buffer?
	//Not sure if it works like that because the frame count changes
	static int soundPlayback(const void* inputBuffer, void* outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData){
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

	SoundPlayer::SoundPlayer(){
		CLV_LOG_DEBUG("Sound player init");
		PACall(Pa_Initialize());
	}

	SoundPlayer::~SoundPlayer(){
		CLV_LOG_DEBUG("Sound player shutdown");
		for (auto& [key, val] : openStreams){
			PACall(Pa_CloseStream(val));
		}
		
		PACall(Pa_Terminate());
	}

	AudioStreamID SoundPlayer::playSound(/*const*/ Sound& sound){
		theSound = sound;
		
		PaStream* stream;

		PaStreamParameters outputParameters;
		outputParameters.device = Pa_GetDefaultOutputDevice();
		outputParameters.sampleFormat = paInt32;
		outputParameters.channelCount = sound.getChannels();
		outputParameters.suggestedLatency = 0.2f;
		outputParameters.hostApiSpecificStreamInfo = nullptr;

		PACall(Pa_OpenStream(&stream, 0, &outputParameters, sound.getSampleRate(), paFramesPerBufferUnspecified, paNoFlag, soundPlayback, &theSound));
		PACall(Pa_StartStream(stream));

		auto ID = generateNextID();
		openStreams[ID.ID] = stream;

		return ID;
	}

	void SoundPlayer::stopSound(AudioStreamID ID){
		//Pa_StopStream .. ??
		PACall(Pa_CloseStream(openStreams[ID.ID])); //Do I need to do this when it finishes?
	}

	AudioStreamID SoundPlayer::generateNextID(){
		static int32 ID = 0; //TODO: Make a proper ID system
		return { ID };
	}
}