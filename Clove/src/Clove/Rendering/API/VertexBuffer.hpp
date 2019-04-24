#pragma once

namespace clv{
	class VertexBuffer{
		//VARIABLES
	private:
		unsigned int rendererID = 0;

		//FUNCTIONS
	public:
		VertexBuffer(const void* data, unsigned int size);
		VertexBuffer(VertexBuffer&& other) noexcept;

		~VertexBuffer();

		void bind() const;
		void unbind()  const;

		VertexBuffer& operator=(VertexBuffer&& other) noexcept;
	};
}
