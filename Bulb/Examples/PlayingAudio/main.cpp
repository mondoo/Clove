#include <Bulb/Bulb.hpp>

static clv::AudioFormat getBufferFormat(const blb::aud::SoundFile& file) {
    const bool is16 = file.getFormat() == blb::aud::SoundFile::Format::S16;

    if(file.getChannels() == 1) {
        return is16 ? clv::AudioFormat::Mono16 : clv::AudioFormat::Mono8;
    } else {
        return is16 ? clv::AudioFormat::Stereo16 : clv::AudioFormat::Stereo8;
    }
}

int main() {
    blb::aud::SoundFile file{ SOURCE_DIR "/bounce.wav" };

    auto audioFactory = clv::createAudioFactory(clv::AudioAPI::OpenAl);

    auto [rawBuffer, bufferSize] = file.read(file.getFrames());

    clv::AudioDataDescriptor descriptor{};
    descriptor.format     = getBufferFormat(file);
    descriptor.sampleRate = file.getSampleRate();
    descriptor.data       = rawBuffer;
    descriptor.dataSize   = bufferSize;
    auto audioBuffer      = audioFactory->createAudioBuffer(std::move(descriptor));

    delete rawBuffer;

    auto audioSource = audioFactory->createAudioSource();
    audioSource->setBuffer(*audioBuffer);
    audioSource->setLooping(true);
    audioSource->play();

    auto prevFrameTime = std::chrono::system_clock::now();
    while(true) {
        auto currFrameTime                        = std::chrono::system_clock::now();
        std::chrono::duration<float> deltaSeconds = currFrameTime - prevFrameTime;
        prevFrameTime                             = currFrameTime;

        static float total = 0.0f;
        total += deltaSeconds.count();

        float x = sin(total) * 10.0f;
        audioSource->setPosition({ x, 0.0f, 3.0f });

        //Stop after 15 seconds
        if(total >= 15.0f) {
            break;
        }
    }

    return 0;
}