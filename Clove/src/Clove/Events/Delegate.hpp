#pragma once

#include <optional>
#include <map>

namespace clv::evt{
	namespace utility{
		template<typename RetType, typename ObjectType, typename ...Args>
		constexpr auto getArgumentCount(RetType(ObjectType::*)(Args...)){
			return std::integral_constant<size_t, sizeof ...(Args)>{};
		}
	}

	template<typename FunctionPrototype>
	class SingleCastDelegate{
		//VARIABLES
	private:
		std::function<FunctionPrototype> functionPointer;

		//FUNCTIONS
	public:
		template<typename BindFunctionPrototype, typename ...Args>
		void bindMemberFunction(BindFunctionPrototype&& function, Args&& ...args);
		template<typename BindFunctionPrototype>
		void bindLambda(BindFunctionPrototype&& function);

		void unbind();

		template<typename ...Args>
		auto broadcast(Args&& ...args) const;
	};

	struct MultiCastDelegateHandle{
		//VARIABLES
	public:
		const std::optional<int> ID = {};

		//FUNCTIONS
	public:
		MultiCastDelegateHandle() = default;
		MultiCastDelegateHandle(int ID) : ID(ID){}

		operator int() const{ return ID.value_or(-1); }

		bool operator <(const MultiCastDelegateHandle& rhs) const{ return ID.value_or(-1) < rhs.ID.value_or(-1); }
	};

	template<typename FunctionPrototype>
	class MultiCastDelegate{
		//VARIABLES
	private:
		std::map<MultiCastDelegateHandle, std::function<FunctionPrototype>> functionPointers;

		int nextID = 0;

		//FUNCTIONS
	public:
		template<typename BindFunctionPrototype, typename ...Args>
		MultiCastDelegateHandle bindMemberFunction(BindFunctionPrototype&& function, Args&& ...args);
		template<typename BindFunctionPrototype>
		MultiCastDelegateHandle bindLambda(BindFunctionPrototype&& function);

		void unbind(const MultiCastDelegateHandle& handle);
		void unbindAll();

		template<typename ...Args>
		void broadcast(Args&& ...args) const;
	};
}

#include "Delegate.inl"