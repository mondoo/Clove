namespace clv::gfx{
	constexpr size_t VertexElement::sizeOf(VertexElementType type){
		switch(type){
			case VertexElementType::position2D:
				return sizeof(math::Vector2f);
			case VertexElementType::position3D:
				return sizeof(math::Vector3f);
			case VertexElementType::texture2D:
				return sizeof(math::Vector2f);
			case VertexElementType::normal:
				return sizeof(math::Vector3f);
			default:
				CLV_ASSERT(false, "Invalid element type");
				return 0u;
		}
	}

	constexpr unsigned int VertexElement::countOf(VertexElementType type){
		switch(type){
			case VertexElementType::position2D:
				return 2u; //sizeof(math::Vector2f) / size(float) ???
			case VertexElementType::position3D:
				return 3u;
			case VertexElementType::texture2D:
				return 2u;
			case VertexElementType::normal:
				return 3u;
			default:
				CLV_ASSERT(false, "Invalid element type");
				return 0u;
		}
	}

	template<VertexElementType type>
	auto& gfx::Vertex::getAttribute(){
		char* attributeData = data + layout.resolve(type).getOffset();

		if constexpr(type == VertexElementType::position2D){
			return *reinterpret_cast<math::Vector2f*>(attributeData);
		} else if constexpr(type == VertexElementType::position3D){
			return *reinterpret_cast<math::Vector3f*>(attributeData);
		} else if constexpr(type == VertexElementType::texture2D){
			return *reinterpret_cast<math::Vector2f*>(attributeData);
		} else if constexpr(type == VertexElementType::normal){
			return *reinterpret_cast<math::Vector3f*>(attributeData);
		} else{
			CLV_ASSERT(false, "Unable to resolve element type");
			return *reinterpret_cast<char*>(attributeData);
		}
	}

	template<typename DestDataType, typename SourceDataType>
	void Vertex::setAttribute(char* attribute, SourceDataType&& value){
		if constexpr(std::is_assignable_v<DestDataType, SourceDataType>){
			*reinterpret_cast<DestDataType*>(attribute) = value;
		} else{
			CLV_ASSERT(false, "Types are not assignable");
		}
	}

	template<typename First, typename ...Rest>
	void Vertex::setAttributeByIndex(size_t i, First&& first, Rest&& ...rest){
		setAttributeByIndex(i, std::forward<First>(first));
		setAttributeByIndex(i + 1, std::forward<Rest>(rest)...);
	}

	template<typename T>
	void Vertex::setAttributeByIndex(size_t i, T&& val){
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
				setAttribute<math::Vector3f>(attribute, std::forward<T>(val));
				break;

			default:
				CLV_ASSERT(false, "Element type is not supported");
				break;
		}
	}

	template<typename ...Args>
	void VertexBufferData::emplaceBack(Args&& ...args){
		buffer.resize(buffer.size() + layout.size());
		back().setAttributeByIndex(0u, std::forward<Args>(args)...);
	}
}