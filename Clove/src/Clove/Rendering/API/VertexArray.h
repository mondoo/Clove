#pragma once

namespace clv{
	class VertexBuffer;
	class VertexBufferLayout;

	class CLV_API VertexArray{
		//VARIABLES
	private:
		unsigned int rendererID;

		//FUNCTIONS
	public:
		VertexArray();
		~VertexArray();

		void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

		void bind() const;
		void unbind() const;
	};
}
