#pragma once

//TODO: .inl stuff

#include <optional>
#include <map>

namespace clv::evt{//Maybe move to utils?
	template<typename FunctionPrototype>
	class SingleCastDelegate{
		//VARIABLES
	private:
		std::function<FunctionPrototype> functionPointer;

		//FUNCTIONS
	public:
		template<typename BindFunctionPrototype, typename ...Args>
		void bindMemberFunction(BindFunctionPrototype&& function, Args&& ...args){
			functionPointer = std::bind(std::forward<BindFunctionPrototype>(function), std::forward<Args>(args)...);
		}

		template<typename BindFunctionPrototype>
		void bindLambda(BindFunctionPrototype&& function){
			functionPointer = function;
		}

		void unbind(){
			functionPointer = nullptr;
		}

		template<typename ...Args>
		auto broadcast(Args&& ...args){
			if(functionPointer){
				return functionPointer(std::forward<Args>(args)...);
			}
		}
	};

	struct MultiCastDelegateHandle{
		const std::optional<int> ID = {};
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
		MultiCastDelegateHandle bindMemberFunction(BindFunctionPrototype&& function, Args&& ...args){
			auto handle = MultiCastDelegateHandle{ nextID++ };
			auto functionPointer = std::bind(std::forward<BindFunctionPrototype>(function), std::forward<Args>(args)...);
			functionPointers.emplace(handle, functionPointer);
			return handle;
		}

		template<typename BindFunctionPrototype>
		MultiCastDelegateHandle bindLambda(BindFunctionPrototype&& function){
			auto handle = MultiCastDelegateHandle{ nextID++ };
			functionPointers.emplace(handle, function);
			return handle;
		}

		void unbind(const MultiCastDelegateHandle& handle){
			functionPointers.erase(handle);
		}

		void unbindAll(){
			functionPointers.clear();
		}

		template<typename ...Args>
		void broadcast(Args&& ...args){
			for(auto& [handle, function] : functionPointers){
				if(function){
					function(std::forward<Args>(args)...);
				}
			}
		}
	};
}

#include "Delegate.inl"