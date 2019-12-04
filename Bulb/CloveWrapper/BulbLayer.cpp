#include "BulbLayer.hpp"

namespace clv::blb{
	void BulbNativeLayer::onAttach(){
		onAttachDelegate();
	}

	void BulbNativeLayer::onUpdate(utl::DeltaTime deltaTime){
		onUpdateDelegate(deltaTime.getDeltaSeconds());
	}

	void BulbNativeLayer::onDetach(){
		onDetachDelegate();
	}
}

namespace Bulb::CloveWrapper{
	BulbLayer::BulbLayer(){
		onAttachDelegate = gcnew AttachmentDelegate(this, &BulbLayer::onAttach);
		onUpdateDelegate = gcnew UpdateDelegate(this, &BulbLayer::onUpdate);
		onDetachDelegate = gcnew AttachmentDelegate(this, &BulbLayer::onDetach);

		System::IntPtr attachPointer = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(onAttachDelegate);
		System::IntPtr updatePointer = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(onUpdateDelegate);
		System::IntPtr detachPointer = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(onDetachDelegate);

		nativeLayer = new std::shared_ptr<clv::blb::BulbNativeLayer>();
		*nativeLayer = std::make_shared<clv::blb::BulbNativeLayer>();

		(*nativeLayer)->onAttachDelegate = static_cast<AttachmentFunctionPointer>(attachPointer.ToPointer());
		(*nativeLayer)->onUpdateDelegate = static_cast<UpdateFunctionPointer>(updatePointer.ToPointer());
		(*nativeLayer)->onDetachDelegate = static_cast<AttachmentFunctionPointer>(detachPointer.ToPointer());
	}

	BulbLayer::~BulbLayer(){
		delete nativeLayer;
	}
	
	const std::shared_ptr<clv::blb::BulbNativeLayer>& BulbLayer::getNativeLayer(){
		return *nativeLayer;
	}
}