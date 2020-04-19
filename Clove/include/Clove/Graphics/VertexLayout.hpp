#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx {
	template<VertexElementType>
	struct VertexElementData;

	template<>
	struct VertexElementData<VertexElementType::position2D> {
		using DataType = mth::vec2f;
		static constexpr uint32_t elementCount = 2u;
		static constexpr std::string_view semantic = "POSITION2D";
	};
	template<>
	struct VertexElementData<VertexElementType::position3D> {
		using DataType = mth::vec3f;
		static constexpr uint32_t elementCount = 3u;
		static constexpr std::string_view semantic = "POSITION3D";
	};
	template<>
	struct VertexElementData<VertexElementType::texture2D> {
		using DataType = mth::vec2f;
		static constexpr uint32_t elementCount = 2u;
		static constexpr std::string_view semantic = "TEXCOORD";
	};
	template<>
	struct VertexElementData<VertexElementType::normal> {
		using DataType = mth::vec3f;
		static constexpr uint32_t elementCount = 3u;
		static constexpr std::string_view semantic = "NORMAL";
	};
	template<>
	struct VertexElementData<VertexElementType::colour3D> {
		using DataType = mth::vec3f;
		static constexpr uint32_t elementCount = 3u;
		static constexpr std::string_view semantic = "COLOUR3D";
	};

	class VertexElement {
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
		uint32_t getCount() const;
		std::string_view getSemantic() const;

		VertexElementType getType() const;

		static constexpr size_t sizeOf(VertexElementType type);
		static constexpr uint32_t countOf(VertexElementType type);
		static constexpr std::string_view semanticOf(VertexElementType type);
		static VertexElementType getTypeFromSemantic(const std::string& semantic);

		bool operator==(const VertexElement& other) const;
	};

	class VertexLayout {
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

		void clear();

		const VertexElement& resolve(VertexElementType type) const;
		const VertexElement& resolve(size_t i) const;

		bool operator==(const VertexLayout& other) const;
	};

	struct VertexLayoutHasher {
		size_t operator()(const VertexLayout& layout) const {
			size_t seed = layout.count();
			for(size_t i = 0; i < layout.count(); ++i) {
				const VertexElement& element = layout.resolve(i);
				uint32_t typeAsInt = static_cast<uint32_t>(element.getType());

				seed ^= typeAsInt + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			}
			return seed;
		};
	};

	class Vertex {
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

		template<VertexElementType type>
		auto& getAttribute();

	private:
		Vertex(char* data, const VertexLayout& layout);

		template<VertexElementType DestType, typename SourceDataType>
		void setAttribute(char* attribute, SourceDataType&& value);

		template<typename First, typename... Rest>
		void setAttributeByIndex(size_t i, First&& first, Rest&&... rest);

		template<typename T>
		void setAttributeByIndex(size_t i, T&& val);
	};

	class VertexBufferData {
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

		void resize(size_t size);

		template<typename... Args>
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