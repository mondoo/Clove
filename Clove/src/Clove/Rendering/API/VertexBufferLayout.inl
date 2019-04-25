namespace clv{
	inline unsigned int VertexBufferElement::getSizeOfType(unsigned int type){
		switch(type){
			case GL_FLOAT:
				return 4;
			case GL_UNSIGNED_INT:
				return 4;
			case GL_UNSIGNED_BYTE:
				return 1;
			default:
				CLV_ASSERT(false, "Unsupported type in {0}", __FUNCTION__);
				return 0;
		}
	}

	template<typename T>
	void VertexBufferLayout::push(unsigned int count){
		CLV_ASSERT(false, "Unsupported type for {0}", __FUNCTION__);
	}

	template <>
	void VertexBufferLayout::push<float>(unsigned int count){
		elements.push_back({ GL_FLOAT, count, GL_FALSE });
		stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
	}

	template <>
	void VertexBufferLayout::push<unsigned int>(unsigned int count){
		elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
	}

	template <>
	void VertexBufferLayout::push<unsigned char>(unsigned int count){
		elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement>& VertexBufferLayout::getElements() const{
		return elements;
	}

	inline unsigned int VertexBufferLayout::getStride() const{
		return stride;
	}
}