#pragma once

//TODO: Move all of this to .cpp/.inl

namespace clv::gfx{
	enum class VertexElementType{
		position2D,
		position3D,
		texture2D,
		normal,
	};

	class VertexElement{
		//VARIABLES
	private:
		VertexElementType type;
		size_t offset;

		//FUNCTIONS
	public:
		VertexElement(VertexElementType type, size_t offset)
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

		VertexElementType getType() const{
			return type;
		}

		static constexpr size_t sizeOf(VertexElementType type){
			switch(type){
				case VertexElementType::position2D:
					return sizeof(math::Vector2f);
				case VertexElementType::position3D:
					return sizeof(math::Vector3f);
				case VertexElementType::texture2D:
					return sizeof(math::Vector2f);
				case VertexElementType::normal:
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
		VertexLayout& add(VertexElementType type){
			elements.emplace_back(type, size());
			return *this;
		}

		size_t size() const{
			return elements.empty() ? 0u : elements.back().getOffsetAfter();
		}
		size_t count() const{
			return elements.size();
		}

		const VertexElement& resolve(VertexElementType type) const{
			for(auto& element : elements){
				if(element.getType() == type){
					return element;
				}
			}
			CLV_ASSERT(false, "Could not find element of type");
			return elements.front();
		}

		const VertexElement& resolve(size_t i) const{
			return elements[i];
		}
	};

	class Vertex{
		friend class VertexBufferData;

		//VARIABLES
	private:
		char* data = nullptr;
		const VertexLayout& layout;

		//FUNCTIONS
	public:
		template<VertexElementType type>
		auto& getAttribute(){
			char* attributeData = data + layout.resolve(type).getOffset();

			if constexpr(type == VertexElementType::position2D){
				return *reinterpret_cast<math::Vector2f*>(attributeData);
			} else if constexpr(type == VertexElementType::position3D){
				return *reinterpret_cast<math::Vector3f*>(attributeData);
			} else if constexpr(type == VertexElementType::texture2D){
				return *reinterpret_cast<math::Vector2f*>(attributeData);
			} else if constexpr(type == VertexElementType::normal){
				return *reinterpret_cast<math::Vector2f*>(attributeData);
			} else{
				CLV_ASSERT(false, "Unable to resolve element type");
				return *reinterpret_cast<char*>(attributeData);
			}
		}

		//TODO: Set attribute?

	private:
		Vertex(char* data, const VertexLayout& layout)
			: data(data)
			, layout(layout){
			CLV_ASSERT(data != nullptr, "Data is nullptr");
		}

		template<typename DestDataType, typename SourceDataType>
		void setAttribute(char* attribute, SourceDataType&& value){
			if constexpr(std::is_assignable_v<DestDataType, SourceDataType>){
				*reinterpret_cast<DestDataType*>(attribute) = value;
			}else{
				CLV_ASSERT(false, "Types are not assignable");
			}
		}

		template<typename First, typename ...Rest>
		void setAttributeByIndex(size_t i, First&& first, Rest&&... rest){
			setAttributeByIndex(i, std::forward<First>(first));
			setAttributeByIndex(i + 1, std::forward<Rest>(rest)...);
		}

		template<typename T>
		void setAttributeByIndex(size_t i, T&& val){
			const auto& element = layout.resolve(i);
			char* attribute = data + element.getOffset();
			switch(element.getType()){
				case VertexElementType::position2D:
					setAttribute<math::Vector2f>(attribute, std::forward<T>(val));
					break;

				case VertexElementType::position3D:
					setAttribute<math::Vector3f>(attribute, std::forward<T>(val));
					break;

				case VertexElementType::texture2D:
					setAttribute<math::Vector2f>(attribute, std::forward<T>(val));
					break;

				case VertexElementType::normal:
					setAttribute<math::Vector2f>(attribute, std::forward<T>(val));
					break;

				default:
					CLV_ASSERT(false, "Element type is not supported");
					break;
			}
		}
	};

	class VertexBufferData{
		//VARIABLES
	private:
		std::vector<char> buffer;
		VertexLayout layout;

		//FUNCTIONS
	public:
		VertexBufferData(VertexLayout layout)
			: layout(std::move(layout)){
		}

		//TODO: reserve?

		template<typename ...Args>
		void emplaceBack(Args&&... args){
			buffer.resize(buffer.size() + layout.size());
			back().setAttributeByIndex(0u, std::forward<Args>(args)...);
		}

		Vertex front(){
			return { buffer.data(), layout };
		}
		Vertex back(){
			return { (buffer.data() + buffer.size()) - layout.size(), layout };
		}

		Vertex operator[](size_t i){
			return { buffer.data() + (layout.size() * i), layout };
		}

		const char* data(){
			return buffer.data();
		}

		size_t size(){
			return buffer.size() / layout.size();
		}

		const VertexLayout& getLayout(){
			return layout;
		}
	};
}