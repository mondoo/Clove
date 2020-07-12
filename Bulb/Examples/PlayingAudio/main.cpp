#include <Bulb/Bulb.hpp>

//Demonstrates how to play audio in Bulb / Clove

//Helper funcntion for converting a SoundFile format into an AudioBuffer format
static clv::AudioBuffer::Format getBufferFormat(const blb::aud::SoundFile& file) {
    const bool is16 = file.getFormat() == blb::aud::SoundFile::Format::S16;

    if(file.getChannels() == 1) {
        return is16 ? clv::AudioBuffer::Format::Mono16 : clv::AudioBuffer::Format::Mono8;
    } else {
        return is16 ? clv::AudioBuffer::Format::Stereo16 : clv::AudioBuffer::Format::Stereo8;
    }
}

int main() {
	//Load our file to play audio from
    blb::aud::SoundFile file{ SOURCE_DIR "/bounce.wav" };

	//Create an AudioFactory that uses OpenAL
	//An AudioFactory creates all of the components needed to play audio
    std::unique_ptr<clv::AudioFactory> audioFactory = clv::createAudioFactory(clv::AudioAPI::OpenAl);

	//Read the entire file contents into a buffer
    auto [rawBuffer, bufferSize] = file.read(file.getFrames());

	//Pass that data along to the AudioFactory to create an AudioBuffer from
    clv::AudioBuffer::DataInfo info{};
    info.format     = getBufferFormat(file);
    info.sampleRate = file.getSampleRate();
    info.data       = rawBuffer;
    info.dataSize   = bufferSize;
	
	std::unique_ptr<clv::AudioBuffer> audioBuffer = audioFactory->createAudioBuffer(std::move(info));

	//Delete the rawBuffer obtained from the file
    delete rawBuffer;

	//Create an AudioSource to configure where audio is played from
    std::unique_ptr<clv::AudioSource> audioSource = audioFactory->createAudioSource();
    audioSource->setBuffer(*audioBuffer);
    audioSource->setLooping(true);
    audioSource->play();

	//Get previous frame time to calculate the delta between frames
    auto prevFrameTime = std::chrono::system_clock::now();
    while(true) {
        auto currFrameTime                        = std::chrono::system_clock::now();
        std::chrono::duration<float> deltaSeconds = currFrameTime - prevFrameTime;
        prevFrameTime                             = currFrameTime;

        static float total = 0.0f;
        total += deltaSeconds.count();

		//Move the audio source right to left
        float x = sin(total) * 10.0f;
        audioSource->setPosition({ x, 0.0f, 3.0f });

        //Stop after 15 seconds
        if(total >= 15.0f) {
            break;
        }
    }

    return 0;
}
