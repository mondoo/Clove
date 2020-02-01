#pragma once

namespace clv::utl{
	struct MultiCastDelegateHandle{
		//VARIABLES
	public:
		const std::optional<int32_t> ID = {};

		//FUNCTIONS
	public:
		MultiCastDelegateHandle() = default;
		MultiCastDelegateHandle(int32_t ID) : ID(ID){}

		operator int() const noexcept{ return ID.value_or(-1); }
	};
}

namespace std{
	template<>
	struct hash<clv::utl::MultiCastDelegateHandle>{
		std::size_t operator()(const clv::utl::MultiCastDelegateHandle& handle) const noexcept{
			return hash<int>()(handle);
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

		int32_t nextID = 0;

		//FUNCTIONS
	public:
		MultiCastDelegate();

		MultiCastDelegate(const MultiCastDelegate& other) = delete;
		MultiCastDelegate(MultiCastDelegate&& other) noexcept;

		MultiCastDelegate& operator=(const MultiCastDelegate& other) = delete;
		MultiCastDelegate& operator=(MultiCastDelegate&& other) noexcept;

		~MultiCastDelegate();

		template<typename BindFunctionPrototype, typename ObjectType>
		MultiCastDelegateHandle bind(BindFunctionPrototype&& function, ObjectType* object);
		template<typename BindFunctionPrototype>
		MultiCastDelegateHandle bind(BindFunctionPrototype&& function);

		void unbind(const MultiCastDelegateHandle& handle);
		void unbindAll();

		template<typename ...Args>
		void broadcast(Args&& ...args) const;
	};
}

#include "Delegate.inl"