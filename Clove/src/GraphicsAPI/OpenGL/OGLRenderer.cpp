#include "clvpch.hpp"
#include "OGLRenderer.hpp"

#include "Clove/Window.hpp"
#include "Clove/Rendering/API/GLHelpers.hpp"
#include "Clove/Rendering/API/VertexArray.hpp"
#include "Clove/Rendering/API/IndexBuffer.hpp"
#include "Clove/Rendering/API/Shader.hpp"
#include "Clove/Rendering/API/Material.hpp"
#include "Clove/Scene/Scene.hpp"
#include "Clove/Scene/RenderableSceneNode.hpp"
#include "Clove/Scene/CameraSceneNode.hpp"
#include "Clove/Scene/Lights/PointLightSceneNode.hpp"
#include "Clove/Scene/Lights/DirectionalLightSceneNode.hpp"

#if CLV_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsException.hpp"
#endif

#include <glad/glad.h>

namespace clv{
	namespace graphics{
		OGLRenderer::OGLRenderer(const Window& window){
		#if CLV_PLATFORM_WINDOWS
			windowsHandle = reinterpret_cast<HWND>(window.getNativeWindow());

			windowsDeviceContext = GetDC(windowsHandle);

			PIXELFORMATDESCRIPTOR pfd = { 0 };
			pfd.nSize = sizeof(pfd);
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = 32;
			pfd.cAlphaBits = 8;
			pfd.cDepthBits = 24;

			int pf;
			pf = ChoosePixelFormat(windowsDeviceContext, &pfd);
			if(pf == 0){
				throw CLV_WINDOWS_LAST_EXCEPTION;
			}

			if(SetPixelFormat(windowsDeviceContext, pf, &pfd) == FALSE){
				throw CLV_WINDOWS_LAST_EXCEPTION;
			}

			windowsResourceContext = wglCreateContext(windowsDeviceContext);
			wglMakeCurrent(windowsDeviceContext, windowsResourceContext);
		#endif

			CLV_TRACE("Device context created");

			CLV_ASSERT(gladLoadGL(), "Failed to load OpenGL functions");

			CLV_INFO("Rendering environment created successfully!");
			CLV_INFO("GL version: {0}", glGetString(GL_VERSION));

			CLV_TRACE("Enabling Depth buffer");
			GLCall(glDepthFunc(GL_LESS));
			GLCall(glEnable(GL_DEPTH_TEST));

			CLV_TRACE("Blend set to: SRC_ALPHA | ONE_MINUS_SRC_ALPHA");
			//src is from the image - dest is what is already in the buffer
			GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
			GLCall(glEnable(GL_BLEND));
			//I guess it's called blending because you blend the src with the destination

			defaultShader = std::make_unique<Shader>();
			defaultShader->attachShader(ShaderTypes::Vertex, "../Clove/res/Shaders/VertexShader.glsl");
			defaultShader->attachShader(ShaderTypes::Fragment, "../Clove/res/Shaders/FragmentShader.glsl");

			lightShader = std::make_unique<Shader>();
			lightShader->attachShader(ShaderTypes::Vertex, "../Clove/res/Shaders/VertexShader.glsl");
			lightShader->attachShader(ShaderTypes::Fragment, "../Clove/res/Shaders/LightFragShader.glsl");

			currentShaderType = ShaderType::standard;

			currentShader = defaultShader.get();
			currentShader->bind();
		}

		OGLRenderer::~OGLRenderer(){
			defaultShader.reset();
			lightShader.reset();

		#if CLV_PLATFORM_WINDOWS
			ReleaseDC(windowsHandle, windowsDeviceContext);
			wglDeleteContext(windowsResourceContext);	
		#endif
		}

		void OGLRenderer::clear(){
		#if CLV_PLATFORM_WINDOWS
			SwapBuffers(windowsDeviceContext);
		#endif
			GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		}

