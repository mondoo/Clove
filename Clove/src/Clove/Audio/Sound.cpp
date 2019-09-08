#include "clvpch.hpp"
#include "Sound.hpp"

#define PACall(x) { auto err = x; if(err != paNoError) { CLV_ASSERT(false, /*"Port audio assertion: {0}",*/ Pa_GetErrorText(err)); } }

namespace clv::aud{
	Sound::Sound(){
		CLV_LOG_TRACE("Sound player intialised");
		PACall(Pa_Initialize());
	}

	Sound::Sound(const Sound& other){
		file = other.file;
	}

	Sound& Sound::operator=(const Sound& other){
		file = other.file;
		return *this;
	}

	Sound::Sound(Sound&& other) = default;

	Sound& Sound::operator=(Sound&& other) = default;

	Sound::~Sound(){
		if(isPlaying()){

		}
		CLV_LOG_TRACE("Sound player shutdown");
		PACall(Pa_Terminate());
	}

	Sound::Sound(const std::string& filePath)
		: file(filePath.c_str(), SFM_READ){
		CLV_LOG_TRACE("Sound player intialised");
		PACall(Pa_Initialize());
	}

	void Sound::play(PlaybackMode playback){
		activeStreamData.position = 0;
		activeStreamData.file = file;

		PaStreamParameters outputParameters;
		outputParameters.device = Pa_GetDefaultOutputDevice();
		outputParameters.sampleFormat = paInt32;
		outputParameters.channelCount = file.channels();
		outputParameters.suggestedLatency = 0.2f;
		outputParameters.hostApiSpecificStreamInfo = nullptr;

		//Want to pass the SndFileHandle here so we can copy sound files, but how to keep track of position?
		//Or the sound should be equipped with being able to be copied in here and kept alive?
		PACall(Pa_OpenStream(&openStream, 0, &outputParameters, file.samplerate(), paFramesPerBufferUnspecified, paNoFlag, &Sound::soundPlayback_Once, &activeStreamData));
		PACall(Pa_StartStream(openStream));

		//Pa_is
	}

	void Sound::pause(){
		//PACall(Pa_StopStream(openStreams[sound.streamID.ID]));
	}

	void Sound::stop(){
		//PACall(Pa_CloseStream(openStreams[sound.streamID.ID]));
	}

	bool Sound::isPlaying(){
		return false;
	}

	int Sound::soundPlayback_Loop(const void* inputBuffer, void* outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData){
		StreamData* data = static_cast<StreamData*>(userData);
		int32* out = static_cast<int32*>(outputBuffer);
		int32* cursor = out;
		int32 currentFrameCount = frameCount;
		int32 frameCountToRead = 0;

		while(currentFrameCount > 0){
			//Seek to the position
			sf_seek(data->file.rawHandle(), data->position, SEEK_SET);
			//data->file.seek(data->position, SEEK_SET);

			//Get the amount of frames to read
			if(currentFrameCount > (data->file.frames() - data->position)){
				frameCountToRead = data->file.frames() - data->position;
				data->position = 0;
			} else{
				frameCountToRead = currentFrameCount;
				data->position += frameCountToRead;
			}

			//Read X amount of frames into cursor
			sf_readf_int(data->file.rawHandle(), cursor, frameCountToRead);

			cursor += frameCountToRead;
			currentFrameCount -= frameCountToRead;
		}

		return paContinue;
	}

	int Sound::soundPlayback_Once(const void* inputBuffer, void* outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData){
		StreamData* data = static_cast<StreamData*>(userData);
		int32* out = static_cast<int32*>(outputBuffer);
		int32* cursor = out;
		int32 currentFrameCount = frameCount;
		int32 frameCountToRead = 0;

		while(currentFrameCount > 0){
			//Seek to the position
			sf_seek(data->file.rawHandle(), data->position, SEEK_SET);

			//Get the amount of frames to read
			if(currentFrameCount > (data->file.frames() - data->position)){
				return paComplete;
			} else{
				frameCountToRead = currentFrameCount;
				data->position += frameCountToRead;
			}

			//Read X amount of frames into cursor
			sf_readf_int(data->file.rawHandle(), cursor, frameCountToRead);

			cursor += frameCountToRead;
			currentFrameCount -= frameCountToRead;
		}

		return paContinue;
	}
}