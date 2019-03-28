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

		void bind() const;
		void unbind() const;

		void deleteBuffer();

		inline unsigned int getCount() const{ return count; }
	};
}
