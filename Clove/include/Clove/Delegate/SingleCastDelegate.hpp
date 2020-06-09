#pragma once

namespace clv {
	template<typename FunctionPrototype>
	class SingleCastDelegate {
		//VARIABLES
	private:
		std::function<FunctionPrototype> functionPointer;

		//FUNCTIONS
	public:
		SingleCastDelegate();

		SingleCastDelegate(const SingleCastDelegate& other) = delete;
		SingleCastDelegate(SingleCastDelegate&& other) noexcept;

		SingleCastDelegate& operator=(const SingleCastDelegate& other) = delete;
		SingleCastDelegate& operator								   =(SingleCastDelegate&& other) noexcept;

		~SingleCastDelegate();

		template<typename RetType, typename ObjectType, typename... Args>
		void bind(RetType (ObjectType::*function)(Args...), ObjectType* object);
		template<typename BindFunctionPrototype>
		void bind(BindFunctionPrototype&& function);

		void unbind();

		bool isBound() const;
		operator bool() const;

		template<typename... Args>
		auto broadcast(Args&&... args) const;
	};
}

#include "SingleCastDelegate.inl"