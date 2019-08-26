#include "clvpch.hpp"
#include "SoundPlayer.hpp"

#include <portaudio.h>
#include <sndfile.h>

#define PACall(x) { auto err = x; if(err != paNoError) { CLV_ASSERT(false, /*"Port audio assertion: {0}",*/ Pa_GetErrorText(err)); } }

//TODO:
//Play streams without sleeping?
//keep track of open streams???
//Stop playing sounds?

namespace clv::aud{
	static int soundPlayback(const void* inputBuffer, void* outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData){
		Sound* data = static_cast<Sound*>(userData); /* we passed a data structure
		into the callback so we have something to work with */
		int32* out = static_cast<int32*>(outputBuffer);
		int32* cursor = out; /* current pointer into the output */
		int32 thisSize = frameCount;
		int32 thisRead = 0;
		
		//cursor = out; /* set the output cursor to the beginning */
		while (thisSize > 0){
			/* seek to our current file position */
			sf_seek(data->getFile(), data->position, SEEK_SET);
		
			/* are we going to read past the end of the file?*/
			if (thisSize > (data->getFrames() - data->position)){
				/*if we are, only read to the end of the file*/
				thisRead = data->getFrames() - data->position;
				/* and then loop to the beginning of the file */
				data->position = 0;
			}else{
				/* otherwise, we'll just fill up the rest of the output buffer */
				thisRead = thisSize;
				/* and increment the file position */
				data->position += thisRead;
			}
			/* since our output format and channel interleaving is the same as
			sf_readf_int's requirements */
			/* we'll just read straight into the output buffer */
			sf_readf_int(data->getFile(), cursor, thisRead);
			/* increment the output cursor*/
			cursor += thisRead;
			/* decrement the number of samples left to process */
			thisSize -= thisRead;
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

		Pa_Sleep(5 * 1000);

		//PACall(Pa_CloseStream(stream));

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