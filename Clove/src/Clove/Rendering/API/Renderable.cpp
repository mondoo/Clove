#include "clvpch.hpp"
#include "Renderable.hpp"

#include "Clove/Rendering/API/VertexArray.hpp"
#include "Clove/Rendering/API/VertexBuffer.hpp"
#include "Clove/Rendering/API/Material.hpp"
#include "Clove/Rendering/API/Shader.hpp"

namespace clv{
	Renderable::Renderable() = default;

	Renderable::Renderable(Renderable&& other) noexcept = default;

	Renderable::~Renderable() = default;

	void Renderable::bind(Shader& shader){
		vertexArray->bind();
		indexBuffer->bind();
		material->bind(shader);
	}

	void Renderable::unbind(){
		vertexArray->unbind();
		indexBuffer->unbind();
		material->unbind();
	}

	Renderable& Renderable::operator=(Renderable&& other) noexcept = default;
}