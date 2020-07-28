#include <Root/Definitions.hpp>

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
            case VertexElementType::colour3D:
                return sizeof(VertexElementData<VertexElementType::colour3D>::DataType);
            case VertexElementType::jointIds:
                return sizeof(VertexElementData<VertexElementType::jointIds>::DataType);
            case VertexElementType::weights:
                return sizeof(VertexElementData<VertexElementType::weights>::DataType);
			default:
				GARLIC_ASSERT(false, "Invalid element type. {0}", GARLIC_FUNCTION_NAME);
				return 0u;
		}
	}

	constexpr uint32_t VertexElement::countOf(VertexElementType type){
		switch(type){
			case VertexElementType::position2D:
				return VertexElementData<VertexElementType::position2D>::elementCount;
			case VertexElementType::position3D:
				return VertexElementData<VertexElementType::position3D>::elementCount;
			case VertexElementType::texture2D:
				return VertexElementData<VertexElementType::texture2D>::elementCount;
			case VertexElementType::normal:
				return VertexElementData<VertexElementType::normal>::elementCount;
			case VertexElementType::colour3D:
				return VertexElementData<VertexElementType::colour3D>::elementCount;
            case VertexElementType::jointIds:
                return VertexElementData<VertexElementType::jointIds>::elementCount;
            case VertexElementType::weights:
                return VertexElementData<VertexElementType::weights>::elementCount;
			default:
				GARLIC_ASSERT(false, "Invalid element type. {0}", GARLIC_FUNCTION_NAME);
				return 0u;
		}
	}

	constexpr std::string_view VertexElement::semanticOf(VertexElementType type){
		switch(type){
			case VertexElementType::position2D:
				return VertexElementData<VertexElementType::position2D>::semantic;
			case VertexElementType::position3D:
				return VertexElementData<VertexElementType::position3D>::semantic;
			case VertexElementType::texture2D:
				return VertexElementData<VertexElementType::texture2D>::semantic;
			case VertexElementType::normal:
				return VertexElementData<VertexElementType::normal>::semantic;
			case VertexElementType::colour3D:
				return VertexElementData<VertexElementType::colour3D>::semantic;
            case VertexElementType::jointIds:
                return VertexElementData<VertexElementType::jointIds>::semantic;
            case VertexElementType::weights:
                return VertexElementData<VertexElementType::weights>::semantic;
			default:
				GARLIC_ASSERT(false, "Invalid element type. {0}", GARLIC_FUNCTION_NAME);
				return nullptr;
		}
	}

	template<VertexElementType type>
	auto& gfx::Vertex::getAttribute(){
		char* attributeData = data + layout.resolve(type).getOffset();
		return *reinterpret_cast<typename VertexElementData<type>::DataType*>(attributeData);
	}

	template<VertexElementType DestType, typename SourceDataType>
	void Vertex::setAttribute(char* attribute, SourceDataType&& value){
		using DestDataType = typename VertexElementData<DestType>::DataType;
		if constexpr(std::is_assignable_v<DestDataType, SourceDataType>){
			*reinterpret_cast<DestDataType*>(attribute) = value;
		} else{
			GARLIC_ASSERT(false, "Types are not assignable. {0}", GARLIC_FUNCTION_NAME);
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

			case VertexElementType::colour3D:
				setAttribute<VertexElementType::colour3D>(attribute, std::forward<T>(val));
				break;

			default:
				GARLIC_ASSERT(false, "Element type is not supported");
				break;
		}
	}

	template<typename ...Args>
	void VertexBufferData::emplaceBack(Args&& ...args){
		buffer.resize(buffer.size() + layout.size());
		back().setAttributeByIndex(0u, std::forward<Args>(args)...);
	}
}