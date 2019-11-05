#include "RenderDevice.hpp"

namespace clv::gfx{
	API RenderDevice::api = API::None;

	RenderDevice::RenderDevice(RenderDevice&& other) noexcept = default;
	
	RenderDevice& RenderDevice::operator=(RenderDevice&& other) noexcept = default;

	RenderDevice::~RenderDevice() = default;
	
	RenderDevice::RenderDevice(API apiType){
		api = apiType;
	}
	
	API RenderDevice::getAPIType(){
		return api;
	}
}