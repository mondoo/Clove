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

#include "imgui.h"

namespace clv{
	RenderLayer::RenderLayer()
		: Layer("Render Layer"){
	}

	void RenderLayer::onAttach(){
		CLV_TRACE("GL version: {0}", glGetString(GL_VERSION));

		GLCall(glDepthFunc(GL_LESS));
		GLCall(glEnable(GL_DEPTH_TEST));

		//src is from the image - dest is what is already in the buffer
		//GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		//GLCall(glEnable(GL_BLEND));
		//I guess it's called blending because you blend the src with the destination

		//Vertex Buffer
		vb = VertexBuffer(positions, sizeof(positions));
		VertexBufferLayout layout;
		layout.push<float>(3); //pos
		//layout.push<float>(2); //tex coord
		va.addBuffer(vb, layout);

		//Index Buffer
		ib = IndexBuffer(indicies, sizeof(indicies) / sizeof(unsigned int));

		//Shaders
		shader = Shader("F:/Clove/Clove/res/Shaders/Basic.shader");
		shader.bind();
		shader.setUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

		//texture = Texture("F:/Clove/Clove/res/Textures/Zombie-32x32.png");
		//texture.bind();
		//shader.setUniform1i("u_Texture", 0);//<- the 0 is the slot that we bound the texture to
	
		va.unbind();
		vb.unbind();
		ib.unbind();
		shader.unbind();
	}

	void RenderLayer::onDetach(){
		shader.deleteShader();
		vb.deleteBuffer();
		ib.deleteBuffer();
		va.deleteArray();
	}

	void RenderLayer::onUpdate(){
		renderer.clear();

		shader.bind();

		float width = static_cast<float>(Application::get().getWindow().getWidth());
		float height = static_cast<float>(Application::get().getWindow().getHeight());

		//mvp matricies = model (the matrix of the thing we are rendering) | view (matrix of the camera) | projection (our projection matrix)
		glm::mat4 objTrans	= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -500.0f));
		glm::mat4 objRot	= glm::rotate(glm::mat4(1.0f), rot * 0.0174532925f, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 objScale	= glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 100.0f));

		glm::mat4 viewTrans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -30.0f, 0.0f));
		glm::mat4 viewRot	= glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 viewScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

		glm::mat4 proj	= glm::perspective(45.0f, 16.0f / 9.0f, 1.0f, -1.0f);
		//glm::mat4 proj	= glm::ortho((width / -2), (width / 2), (height / -2), (height / 2)); //16:9
		glm::mat4 view	= viewTrans * viewRot * viewScale;
		glm::mat4 model = objTrans * objRot * objScale;
		
		//opengl does the proj first
		glm::mat4 mvp = proj * view * model;

		//shader.setUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
		shader.setUniformMat4f("u_MVP", mvp);

		renderer.draw(va, ib, shader);

		if(r > 1.0f || r < 0.0f){
			increment = -increment;
		}
		r += increment;
		rot += 0.5f;
	}

	void RenderLayer::onImGuiRender(){
		static float outfloat;

		//Debug
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::SliderFloat("float", &outfloat, 0.0f, 1.0f);
	}
}