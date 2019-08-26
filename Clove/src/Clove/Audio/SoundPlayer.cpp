#include "clvpch.hpp"
#include "SoundPlayer.hpp"

#include <portaudio.h>
#include <sndfile.h>

#define PACall(x) { auto err = x; if(err != paNoError) { CLV_ASSERT(false, "Port audio assertion: {0}", Pa_GetErrorText(err)); } }

//TODO:
//Play streams without sleeping?
//keep track of open streams???
//Stop playing sounds?

namespace clv::aud{
	static int soundPlayback(const void* inputBuffer, void* outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData){
		//float* out = static_cast<float*>(outputBuffer);
		//Sound* data = static_cast<Sound*>(userData);
		//sf_count_t num_read;
//
		//memset(out, 0, sizeof(float) * framesPerBuffer * data->getChannels());
//
		//num_read = sf_read_float(data->getFile(), out, framesPerBuffer * data->getChannels());
//
		//CLV_LOG_DEBUG("(num_Read){0} : (framesPerBuffer){1}", num_read, framesPerBuffer);
		////??????
		//if(num_read < framesPerBuffer){
		//	return paComplete;
		//}
//
		//return paContinue;

		CLV_LOG_DEBUG("Called");

		Sound* data = static_cast<Sound*>(userData); /* we passed a data structure
		into the callback so we have something to work with */
		int *cursor; /* current pointer into the output */
		int *out = (int *)outputBuffer;
		int thisSize = frameCount;
		int thisRead;

		cursor = out; /* set the output cursor to the beginning */
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

		PACall(Pa_OpenDefaultStream(&stream, 0, sound.getChannels(), paFloat32, sound.getSampleRate(), paFramesPerBufferUnspecified, soundPlayback, &theSound));
		PACall(Pa_StartStream(stream));

		openStreams[generateNextID().ID] = stream;

		//Pa_Sleep(5 * 1000);

		//PACall(Pa_CloseStream(stream));
	}

	void SoundPlayer::stopSound(AudioStreamID ID){
		//Pa_StopStream .. ??
		PACall(Pa_CloseStream(openStreams[ID.ID])); //Do I need to do this when it finishes?
	}

	AudioStreamID SoundPlayer::generateNextID(){
		static int32 ID = 0; //TODO: Make a proper ID system

	}
}