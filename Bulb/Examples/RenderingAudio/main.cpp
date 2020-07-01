#include <Bulb/Bulb.hpp>

int main(){
    auto audioFactory = clv::createAudioFactory(clv::AudioAPI::OpenAl);

    auto audioBuffer = blb::aud::SoundLoader::loadSound(*audioFactory, SOURCE_DIR "/bounce.wav");

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