#pragma once

namespace clv::utl{
	struct MultiCastDelegateHandle{
		//VARIABLES
	public:
		using IdType = int32_t;

	private:
		const std::optional<IdType> id = {};
		static constexpr IdType INVALID_ID = -1;

		//FUNCTIONS
	public:
		MultiCastDelegateHandle() = default;
		MultiCastDelegateHandle(IdType id) : id(id) {}

		operator IdType() const noexcept { return id.value_or(INVALID_ID); }
	};
}

namespace std{
	template<>
	struct hash<clv::utl::MultiCastDelegateHandle>{
		std::size_t operator()(const clv::utl::MultiCastDelegateHandle& handle) const noexcept{
			return hash<clv::utl::MultiCastDelegateHandle::IdType>()(handle);
		}
	};
}

namespace clv::utl{
	template<typename FunctionPrototype>
	class SingleCastDelegate{
		//VARIABLES
	private:
		std::function<FunctionPrototype> functionPointer;

		//FUNCTIONS
	public:
		SingleCastDelegate();

		SingleCastDelegate(const SingleCastDelegate& other) = delete;
		SingleCastDelegate(SingleCastDelegate&& other) noexcept;

		SingleCastDelegate& operator=(const SingleCastDelegate& other) = delete;
		SingleCastDelegate& operator=(SingleCastDelegate&& other) noexcept;

		~SingleCastDelegate();

		template<typename BindFunctionPrototype, typename ObjectType>
		void bind(BindFunctionPrototype&& function, ObjectType* object);
		template<typename BindFunctionPrototype>
		void bind(BindFunctionPrototype&& function);

		void unbind();

		bool isBound() const;
		operator bool() const;

		template<typename ...Args>
		auto broadcast(Args&& ...args) const;
	};

	template<typename FunctionPrototype>
	class MultiCastDelegate{
		//VARIABLES
	private:
		std::unordered_map<MultiCastDelegateHandle, std::function<FunctionPrototype>> functionPointers;

		inline static MultiCastDelegateHandle::IdType nextId = 0;

		//FUNCTIONS
	public:
		MultiCastDelegate();

		MultiCastDelegate(const MultiCastDelegate& other) = delete;
		MultiCastDelegate(MultiCastDelegate&& other) noexcept;

		MultiCastDelegate& operator=(const MultiCastDelegate& other) = delete;
		MultiCastDelegate& operator=(MultiCastDelegate&& other) noexcept;

		~MultiCastDelegate();

		template<typename BindFunctionPrototype, typename ObjectType>
		[[nodiscard]] MultiCastDelegateHandle bind(BindFunctionPrototype&& function, ObjectType* object);
		template<typename BindFunctionPrototype>
		[[nodiscard]] MultiCastDelegateHandle bind(BindFunctionPrototype&& function);

		void unbind(const MultiCastDelegateHandle& handle);
		void unbindAll();

		template<typename ...Args>
		void broadcast(Args&& ...args) const;
	};
}

#include "Delegate.inl"