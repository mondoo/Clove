#pragma once

namespace clv{
	class VertexBuffer;
	class VertexBufferLayout;

	class VertexArray{
		//VARIABLES
	private:
		unsigned int rendererID;

		//FUNCTIONS
	public:
		VertexArray();
		VertexArray(const VertexArray& other) = delete;
		VertexArray(VertexArray&& other) noexcept;

		~VertexArray();

		void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

		void bind() const;
		void unbind() const;

		VertexArray& operator=(const VertexArray& other) = delete;
		VertexArray& operator=(VertexArray&& other) noexcept;
	};
}
