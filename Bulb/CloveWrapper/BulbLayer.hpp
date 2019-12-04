#pragma once

#include <Clove/Core/Layer.hpp>

#pragma managed(push, off)
typedef void(*AttachmentFunctionPointer)();
typedef void(*UpdateFunctionPointer)(float);
#pragma managed(pop)

namespace clv::blb{
	class BulbNativeLayer : public Layer{
		//VARIABLES
	public:
		AttachmentFunctionPointer	onAttachDelegate;
		UpdateFunctionPointer		onUpdateDelegate;
		AttachmentFunctionPointer	onDetachDelegate;

		//delegate void test();

		//FUNCTIONS
	public:
		//TODO: Ctors

		virtual void onAttach() override;
		virtual void onUpdate(utl::DeltaTime deltaTime) override;
		virtual void onDetach() override;
	};
}

namespace Bulb::CloveWrapper{
	public ref class BulbLayer abstract{
		delegate void AttachmentDelegate();
		delegate void UpdateDelegate(float);

		//VARIABLES
	private:
		std::shared_ptr<clv::blb::BulbNativeLayer>* nativeLayer = nullptr;

		AttachmentDelegate^ onAttachDelegate;
		UpdateDelegate^		onUpdateDelegate;
		AttachmentDelegate^ onDetachDelegate;

		//FUNCTIONS
	public:
		BulbLayer();
		~BulbLayer();

		const std::shared_ptr<clv::blb::BulbNativeLayer>& getNativeLayer();

	protected:
		virtual void onAttach() abstract;
		virtual void onUpdate(float deltaTime) abstract;
		virtual void onDetach() abstract;
	};
}