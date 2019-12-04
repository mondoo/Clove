#pragma once

#include <Clove/Core/Layer.hpp>

#pragma managed(push, off)
typedef void(*AttachmentFunctionPointer)();
typedef void(*UpdateFunctionPointer)(float);
#pragma managed(pop)

namespace clv::blb{
	class NativeLayer : public Layer{
		//VARIABLES
	public:
		AttachmentFunctionPointer	onAttachDelegate;
		UpdateFunctionPointer		onUpdateDelegate;
		AttachmentFunctionPointer	onDetachDelegate;

		//FUNCTIONS
	public:
		//TODO: Ctors

		virtual void onAttach() override;
		virtual void onUpdate(utl::DeltaTime deltaTime) override;
		virtual void onDetach() override;
	};
}

namespace Clove{
	public ref class Layer abstract{
		delegate void AttachmentDelegate();
		delegate void UpdateDelegate(float);

		//VARIABLES
	private:
		std::shared_ptr<clv::blb::NativeLayer>* nativeLayer = nullptr;

		AttachmentDelegate^ onAttachDelegate;
		UpdateDelegate^		onUpdateDelegate;
		AttachmentDelegate^ onDetachDelegate;

		//FUNCTIONS
	public:
		Layer();
		~Layer();

		const std::shared_ptr<clv::blb::NativeLayer>& getNativeLayer();

	protected:
		virtual void onAttach() abstract;
		virtual void onUpdate(float deltaTime) abstract;
		virtual void onDetach() abstract;
	};
}