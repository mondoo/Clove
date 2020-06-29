#include <Bulb/Bulb.hpp>

int main(){
    blb::aud::Sound sound(SOURCE_DIR "/beep.wav");

    //bool is16 = sound.

    clv::AudioBufferDescriptor descriptor{};
    if(sound.getChannels() == 1) {
        //descriptor.format = file.getBitDepth() == 8 ? clv::BufferFormat::Mono8 : clv::BufferFormat::Mono16;
    } else {
        //descriptor.format = file.getBitDepth() == 8 ? clv::BufferFormat::Stereo8 : clv::BufferFormat::Stereo16;
    }
    descriptor.sampleRate = sound.getSamplerate();

    auto audioFactory = clv::createAudioFactory(clv::AudioAPI::OpenAl);
    //auto audioBuffer = audioFactory->createAudioBuffer(descriptor, file.samples[0].data(), file.samples[0].size());//TODO: Use both channels
    auto audioSource = audioFactory->createAudioSource();

    //audioSource->setBuffer(*audioBuffer);

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
        audioSource->setPosition({ x, 0.0f, 0.0f });

        //Stop after 15 seconds
        if(total >= 15.0f) {
            break;
        }
    }

    return 0;
}