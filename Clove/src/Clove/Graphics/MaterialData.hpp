#pragma once

namespace clv::gfx{
	//template<typename T>
	class MaterialData{
		//VARIABLES
	private:
		unsigned int bindingPoint = 0;
		std::vector<char> dataBuffer; //Storing it like this so no leaks happen but most operations will be char*

		//FUNCTIONS
	public:
		//TODO: ctors
		
		//We might need to template this so we can get this size of the data to resizr the data buffer
		//MaterialData(unsigned int bindingPoint, char* data);
		//-Maybe not a ctor

		//TODO .inl
		template<typename T>
		void set(T&& data){
			dataBuffer.clear();
			dataBuffer.resize(sizeof(T));
			dataBuffer.data() = &data; //Not sure this'll do what I want
			//I think the vertex layout works because it resizes it's own internal vector then gives a pointer to a position in that to the vertex to fill
		}

		template<typename T>
		T& get(){

		}
	};
}
