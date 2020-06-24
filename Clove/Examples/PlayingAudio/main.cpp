#include <Clove/Clove.hpp>

#include "AudioFile.h"

//An example on how to play audio through Clove

int main(){
	auto audioFactory = clv::createAudioFactory(clv::AudioAPI::OpenAl);

	AudioFile<double> file;
	file.load(SOURCE_DIR "/beep.wav");

	clv::AudioBufferDescriptor descriptor{};
	if(file.getNumChannels() == 1){
		descriptor.format = file.getBitDepth() == 8 ? clv::BufferFormat::Mono8 : clv::BufferFormat::Mono16;
	}else{
		descriptor.format = file.getBitDepth() == 8 ? clv::BufferFormat::Stereo8 : clv::BufferFormat::Stereo16;
	}
	descriptor.sampleRate = file.getSampleRate();

	auto audioBuffer = audioFactory->createAudioBuffer(descriptor, file.samples[0].data(), file.samples[0].size()); //TODO: Use both channels
	auto audioSource = audioFactory->createAudioSource();

	audioSource->setBuffer(*audioBuffer);

	audioSource->setLooping(true);
	audioSource->play();

	while(true);
}