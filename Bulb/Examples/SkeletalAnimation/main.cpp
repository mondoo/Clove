#include <Bulb/Bulb.hpp>

int main() {
    //Create the platform instance we'll use to make OS calls
    std::unique_ptr<clv::plt::Platform> platformInstance = clv::plt::createPlatformInstance();

    //Create the window which we will render to
    std::shared_ptr<clv::plt::Window> window = platformInstance->createWindow({ "Rotating Cube", 1280, 720, clv::plt::getPlatformPreferedAPI() });
    window->setVSync(true);

    //Cache of the 'previous frame time' that'll be used to calculate the delta time
    auto prevFrameTime = std::chrono::system_clock::now();

    //Run our program while the window is open
    while(window->isOpen()) {
        //Calculate the delta time
        auto currFrameTime                        = std::chrono::system_clock::now();
        std::chrono::duration<float> deltaSeconds = currFrameTime - prevFrameTime;
        prevFrameTime                             = currFrameTime;

        //Tell our window we're beginning a frame so it can handle any input events
        window->beginFrame();

        //TODO...

        //Tell our window we've finished frame so it can present it's back buffer
        window->endFrame();
    }

    return 0;
}