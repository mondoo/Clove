#include <Bulb/Bulb.hpp>

//Demonstrates how to stream audio in Bulb / Clove

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
    blb::aud::SoundFile file{ SOURCE_DIR "/countdown.wav" };

    //Create an AudioFactory that uses OpenAL
    //An AudioFactory creates all of the components needed to play audio
    std::unique_ptr<clv::AudioFactory> audioFactory = clv::createAudioFactory(clv::AudioAPI::OpenAl);

    //Create 2 empty buffers. One will be playing audio and the other will
    //have the next section of audio data read into
    std::shared_ptr<clv::AudioBuffer> bufferA = audioFactory->createAudioBuffer();
    std::shared_ptr<clv::AudioBuffer> bufferB = audioFactory->createAudioBuffer();

    //Create the AudioSource to control where the sound is played from
    std::unique_ptr<clv::AudioSource> audioSource = audioFactory->createAudioSource();

    //Stream with 2 buffers each having a quater of the file
    const uint32_t framesPerBuffer = file.getFrames() / 4;

    //Get some data set up initially
    {
        //Reading also seeks the file's cursor along
        auto [bufferAData, bufferASize] = file.read(framesPerBuffer);
        auto [bufferBData, bufferBSize] = file.read(framesPerBuffer);

        //Map buffers
        {
            clv::AudioBuffer::DataInfo bufferData{};
            bufferData.format     = getBufferFormat(file);
            bufferData.sampleRate = file.getSampleRate();
            bufferData.data       = bufferAData;
            bufferData.dataSize   = bufferASize;
            bufferA->map(std::move(bufferData));
        }
        {
            clv::AudioBuffer::DataInfo bufferData{};
            bufferData.format     = getBufferFormat(file);
            bufferData.sampleRate = file.getSampleRate();
            bufferData.data       = bufferBData;
            bufferData.dataSize   = bufferBSize;
            bufferB->map(std::move(bufferData));
        }

        //Clean up raw data after usage
        delete bufferAData;
        delete bufferBData;
    }

    //Put our buffers into the AudioSource's internal queue
    audioSource->queueBuffers({ bufferA, bufferB });
    //Start plating the queue
    audioSource->play();

    auto prevFrameTime = std::chrono::system_clock::now();
    while(true) {
        auto currFrameTime                        = std::chrono::system_clock::now();
        std::chrono::duration<float> deltaSeconds = currFrameTime - prevFrameTime;
        prevFrameTime                             = currFrameTime;

        static float total = 0.0f;
        total += deltaSeconds.count();

        //Stop after 30 seconds
        if(total >= 30.0f) {
            break;
        }

        //While we have a buffer that has been processed. Load up the
        //next chunk of data and then queue that buffer
        uint32_t numProcessed = audioSource->getNumBuffersProcessed();
        if(numProcessed > 0) {
            while(numProcessed-- > 0) {
                auto buffer = audioSource->unQueueBuffers(1)[0];

                auto [data, size] = file.read(framesPerBuffer);

                clv::AudioBuffer::DataInfo bufferData{};
                bufferData.format     = getBufferFormat(file);
                bufferData.sampleRate = file.getSampleRate();
                bufferData.data       = data;
                bufferData.dataSize   = size;
                buffer->map(std::move(bufferData));

                audioSource->queueBuffers({ buffer });
            }
        }
    }

    return 0;
}
