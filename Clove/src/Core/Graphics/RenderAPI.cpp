#include "RenderAPI.hpp"

namespace clv::gfx{
	API RenderAPI::api = API::None;

	RenderAPI::RenderAPI(RenderAPI&& other) noexcept = default;
	
	RenderAPI& RenderAPI::operator=(RenderAPI&& other) noexcept = default;

	RenderAPI::~RenderAPI() = default;
	
	RenderAPI::RenderAPI(API apiType){
		api = apiType;
	}
	
	API RenderAPI::getAPIType(){
		return api;
	}
}