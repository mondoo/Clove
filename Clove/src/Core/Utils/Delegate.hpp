#pragma once

#include <optional>
#include <map>

namespace clv::utl{
	template<typename FunctionPrototype>
	class SingleCastDelegate{
		//VARIABLES
	private:
		std::function<FunctionPrototype> functionPointer;

		//FUNCTIONS
	public:
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

	struct MultiCastDelegateHandle{
		//VARIABLES
	public:
		const std::optional<int32> ID = {};

		//FUNCTIONS
	public:
		MultiCastDelegateHandle() = default;
		MultiCastDelegateHandle(int32 ID) : ID(ID){}

		operator int() const{ return ID.value_or(-1); }

		bool operator <(const MultiCastDelegateHandle& rhs) const{ return ID.value_or(-1) < rhs.ID.value_or(-1); }
	};

	template<typename FunctionPrototype>
	class MultiCastDelegate{
		//VARIABLES
	private:
		std::map<MultiCastDelegateHandle, std::function<FunctionPrototype>> functionPointers;

		int32 nextID = 0;

		//FUNCTIONS
	public:
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