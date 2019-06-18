#pragma once

namespace clv::gfx{
	/*enum class BufferElementFormat{
		FLOAT_2,
		FLOAT_3
	};*/

	enum class BufferElementType{
		position2D,
		position3D,
		texture2D,
		normal,
	};

	class VertexElement{
		//VARIABLES
	private:
		BufferElementType type;
		size_t offset;

		//FUNCTIONS
	public:
		VertexElement(BufferElementType type, size_t offset)
			: type(type)
			, offset(offset){
		}

		size_t getOffset() const{
			return offset;
		}
		size_t getOffsetAfter() const{
			return getOffset() + getSize();
		}

		size_t getSize() const{
			return sizeOf(type);
		}

		BufferElementType getType() const{
			return type;
		}

		static constexpr size_t sizeOf(BufferElementType type){
			switch(type){
				case BufferElementType::position2D:
					return sizeof(math::Vector2f);
				case BufferElementType::position3D:
					return sizeof(math::Vector3f);
				case BufferElementType::texture2D:
					return sizeof(math::Vector2f);
				case BufferElementType::normal:
					return sizeof(math::Vector2f);
				default:
					CLV_ASSERT(false, "Invalid element type");
					return 0u;
			}
		}
	};

	class VertexLayout{
		//VARIABLES
	private:
		std::vector<VertexElement> elements;

		//FUNCTIONS
	public:
		VertexLayout& add(BufferElementType type){
			elements.emplace_back(type, size());
			return *this;
		}

		size_t size(){
			return elements.empty() ? 0u : elements.back().getOffsetAfter();
		}
		size_t count(){
			return elements.size();
		}

		template<BufferElementType type>
		const VertexElement& resolve(){
			for(auto& element : elements){
				if(element.getType() == type){
					return element;
				}
			}
			CLV_ASSERT(false, "Could not find element of type");
			return elements.front();
		}
	};

	class Vertex{
		//VARIABLES
	private:
		char* data = nullptr;
		const VertexLayout& layout;

		//FUNCTIONS
	public:
		template<BufferElementType type>
		auto& attribute(){
			char* attributeData = data + layout.resolve<type>().getOffset();

			if constexpr(type == BufferElementType::position2D){
				return *reinterpret_cast<math::Vector2f*>(attributeData);
			} else if constexpr(type == BufferElementType::position3D){
				return *reinterpret_cast<math::Vector3f*>(attributeData);
			} else if constexpr(type == BufferElementType::texture2D){
				return *reinterpret_cast<math::Vector2f*>(attributeData);
			} else if constexpr(type == BufferElementType::normal){
				return *reinterpret_cast<math::Vector2f*>(attributeData);
			} else{
				CLV_ASSERT(false, "Unable to resolve element type");
				return *reinterpret_cast<char*>(attributeData);
			}
		}

		//Set attribute
		//with recursion

	protected:
		Vertex(char* data, const VertexLayout& layout)
			: data(data)
			, layout(layout){
			CLV_ASSERT(data != nullptr, "Data is nullptr");
		}
	};

	class VertexArray{
		//VARIABLES
	private:
		std::vector<char> buffer; //shouldn't this be an array of vertices? perhaps not because we don't know what the data is
		VertexLayout layout;

		//FUNCTIONS
	public:
		VertexArray(VertexLayout layout)
			: layout(std::move(layout)){
		}

		template<typename ...Args>
		void emplaceBack(Args&&... args){
			//todo add on args to the back
		}

		Vertex front() const{
			//TODO:
		}
		Vertex back() const{
			//TODO:
		}

		Vertex operator[](size_t i) const{
			//TODO:
		}
	};
}