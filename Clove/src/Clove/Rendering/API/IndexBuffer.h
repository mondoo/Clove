#pragma once

namespace clv{
	class CLV_API IndexBuffer{
		//VARIABLES
	private:
		unsigned int rendererID = 0;
		unsigned int count = 0;

		//FUNCTIONS
	public:
		IndexBuffer() = default; //temp
		IndexBuffer(const unsigned int* data, unsigned int inCount);
		~IndexBuffer();

		void bind() const;
		void unbind() const;

		inline unsigned int getCount() const{ return count; }
	};
}
