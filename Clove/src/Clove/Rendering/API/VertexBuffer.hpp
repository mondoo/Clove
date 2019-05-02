#pragma once

namespace clv{
	class VertexBuffer{
		//VARIABLES
	private:
		unsigned int rendererID = 0;

		//FUNCTIONS
	public:
		VertexBuffer() = delete;
		VertexBuffer(const VertexBuffer& other) = delete;
		VertexBuffer(VertexBuffer&& other) noexcept;
		
		VertexBuffer(const void* data, unsigned int size);

		~VertexBuffer();

		void bind() const;
		void unbind()  const;

		VertexBuffer& operator=(const VertexBuffer& other) = delete;
		VertexBuffer& operator=(VertexBuffer&& other) noexcept;
	};
}
