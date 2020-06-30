#include <Bulb/Bulb.hpp>

int main(){
    blb::aud::Sound sound(SOURCE_DIR "/bounce.wav");

    bool is16 = (sound.file.format() & SF_FORMAT_PCM_16) != 0;

    clv::AudioBufferDescriptor descriptor{};
    if(sound.getChannels() == 1) {
        descriptor.format = is16 ? clv::BufferFormat::Mono16 : clv::BufferFormat::Mono8;
    } else {
        descriptor.format = is16 ? clv::BufferFormat::Stereo16 : clv::BufferFormat::Stereo8;
    }
    descriptor.sampleRate = sound.getSamplerate();

    short* buff = new short[sound.getFrames() * sound.getChannels()];
    sound.file.readf(buff, sound.getFrames());

    auto audioFactory = clv::createAudioFactory(clv::AudioAPI::OpenAl);
    auto audioBuffer = audioFactory->createAudioBuffer(descriptor, buff, sound.getFrames() * sound.getChannels() * sizeof(short));
    auto audioSource = audioFactory->createAudioSource();

    delete[] buff;

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