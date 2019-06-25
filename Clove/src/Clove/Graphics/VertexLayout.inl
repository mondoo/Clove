namespace clv::gfx{
	constexpr size_t VertexElement::sizeOf(VertexElementType type){
		switch(type){
			case VertexElementType::position2D:
				return sizeof(VertexElementData<VertexElementType::position2D>::DataType);
			case VertexElementType::position3D:
				return sizeof(VertexElementData<VertexElementType::position3D>::DataType);
			case VertexElementType::texture2D:
				return sizeof(VertexElementData<VertexElementType::texture2D>::DataType);
			case VertexElementType::normal:
				return sizeof(VertexElementData<VertexElementType::normal>::DataType);
			default:
				CLV_ASSERT(false, "Invalid element type. {0}", __func__);
				return 0u;
		}
	}

	constexpr unsigned int VertexElement::countOf(VertexElementType type){
		switch(type){
			case VertexElementType::position2D:
				return VertexElementData<VertexElementType::position2D>::elementCount;
			case VertexElementType::position3D:
				return VertexElementData<VertexElementType::position3D>::elementCount;
			case VertexElementType::texture2D:
				return VertexElementData<VertexElementType::texture2D>::elementCount;
			case VertexElementType::normal:
				return VertexElementData<VertexElementType::normal>::elementCount;
			default:
				CLV_ASSERT(false, "Invalid element type. {0}", __func__);
				return 0u;
		}
	}

	constexpr const char* VertexElement::semanticOf(VertexElementType type){
		switch(type){
			case VertexElementType::position2D:
				return VertexElementData<VertexElementType::position2D>::semantic;
			case VertexElementType::position3D:
				return VertexElementData<VertexElementType::position3D>::semantic;
			case VertexElementType::texture2D:
				return VertexElementData<VertexElementType::texture2D>::semantic;
			case VertexElementType::normal:
				return VertexElementData<VertexElementType::normal>::semantic;
			default:
				CLV_ASSERT(false, "Invalid element type. {0}", __func__);
				return nullptr;
		}
	}

	template<VertexElementType Type>
	auto& gfx::Vertex::getAttribute(){
		char* attributeData = data + layout.resolve(Type).getOffset();
		return *reinterpret_cast<VertexElementData<Type>::DataType*>(attributeData);
	}

	template<VertexElementType DestType, typename SourceDataType>
	void Vertex::setAttribute(char* attribute, SourceDataType&& value){
		using DestDataType = typename VertexElementData<DestType>::DataType;
		if constexpr(std::is_assignable_v<DestDataType, SourceDataType>){
			*reinterpret_cast<DestDataType*>(attribute) = value;
		} else{
			CLV_ASSERT(false, "Types are not assignable. {0}", __func__);
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
				setAttribute<VertexElementType::position2D>(attribute, std::forward<T>(val));
				break;

			case VertexElementType::position3D:
				setAttribute<VertexElementType::position3D>(attribute, std::forward<T>(val));
				break;

			case VertexElementType::texture2D:
				setAttribute<VertexElementType::texture2D>(attribute, std::forward<T>(val));
				break;

			case VertexElementType::normal:
				setAttribute<VertexElementType::normal>(attribute, std::forward<T>(val));
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