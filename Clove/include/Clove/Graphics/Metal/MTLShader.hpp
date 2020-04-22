#include "Clove/Graphics/Shader.hpp"

#import <MetalKit/MetalKit.h>

namespace clv::gfx::mtl{
	class MTLShader : public Shader{
		//VARIABLES
	private:
		std::shared_ptr<GraphicsFactory> factory;

		ShaderDescriptor descriptor;
		
		id<MTLFunction> shader;
		
		//FUNCTIONS
	public:
		MTLShader() = delete;
		MTLShader(std::shared_ptr<GraphicsFactory> factory, id<MTLDevice> mtlDevice, ShaderDescriptor descriptor, std::string_view pathToShader);
		MTLShader(std::shared_ptr<GraphicsFactory> factory, id<MTLDevice> mtlDevice, ShaderDescriptor descriptor, const char* bytes, const std::size_t size);
		
		MTLShader(const MTLShader& other) = delete;
		MTLShader(MTLShader&& other) noexcept;
		
		MTLShader& operator=(const MTLShader& other) = delete;
		MTLShader& operator=(MTLShader&& other) noexcept;
		
		~MTLShader();

		const std::shared_ptr<GraphicsFactory>& getFactory() const override;
		
		const ShaderDescriptor& getDescriptor() const override;
		
		const id<MTLFunction> getMTLShader() const;
	};
}
