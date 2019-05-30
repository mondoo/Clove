#pragma once

#include "Clove/Graphics/Bindables/Bindable.hpp"

namespace clv::gfx{
	struct Vertex{
		struct{
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
		} position;

		struct{
			float u = 0.0f;
			float v = 0.0f;
		} texCoord;

		struct{
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
		} normal;
	};

	class VertexBuffer : public Bindable{
		//FUNCTIONS
	public:
		VertexBuffer();
		VertexBuffer(const VertexBuffer& other) = delete;
		VertexBuffer(VertexBuffer&& other) noexcept;
		VertexBuffer& operator=(const VertexBuffer& other) = delete;
		VertexBuffer& operator=(VertexBuffer&& other) noexcept;
		virtual ~VertexBuffer();
	};
}

