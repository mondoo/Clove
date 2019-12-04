#include "Layer.hpp"

namespace clv::blb{
	void NativeLayer::onAttach(){
		onAttachDelegate();
	}

	void NativeLayer::onUpdate(utl::DeltaTime deltaTime){
		onUpdateDelegate(deltaTime.getDeltaSeconds());
	}

	void NativeLayer::onDetach(){
		onDetachDelegate();
	}
}

namespace Clove{
	Layer::Layer(){
		onAttachDelegate = gcnew AttachmentDelegate(this, &Layer::onAttach);
		onUpdateDelegate = gcnew UpdateDelegate(this, &Layer::onUpdate);
		onDetachDelegate = gcnew AttachmentDelegate(this, &Layer::onDetach);

		System::IntPtr attachPointer = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(onAttachDelegate);
		System::IntPtr updatePointer = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(onUpdateDelegate);
		System::IntPtr detachPointer = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(onDetachDelegate);

		nativeLayer = new std::shared_ptr<clv::blb::NativeLayer>();
		*nativeLayer = std::make_shared<clv::blb::NativeLayer>();

		(*nativeLayer)->onAttachDelegate = static_cast<AttachmentFunctionPointer>(attachPointer.ToPointer());
		(*nativeLayer)->onUpdateDelegate = static_cast<UpdateFunctionPointer>(updatePointer.ToPointer());
		(*nativeLayer)->onDetachDelegate = static_cast<AttachmentFunctionPointer>(detachPointer.ToPointer());
	}

	Layer::~Layer(){
		delete nativeLayer;
	}
	
	const std::shared_ptr<clv::blb::NativeLayer>& Layer::getNativeLayer(){
		return *nativeLayer;
	}
}