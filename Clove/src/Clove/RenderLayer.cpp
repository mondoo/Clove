#include "clvpch.h"
#include "RenderLayer.h"

#include <fstream>
#include <string>
#include <sstream>

#include "Clove/Rendering/Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Application.h"
#include "Window.h"

namespace clv{
	RenderLayer::RenderLayer()
		: Layer("Render Layer"){
	}

	void RenderLayer::onAttach(){
		CLV_CORE_TRACE("GL version: {0}", glGetString(GL_VERSION));

		//src is from the image - dest is what is already in the buffer
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));
		//I guess it's called blending because you blend the src with the destination

		//Vertex Buffer
		vb = VertexBuffer(positions, sizeof(positions));
		VertexBufferLayout layout;
		layout.push<float>(2); //pos
		layout.push<float>(2); //tex coord
		va.addBuffer(vb, layout);

		//Index Buffer
		ib = IndexBuffer(indicies, sizeof(indicies) / sizeof(unsigned int));

		//projection matrix

		//mvp matricies = model (the matrix of the thing we are rendering) | view (matrix of the camera) | projection (our projection matrix)

		unsigned int width = Application::get().getWindow().getWidth();
		unsigned int height = Application::get().getWindow().getHeight();

		glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(width), -0.0f, static_cast<float>(height)); //16:9
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, 0.0f));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));

		//opengl does the proj first
		glm::mat4 mvp = proj * view * model;

		//Shaders
		shader = Shader("F:/Clove/Clove/res/Shaders/Basic.shader");
		shader.bind();
		shader.setUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
		shader.setUniformMat4f("u_MVP", mvp);

		texture = Texture("F:/Clove/Clove/res/Textures/Zombie-32x32.png");
		texture.bind();
		shader.setUniform1i("u_Texture", 0);//<- the 0 is the slot that we bound the texture to
	
		va.unbind();
		vb.unbind();
		ib.unbind();
		shader.unbind();
	}

	void RenderLayer::onDetach(){
		shader.deleteShader();
		vb.deleteBuffer();
		ib.deleteBuffer();
	}

	void RenderLayer::onUpdate(){
		renderer.clear();

		shader.bind();
		shader.setUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

		renderer.draw(va, ib, shader);

		if(r > 1.0f || r < 0.0f){
			increment = -increment;
		}
		r += increment;
	}
}