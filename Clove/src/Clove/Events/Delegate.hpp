#pragma once

//TODO: .inl stuff

namespace clv::evt{//Maybe move to utils?
	template<typename Function>
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
			FunctionPrototype function = nullptr;

			Invoker(T* inContext, FunctionPrototype inFunction)
				: context(inContext)
				, function(inFunction){
			}

			virtual void invoke() override{
				if(context){
					(context->*function)();
				}/* else{
					(*function);
				}*/
			}
		};

		template<typename FunctionPrototype>
		struct InvokerB : public InvokerBase{
			FunctionPrototype function = nullptr;

			InvokerB(FunctionPrototype inFunction)
				: function(inFunction){
			}

			virtual void invoke() override{
				function();
			}
		};

		//VARIABLES
	private:
		//FunctionPrototype* function = nullptr;

		std::unique_ptr<InvokerBase> invoker;
		//InvokerBase* invoker = nullptr;
		
		//std::function<FunctionPrototype> func;


		//FUNCTIONS
	public:
		//TODO: Constructors and stuff
		Delegate() = default;
		Delegate(const Delegate& other) = delete; 
		Delegate(Delegate&& other) noexcept = default;
		Delegate& operator=(const Delegate& other) = delete;
		Delegate& operator=(Delegate&& other) noexcept = default;
		~Delegate() = default; //Note we need to delete invoker or we'll leak

		void bind(Function function){
			//func = std::bind(function);
			invoker = std::make_unique<InvokerB<Function>>(function);
		}

		template<typename T>
		void bind(T* context, Function function){
			//func = std::bind(function, context);
			invoker = std::make_unique<Invoker<T, Function>>(context, function);
		}

		/*template<typename T, typename FunctionPrototypeA, typename FunProt>
		friend Delegate<FunProt> bind(T* context, FunctionPrototypeA function);*/

		void broadcast(){
			invoker->invoke();
			//func();
		}

		//TODO: Allow for perfect forwarding
		/*template<typename ...Args>
		void broadcast(Args&& ...args){
			func(std::forward<Args>(args)...);
		}*/
	};

	//Friend function def outside of class
	/*template<typename FunProt, typename T, typename FunctionPrototypeA>
	Delegate<FunProt> bind(T* context, FunctionPrototypeA function){
		Delegate<FunProt> del{};
		del.invoker = std::make_unique<clv::evt::Delegate<FunProt>::Invoker<T, FunctionPrototypeA>>(context, function);
		return del;
	}*/
}

#include "Delegate.inl"