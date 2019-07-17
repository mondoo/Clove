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

		template<typename T, typename FunctionPrototype>
		struct Invoker : public InvokerBase{
			T* context = nullptr;
			FunctionPrototype function;

			Invoker(T* inContext, FunctionPrototype inFunction)
				: context(inContext)
				, function(inFunction){
			}
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

		std::unique_ptr<InvokerBase> invoker; //doesn't work for some weird reason
		//InvokerBase* invoker = nullptr;
		//std::function<FunctionPrototype> func;


		//FUNCTIONS
	public:
		//TODO: Constructors and stuff
		Delegate() = default;
		Delegate(const Delegate& other) = delete; //Deleting because of unique ptr - will need a custom copy
		Delegate(Delegate&& other) noexcept = default;
		Delegate& operator=(const Delegate& other) = delete;
		Delegate& operator=(Delegate&& other) noexcept = default;
		~Delegate() = default; //Note we need to delete invoker or we'll leak

		//void bind(FunctionPrototype function){
		//	//this->function = function;
		//}

		//template<typename T>
		//void bind(T* context, FunctionPrototype function){
		//	invoker = std::make_unique<Invoker<T>>(context, function);
		//	//Invoker<T>* inv = new Invoker<T>();
		//	//inv->context = context;
		//	//inv->function = function;
		//	//invoker = inv;
		//}

		template<typename T, typename FunctionPrototypeA, typename FunProt>
		friend Delegate<FunProt> bind(T* context, FunctionPrototypeA function);

		void broadcast(){
			invoker->invoke();
		}
	};

	//Friend function def outside of class
	template<typename T, typename FunctionPrototypeA, typename FunProt>
	Delegate<FunProt> bind(T* context, FunctionPrototypeA function){
		Delegate<FunProt> del{};
		del.invoker = std::make_unique<clv::evt::Delegate<FunProt>::Invoker<T, FunctionPrototypeA>>(context, function);
		return del;
	}
}

#include "Delegate.inl"