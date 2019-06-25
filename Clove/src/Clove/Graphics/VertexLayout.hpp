#pragma once

namespace clv::gfx{
	enum class VertexElementType{
		position2D,
		position3D,
		texture2D,
		normal,
	};

	template<VertexElementType> struct VertexElementData;
	template<> struct VertexElementData<VertexElementType::position2D>{
		using DataType = math::Vector2f;
		static constexpr unsigned int elementCount = 2u;
		static constexpr char* semantic = "Position";
	};
	template<> struct VertexElementData<VertexElementType::position3D>{
		using DataType = math::Vector3f;
		static constexpr unsigned int elementCount = 3u;
		static constexpr char* semantic = "Position";
	};
	template<> struct VertexElementData<VertexElementType::texture2D>{
		using DataType = math::Vector2f;
		static constexpr unsigned int elementCount = 2u;
		static constexpr char* semantic = "TexCoord";
	};
	template<> struct VertexElementData<VertexElementType::normal>{
		using DataType = math::Vector3f;
		static constexpr unsigned int elementCount = 3u;
		static constexpr char* semantic = "Normal";
	};

	class VertexElement{
		//VARIABLES
	private:
		VertexElementType type;
		size_t offset;

		//FUNCTIONS
	public:
		VertexElement() = delete;
		VertexElement(const VertexElement& other);
		VertexElement(VertexElement&& other) noexcept;
		VertexElement& operator=(const VertexElement& other);
		VertexElement& operator=(VertexElement&& other) noexcept;
		~VertexElement();

		VertexElement(VertexElementType type, size_t offset);

		size_t getOffset() const;
		size_t getOffsetAfter() const;

		size_t getSize() const;

		unsigned int getCount() const;

		VertexElementType getType() const;

		static constexpr size_t sizeOf(VertexElementType type);
		static constexpr unsigned int countOf(VertexElementType type);
	};

	class VertexLayout{
		//VARIABLES
	private:
		std::vector<VertexElement> elements;

		//FUNCTIONS
	public:
		VertexLayout();
		VertexLayout(const VertexLayout& other);
		VertexLayout(VertexLayout&& other) noexcept;
		VertexLayout& operator=(const VertexLayout& other);
		VertexLayout& operator=(VertexLayout&& other) noexcept;
		~VertexLayout();

		VertexLayout& add(VertexElementType type);

		size_t size() const;
		size_t count() const;

		const VertexElement& resolve(VertexElementType type) const;
		const VertexElement& resolve(size_t i) const;
	};

	class Vertex{
		friend class VertexBufferData;

		//VARIABLES
	private:
		char* data = nullptr;
		const VertexLayout& layout;

		//FUNCTIONS
	public:
		Vertex() = delete;
		Vertex(const Vertex& other) = delete;
		Vertex(Vertex&& other) noexcept = delete;
		Vertex& operator=(const Vertex& other) = delete;
		Vertex& operator=(Vertex&& other) noexcept = delete;
		~Vertex();

		template<VertexElementType Type>
		auto& getAttribute();

		//TODO: Set attribute?

	private:
		Vertex(char* data, const VertexLayout& layout);

		template<VertexElementType DestType, typename SourceDataType>
		void setAttribute(char* attribute, SourceDataType&& value);

		template<typename First, typename ...Rest>
		void setAttributeByIndex(size_t i, First&& first, Rest&& ... rest);

		template<typename T>
		void setAttributeByIndex(size_t i, T&& val);
	};

	class VertexBufferData{
		//VARIABLES
	private:
		std::vector<char> buffer;
		VertexLayout layout;

		//FUNCTIONS
	public:
		VertexBufferData() = delete;
		VertexBufferData(const VertexBufferData& other);
		VertexBufferData(VertexBufferData&& other) noexcept;
		VertexBufferData& operator=(const VertexBufferData& other);
		VertexBufferData& operator=(VertexBufferData&& other) noexcept;
		~VertexBufferData();

		VertexBufferData(VertexLayout layout);

		//TODO: reserve?

		template<typename ...Args>
		void emplaceBack(Args&&... args);

		Vertex front();
		Vertex back();

		Vertex operator[](size_t i);

		const char* data() const;

		size_t size() const;
		size_t sizeBytes() const;

		const VertexLayout& getLayout() const;
	};
}

#include "VertexLayout.inl"