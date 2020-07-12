#include <Bulb/Bulb.hpp>

static clv::AudioBuffer::Format getBufferFormat(const blb::aud::SoundFile& file) {
    const bool is16 = file.getFormat() == blb::aud::SoundFile::Format::S16;

    if(file.getChannels() == 1) {
        return is16 ? clv::AudioBuffer::Format::Mono16 : clv::AudioBuffer::Format::Mono8;
    } else {
        return is16 ? clv::AudioBuffer::Format::Stereo16 : clv::AudioBuffer::Format::Stereo8;
    }
}

int main() {
    blb::aud::SoundFile file{ SOURCE_DIR "/countdown.wav" };

    auto audioFactory = clv::createAudioFactory(clv::AudioAPI::OpenAl);

    std::shared_ptr<clv::AudioBuffer> bufferA = audioFactory->createAudioBuffer();
    std::shared_ptr<clv::AudioBuffer> bufferB = audioFactory->createAudioBuffer();

    auto audioSource = audioFactory->createAudioSource();
    
    //Stream with 2 buffers each having a quater of the file
    const uint32_t framesPerBuffer = file.getFrames() / 4;

    //Get some data set up initially
    {
        //Reading also seeks the file's cursor along along
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

        delete bufferAData;
        delete bufferBData;
    }

    audioSource->queueBuffers({ bufferA, bufferB });

    audioSource->play();

    auto prevFrameTime = std::chrono::system_clock::now();
    while(true) {
        auto currFrameTime                        = std::chrono::system_clock::now();
        std::chrono::duration<float> deltaSeconds = currFrameTime - prevFrameTime;
        prevFrameTime                             = currFrameTime;

        static float total = 0.0f;
        total += deltaSeconds.count();

        //Stop after 15 seconds
        if(total >= 30.0f) {
            break;
        }

        //Update buffers
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
