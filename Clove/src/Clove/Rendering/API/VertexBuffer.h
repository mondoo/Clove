#pragma once

namespace clv{
	class CLV_API VertexBuffer{
		//VARIABLES
	private:
		unsigned int rendererID = 0;

		//FUNCTIONS
	public:
		VertexBuffer(const void* data, unsigned int size);
		VertexBuffer(VertexBuffer&& other);

		~VertexBuffer();

		void bind() const;
		void unbind()  const;

		VertexBuffer& operator=(VertexBuffer&& other);
	};
}
