#include "clvpch.h"
#include "RenderLayer.h"

#include <fstream>
#include <string>
#include <sstream>

#include "Clove/Rendering/Renderer.h"

namespace clv{
	RenderLayer::RenderLayer()
		: Layer("Render Layer"){
	}

	void RenderLayer::onAttach(){
		CLV_CORE_TRACE("GL version: {0}", glGetString(GL_VERSION));

		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		//Vertex Buffer
		vb = VertexBuffer(positions, sizeof(positions));
		VertexBufferLayout layout;
		layout.push<float>(2); //pos
		layout.push<float>(2); //tex coord
		va.addBuffer(vb, layout);

		//Index Buffer
		ib = IndexBuffer(indicies, sizeof(indicies) / sizeof(unsigned int));

		//Shaders
		shader = Shader("F:/Clove/Clove/res/Shaders/Basic.shader");
		shader.bind();
		shader.setUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

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