		void OGLRenderer::drawScene(std::shared_ptr<scene::Scene> scene){
			std::shared_ptr<scene::CameraSceneNode> camera = scene->getActiveCamera();

			if(!camera){
				CLV_ERROR("No active camera in current scene. Unable to draw current queue");
				return;
			}

			const size_t directionalLightNum	= scene->getDirectionalLights().size();
			const size_t pointLightNum			= scene->getPointLights().size();

			if(prevDirLightSize != directionalLightNum){
				defaultShader->setNumberOfDirectionalLights(directionalLightNum);
				prevDirLightSize = directionalLightNum;
			}

			if(prevPointLightSize != pointLightNum){
				defaultShader->setNumberOfPointLights(pointLightNum);
				prevPointLightSize = pointLightNum;
			}

			currentShader->setUniform("viewPos", camera->getPosition());
			currentShader->setUniform("view", camera->getLookAt());
			currentShader->setUniform("projection", camera->getProjection());

			std::vector < std::shared_ptr<scene::DirectionalLightSceneNode>> dirLight = scene->getDirectionalLights();
			for(int i = 0; i < directionalLightNum; ++i){
				defaultShader->setUniform("directionLights[" + std::to_string(i) + "].direction", dirLight[i]->getDirection());

				defaultShader->setUniform("directionLights[" + std::to_string(i) + "].ambient", dirLight[i]->getAmbientColour());
				defaultShader->setUniform("directionLights[" + std::to_string(i) + "].diffuse", dirLight[i]->getDiffuseColour());
				defaultShader->setUniform("directionLights[" + std::to_string(i) + "].specular", dirLight[i]->getSpecularColour());
			}

			std::vector<std::shared_ptr<scene::PointLightSceneNode>> lights = scene->getPointLights();
			for(int i = 0; i < pointLightNum; ++i){
				defaultShader->setUniform("pointLights[" + std::to_string(i) + "].position", lights[i]->getPosition());

				defaultShader->setUniform("pointLights[" + std::to_string(i) + "].ambient", lights[i]->getAmbientColour());
				defaultShader->setUniform("pointLights[" + std::to_string(i) + "].diffuse", lights[i]->getDiffuseColour());
				defaultShader->setUniform("pointLights[" + std::to_string(i) + "].specular", lights[i]->getSpecularColour());

				defaultShader->setUniform("pointLights[" + std::to_string(i) + "].constant", lights[i]->getConstant());
				defaultShader->setUniform("pointLights[" + std::to_string(i) + "].linear", lights[i]->getLinear());
				defaultShader->setUniform("pointLights[" + std::to_string(i) + "].quadratic", lights[i]->getQuadratic());
			}

			for(const auto& renderable : scene->getRenderables()){
				prepareShader(renderable->getShaderType(), camera);

				currentShader->setUniform("model", renderable->getWorldTransform());

				renderable->bind(*currentShader);
				GLCall(glDrawElements(GL_TRIANGLES, renderable->getIndexBufferCount(), GL_UNSIGNED_INT, nullptr));
				renderable->unbind();
			}
		}

		void OGLRenderer::prepareShader(ShaderType type, std::shared_ptr<scene::CameraSceneNode> camera){
			if(type != currentShaderType){
				switch(type){
					case ShaderType::standard:
						currentShader = defaultShader.get();
						break;
					case ShaderType::light:
						currentShader = lightShader.get();
						break;

					default:
						CLV_ERROR("Shader type {0} not supported! Using default shader", static_cast<int>(type));
						currentShader = defaultShader.get();
						break;
				}

				currentShaderType = type;
				currentShader->bind();

				currentShader->setUniform("viewPos", camera->getPosition());
				currentShader->setUniform("view", camera->getLookAt());
				currentShader->setUniform("projection", camera->getProjection());
			}
		}

		Renderer* Renderer::createOpenGLRenderer(const Window& window){
			return new OGLRenderer(window);
		}
	}
}