#pragma once

/*
how does this class help?
how do i now givew this to the buffers?
do i refactor the cb / ubo to not take the templated type?
has this really helped anything?


should this be how the base sbo class be handled? - still needs template stuff so maybe THAT should take one of these?
*/


/*
Do I even need this class then if I'm taking everything in as raw data?
Some of the cbs / ubos won't even need to have custom defined structs (the ones that are a single param)

*/

namespace clv::gfx{
	class MaterialData{
		//VARIABLES
	private:
		unsigned int bindingPoint = 0;
		size_t size = 0;
		std::vector<char> dataBuffer; //Storing it like this so no leaks happen but most operations will be char*

		//FUNCTIONS
	public:
		MaterialData() = delete;
		MaterialData(const MaterialData& other);
		MaterialData& operator=(const MaterialData& other);
		MaterialData(MaterialData&& other) noexcept;
		MaterialData& operator=(MaterialData&& other) noexcept;
		~MaterialData();

		MaterialData(unsigned int bindingPoint);


		//TODO .inl
		template<typename T>
		void set(T&& data){
			dataBuffer.clear();
			size = sizeof(T);
			dataBuffer.resize(size);
			char* front = dataBuffer.data();
			*reinterpret_cast<T*>(front) = data;
		}

		template<typename T>
		T& get(){
			return *reinterpret_cast<T*>(data);
		}
		const char* getBuffer() const{
			return dataBuffer.data();
		}

		size_t getSize() const;
	};
}
