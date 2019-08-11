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
#include <sndfile.hh>

//audio test
struct paTestData{
	float left_phase;
	float right_phase;
};

struct callback_data_s{
	SNDFILE* file;
	SF_INFO      info;
};

static int patestCallback(const void* inputBuffer, void* outputBuffer,
						  unsigned long framesPerBuffer,
						  const PaStreamCallbackTimeInfo* timeInfo,
						  PaStreamCallbackFlags statusFlags,
						  void* userData){
	float* out;
	callback_data_s* p_data = (callback_data_s*)userData;
	sf_count_t       num_read;

	out = (float*)outputBuffer;
	p_data = (callback_data_s*)userData;

	/* clear output buffer */
	memset(out, 0, sizeof(float) * framesPerBuffer * p_data->info.channels);

	/* read directly into output buffer */
	num_read = sf_read_float(p_data->file, out, framesPerBuffer * p_data->info.channels);

	/*  If we couldn't read a full frameCount of samples we've reached EOF */
	if(num_read < framesPerBuffer){
		return paComplete;
	}

	return paContinue;
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
		
		/*auto file = SndfileHandle("res/Audio/Test.wav");
		int16 buffer[1024];
		file.read(buffer, 1024);*/

		callback_data_s data;
		data.file = sf_open("res/Audio/Test.wav", SFM_READ, &data.info);
		

		CLV_LOG_DEBUG("Start port audio test");
		//audio test
		auto err = Pa_Initialize();
		if(err == paNoError){
			//paTestData data = { 1.0f, 1.0f };
			PaStream* stream;
			PaError err;
			/* Open an audio I/O stream. */
			err = Pa_OpenDefaultStream(&stream,
									   0,          /* no input channels */
									   data.info.channels,          /* stereo output */
									   paFloat32,  /* 32 bit floating point output */
									   data.info.samplerate,
									   512,        /* frames per buffer, i.e. the number
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
					Pa_Sleep(5 * 1000);

					/*while(Pa_IsStreamActive(stream)){
						Pa_Sleep(100);
					}*/

					Pa_StopStream(stream);

					sf_close(data.file);

					err = Pa_CloseStream(stream);

					Pa_Terminate();
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

	Application::~Application() = default;

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