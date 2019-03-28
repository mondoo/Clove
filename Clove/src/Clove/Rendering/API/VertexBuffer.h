#pragma once

namespace clv{
	class CLV_API VertexBuffer{
		//VARIABLES
	private:
		unsigned int rendererID = 0;

		//FUNCTIONS
	public:
		VertexBuffer() = default; //temp
		VertexBuffer(const void* data, unsigned int size);

		void bind() const;
		void unbind()  const;

		void deleteBuffer();
	};
}
