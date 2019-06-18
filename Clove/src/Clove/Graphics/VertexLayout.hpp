#pragma once

namespace clv::gfx{
	/*enum class BufferElementFormat{
		FLOAT_2,
		FLOAT_3
	};*/

	enum class BufferElementFormat{
		POS_2D,
		POS_3D,
		TEX_2D,
		NOR,
	};

	class VertexLayout/* : public Bindable*/{
		//FUNCTIONS
	public:
		VertexLayout();
		VertexLayout(const VertexLayout& other);
		VertexLayout(VertexLayout&& other) noexcept;
		VertexLayout& operator=(const VertexLayout& other);
		VertexLayout& operator=(VertexLayout&& other) noexcept;
		~VertexLayout();

		//virtual void pushElement(const std::string& elementName, BufferElementFormat elementFormat) = 0;
		//virtual void createLayout(Bindable& bindable) = 0;
		//virtual void resetLayout() = 0;
	};
}