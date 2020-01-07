#include "Clove/Core/Graphics/Shader.hpp"

namespace clv::gfx::mtl{
	class MTLShader : public Shader{
		//VARIABLES
	private:
		ShaderDescriptor descriptor;
		
		id<MTLFunction> vertexShader;
		id<MTLFunction> pixelShader;
		
		//FUNCTIONS
	public:
		MTLShader() = delete;
		MTLShader(id<MTLDevice> mtlDevice, const ShaderDescriptor& descriptor);
		
		MTLShader(const MTLShader& other) = delete;
		MTLShader(MTLShader&& other) noexcept;
		
		MTLShader& operator=(const MTLShader& other) = delete;
		MTLShader& operator=(MTLShader&& other) noexcept;
		
		virtual ~MTLShader();
		
		const id<MTLFunction> getMTLVertexShader() const;
		const id<MTLFunction> getMTLPixelShader() const;
		
		virtual const ShaderDescriptor& getDescriptor() const override;
		virtual ShaderReflectionData getReflectionData() const override;
		
	private:
		void initialise(id<MTLDevice> mtlDevice, ShaderStyle style);
	};
}
