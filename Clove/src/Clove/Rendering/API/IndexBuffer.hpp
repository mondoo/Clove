#pragma once

namespace clv{
	class IndexBuffer{
		//VARIABLES
	private:
		unsigned int rendererID = 0;
		unsigned int count = 0;

		//FUNCTIONS
	public:
		IndexBuffer(const unsigned int* data, unsigned int inCount);
		IndexBuffer(IndexBuffer&& other) noexcept;

		~IndexBuffer();

		void bind() const;
		void unbind() const;

		//TODO: Move to inl
		inline unsigned int getCount() const{ return count; }

		IndexBuffer& operator=(IndexBuffer&& other) noexcept;
	};
}
