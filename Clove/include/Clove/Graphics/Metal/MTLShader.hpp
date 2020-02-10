#include "Clove/Graphics/Core/Shader.hpp"

#import <MetalKit/MetalKit.h>

namespace clv::gfx::mtl{
	class MTLShader : public Shader{
		//VARIABLES
	private:
		ShaderDescriptor descriptor;
		
		id<MTLFunction> shader;
		
		//FUNCTIONS
	public:
		MTLShader() = delete;
		MTLShader(id<MTLDevice> mtlDevice, const ShaderDescriptor& descriptor, std::string_view pathToShader);
		
		MTLShader(const MTLShader& other) = delete;
		MTLShader(MTLShader&& other) noexcept;
		
		MTLShader& operator=(const MTLShader& other) = delete;
		MTLShader& operator=(MTLShader&& other) noexcept;
		
		virtual ~MTLShader();
		
		virtual const ShaderDescriptor& getDescriptor() const override;
		
		const id<MTLFunction> getMTLShader() const;
	};
}
