#pragma once

namespace clv{
	class IndexBuffer{
		//VARIABLES
	private:
		unsigned int rendererID = 0;
		unsigned int count = 0;

		//FUNCTIONS
	public:
		IndexBuffer() = delete;
		IndexBuffer(const IndexBuffer& other) = delete;
		IndexBuffer(IndexBuffer&& other) noexcept;
		
		IndexBuffer(const unsigned int* data, unsigned int inCount);

		~IndexBuffer();

		void bind() const;
		void unbind() const;

		//TODO: Move to inl
		inline unsigned int getCount() const{ return count; }

		IndexBuffer& operator=(const IndexBuffer& other) = delete;
		IndexBuffer& operator=(IndexBuffer&& other) noexcept;
	};
}
