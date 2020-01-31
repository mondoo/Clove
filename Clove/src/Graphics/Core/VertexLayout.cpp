#include "Clove/Graphics/Core/VertexLayout.hpp"

namespace clv::gfx{
	VertexElement::VertexElement(const VertexElement& other) = default;

	VertexElement::VertexElement(VertexElement&& other) noexcept = default;

	VertexElement& VertexElement::operator=(const VertexElement& other) = default;

	VertexElement& VertexElement::operator=(VertexElement&& other) noexcept = default;

	VertexElement::~VertexElement() = default;

	VertexElement::VertexElement(VertexElementType type, size_t offset)
		: type(type)
		, offset(offset){
	}

	size_t VertexElement::getOffset() const{
		return offset;
	}
	
	size_t VertexElement::getOffsetAfter() const{
		return getOffset() + getSize();
	}
	
	size_t VertexElement::getSize() const{
		return sizeOf(type);
	}
	
	uint32 VertexElement::getCount() const{
		return countOf(type);
	}

	const char* VertexElement::getSemantic() const{
		return semanticOf(type);
	}
	
	VertexElementType VertexElement::getType() const{
		return type;
	}

	VertexElementType VertexElement::getTypeFromSemantic(const std::string& semantic){
		if(VertexElementData<VertexElementType::position2D>::semantic == semantic){
			return VertexElementType::position2D;
		} else if(VertexElementData<VertexElementType::position3D>::semantic == semantic){
			return VertexElementType::position3D;
		} else if(VertexElementData<VertexElementType::texture2D>::semantic == semantic){
			return VertexElementType::texture2D;
		} else if(VertexElementData<VertexElementType::normal>::semantic == semantic){
			return VertexElementType::normal;
		}

		CLV_ASSERT(false, "{0} could not find proper element type", CLV_FUNCTION_NAME);
		return VertexElementType::position2D;
	}

	VertexLayout::VertexLayout() = default;

	VertexLayout::VertexLayout(const VertexLayout& other) = default;

	VertexLayout::VertexLayout(VertexLayout&& other) noexcept = default;

	VertexLayout& VertexLayout::operator=(const VertexLayout& other) = default;

	VertexLayout& VertexLayout::operator=(VertexLayout&& other) noexcept = default;

	VertexLayout::~VertexLayout() = default;

	VertexLayout& VertexLayout::add(VertexElementType type){
		elements.emplace_back(type, size());
		return *this;
	}
	
	size_t VertexLayout::size() const{
		return elements.empty() ? 0u : elements.back().getOffsetAfter();
	}

	size_t VertexLayout::count() const{
		return elements.size();
	}

	void VertexLayout::clear(){
		elements.clear();
	}
	
	const VertexElement& VertexLayout::resolve(VertexElementType type) const{
		for(auto& element : elements){
			if(element.getType() == type){
				return element;
			}
		}
		CLV_ASSERT(false, "Could not find element of type. {0}", CLV_FUNCTION_NAME);
		return elements.front();
	}

	const VertexElement& VertexLayout::resolve(size_t i) const{
		return elements[i];
	}

	Vertex::~Vertex() = default;

	Vertex::Vertex(char* data, const VertexLayout& layout)
		: data(data)
		, layout(layout){
		CLV_ASSERT(data != nullptr, "Data is nullptr. {0}", CLV_FUNCTION_NAME);
	}

	VertexBufferData::VertexBufferData(const VertexBufferData& other) = default;

	VertexBufferData::VertexBufferData(VertexBufferData&& other) noexcept = default;

	VertexBufferData& VertexBufferData::operator=(const VertexBufferData& other) = default;

	VertexBufferData& VertexBufferData::operator=(VertexBufferData&& other) noexcept = default;

	VertexBufferData::~VertexBufferData() = default;
	
	VertexBufferData::VertexBufferData(VertexLayout layout)
		: layout(std::move(layout)){
	}

	void VertexBufferData::resize(size_t size){
		buffer.resize(size * layout.size());
	}
	
	Vertex VertexBufferData::front(){
		return { buffer.data(), layout };
	}
	
	Vertex VertexBufferData::back(){
		return { (buffer.data() + buffer.size()) - layout.size(), layout };
	}
	
	Vertex VertexBufferData::operator[](size_t i){
		return { buffer.data() + (layout.size() * i), layout };
	}
	
	const char* VertexBufferData::data() const{
		return buffer.data();
	}
	
	size_t VertexBufferData::size() const{
		return buffer.size() / layout.size();
	}
	
	size_t VertexBufferData::sizeBytes() const{
		return buffer.size();
	}
	
	const VertexLayout& VertexBufferData::getLayout() const{
		return layout;
	}
}