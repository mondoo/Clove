#pragma once

#include "Clove/Delegate/DelegateHandle.hpp"

namespace clv {
	template<typename FunctionPrototype>
	class MultiCastDelegate {
		//VARIABLES
	private:
		inline static DelegateHandle::IdType nextId = 0;
		std::unordered_map<DelegateHandle::IdType, std::function<FunctionPrototype>> functionPointers;

		std::shared_ptr<DelegateHandle::Proxy> handleProxy;

		//FUNCTIONS
	public:
		MultiCastDelegate();

		MultiCastDelegate(const MultiCastDelegate& other) = delete;
		MultiCastDelegate(MultiCastDelegate&& other) noexcept;

		MultiCastDelegate& operator=(const MultiCastDelegate& other) = delete;
		MultiCastDelegate& operator=(MultiCastDelegate&& other) noexcept;

		~MultiCastDelegate();

		template<typename RetType, typename ObjectType, typename... Args>
		[[nodiscard]] DelegateHandle bind(RetType (ObjectType::*function)(Args...), ObjectType* object);
		template<typename BindFunctionPrototype>
		[[nodiscard]] DelegateHandle bind(BindFunctionPrototype&& function);

		void unbind(DelegateHandle::IdType id);
		void unbindAll();

		template<typename... Args>
		void broadcast(Args&&... args) const;
	};
}

#include "MultiCastDelegate.inl"