#include "clvpch.hpp"
#include "Application.hpp"

#include "Clove/Platform/Window.hpp"
#include "Clove/Input/Input.hpp"
#include "Clove/LayerStack.hpp"
#include "Clove/Layer.hpp"
#include "Clove/Utils/DeltaTime.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/RenderCommand.hpp"

//audio test
#include <portaudio.h>
#include <sndfile.h>

//audio test
struct paTestData{
	float left_phase;
	float right_phase;
};

static int patestCallback(const void* inputBuffer, void* outputBuffer,
						  unsigned long framesPerBuffer,
						  const PaStreamCallbackTimeInfo* timeInfo,
						  PaStreamCallbackFlags statusFlags,
						  void* userData){
	/* Cast data passed through stream to our structure. */
	paTestData* data = (paTestData*)userData;
	float* out = (float*)outputBuffer;
	unsigned int i;
	(void)inputBuffer; /* Prevent unused variable warning. */

	for(i = 0; i < framesPerBuffer; i++){
		*out++ = data->left_phase;  /* left */
		*out++ = data->right_phase;  /* right */
		/* Generate simple sawtooth phaser that ranges between -1.0 and 1.0. */
		data->left_phase += 0.01f;
		/* When signal reaches top, drop back down. */
		if(data->left_phase >= 1.0f) data->left_phase -= 2.0f;
		/* higher pitch so we can distinguish left and right. */
		data->right_phase += 0.03f;
		if(data->right_phase >= 1.0f) data->right_phase -= 2.0f;
	}
	return 0;
}

namespace clv{
	Application* Application::instance = nullptr;

	Application::Application(){
		CLV_ASSERT(!instance, "Application already exists!");
		instance = this;

		window = std::unique_ptr<Window>(Window::create());
		window->onWindowCloseDelegate.bind(&Application::onWindowClose, this);
		window->setVSync(true);

		gfx::RenderCommand::initialiseRenderAPI(window->getContext());
		gfx::RenderCommand::setClearColour({ 1.0f, 0.54f, 0.1f, 1.0f });

		gfx::Renderer::initialise();

		ecsManager = std::make_unique<ecs::Manager>();
		layerStack = std::make_unique<LayerStack>();

		CLV_LOG_INFO("Successfully initialised Clove");

		prevFrameTime = std::chrono::system_clock::now();

		CLV_LOG_DEBUG("Start port audio test");
		//audio test
		auto err = Pa_Initialize();
		if(err == paNoError){
			paTestData data = { 1.0f, 1.0f };
			PaStream* stream;
			PaError err;
			/* Open an audio I/O stream. */
			err = Pa_OpenDefaultStream(&stream,
									   0,          /* no input channels */
									   2,          /* stereo output */
									   paFloat32,  /* 32 bit floating point output */
									   44100,
									   256,        /* frames per buffer, i.e. the number
														  of sample frames that PortAudio will
														  request from the callback. Many apps
														  may want to use
														  paFramesPerBufferUnspecified, which
														  tells PortAudio to pick the best,
														  possibly changing, buffer size.*/
									   patestCallback, /* this is your callback function */
									   &data); /*This is a pointer that will be passed to
														  your callback*/
			if(err == paNoError){
				err = Pa_StartStream(stream);

				if(err == paNoError){
					Pa_Sleep(1 * 1000);

					Pa_StopStream(stream);

					err = Pa_CloseStream(stream);
					if(err == paNoError){

					} else{
						CLV_LOG_ERROR("Port audio error: {0} ", Pa_GetErrorText(err));
					}
				} else{
					CLV_LOG_ERROR("Port audio error: {0} ", Pa_GetErrorText(err));
				}
			} else{
				CLV_LOG_ERROR("Port audio error: {0} ", Pa_GetErrorText(err));
			}
		} else{
			CLV_LOG_ERROR("Port audio error: {0} ", Pa_GetErrorText(err));
		}
	}

	Application::~Application(){
		Pa_Terminate();
	}

	void Application::run(){
		while(running){
			auto currFrameTime = std::chrono::system_clock::now();
			std::chrono::duration<float> deltaSeonds = currFrameTime - prevFrameTime;
			prevFrameTime = currFrameTime;

			window->beginFrame();

			//TODO:
			//Will need process the mouse and keyboard events here eventually

			for(auto layer : *layerStack){
				layer->onUpdate(deltaSeonds.count());
			}

			gfx::RenderCommand::clear();

			gfx::Renderer::beginScene();

			ecsManager->update(deltaSeonds.count());

            gfx::Renderer::endScene();
			
			window->endFrame();
		}
	}

	void Application::stop(){
		running = false;
	}

	void Application::pushLayer(std::shared_ptr<Layer> layer){
		layerStack->pushLayer(layer);
	}

	void Application::pushOverlay(std::shared_ptr<Layer> overlay){
		layerStack->pushOverlay(overlay);
	}

	Application& Application::get(){
		return *instance;
	}

	Window& Application::getWindow(){
		return *window;
	}

	ecs::Manager& Application::getManager(){
		return *ecsManager;
	}

	void Application::onWindowClose(){
		running = false;
	}
}