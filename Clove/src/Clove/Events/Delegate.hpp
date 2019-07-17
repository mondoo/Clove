#pragma once

//TODO: .inl stuff

namespace clv::evt{//Maybe move to utils?
	template<typename FunctionPrototype>
	class Delegate{
		struct InvokerBase{
			virtual void invoke() = 0;

			/*InvokerBase(InvokerBase&& other) noexcept = default;
			InvokerBase& operator=(InvokerBase&& other) noexcept = default;*/

			void operator()(){
				invoke();
			}
		};

		template<typename T>
		struct Invoker : public InvokerBase{
			T* context = nullptr;
			FunctionPrototype function;

			/*Invoker(T* inContext, FunctionPrototype inFunction)
				: context(inContext)
				, function(inFunction){
			}*/
			/*Invoker(const Invoker& other) = default;
			Invoker(Invoker&& other) noexcept = default;
			Invoker& operator=(const Invoker& other) = default;
			Invoker& operator=(Invoker&& other) noexcept = default;*/

			virtual void invoke() override{
				(context->*function)();
			}
		};

		//VARIABLES
	private:
		//FunctionPrototype* function = nullptr;

		//std::unique_ptr<int> invoker; //doesn't work for some weird reason
		InvokerBase* invoker = nullptr;

		//FUNCTIONS
	public:
		//TODO: Constructors and stuff

		~Delegate() = default; //Note we need to delete invoker or we'll leak

		void bind(FunctionPrototype function){
			this->function = function;
		}

		template<typename T>
		void bind(T* context, FunctionPrototype function){
			//invoker = std::make_unique<Invoker<T>>(context, function);
			Invoker<T>* inv = new Invoker<T>();
			inv->context = context;
			inv->function = function;
			invoker = inv;
		}

		void broadcast(){
			invoker->invoke();
		}
	};
}

#include "Delegate.inl"