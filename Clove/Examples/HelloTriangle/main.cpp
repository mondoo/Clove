#include <Clove/Clove.hpp>

//The HelloTriangle example is a basic example on how to render graphics using Clove

using namespace clv;

int main(){
	Log::init();

	//Create the platform instance to make OS calls
	auto platform = plt::createPlatformInstance();

	auto mainWindow = platform->createWindow({"Hello Triangle", 1280, 720});
	mainWindow->setVSync(true);

	auto& graphicsFactory = mainWindow->getGraphicsFactory();

	std::string source_path = SOURCE_DIR;

	//Set up the shaders for the pipeline
	auto vertexShader = graphicsFactory.createShader({ gfx::ShaderStage::Vertex }, source_path + "/VertShader.hlsl");
	auto pixelShader = graphicsFactory.createShader({ gfx::ShaderStage::Pixel }, source_path + "/PixelShader.hlsl");

	//Configure the pipeline state
	auto pipeline = graphicsFactory.createPipelineObject();
	pipeline->setVertexShader(*vertexShader);
	pipeline->setPixelShader(*pixelShader);
	pipeline->setBlendState(true);
	pipeline->setCullMode(gfx::CullFace::Back, true);

	const std::vector<float> vertices = {
		-0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	const std::vector<uint32_t> indices = {
		0, 1, 2
	};

	//Create the buffers to hold the vertices and indices
	gfx::BufferDescriptor vbDesc{};
	vbDesc.elementSize	= sizeof(float) * 5.0f;
	vbDesc.bufferSize	= vertices.size() * sizeof(float);
	vbDesc.bufferType	= gfx::BufferType::VertexBuffer;
	vbDesc.bufferUsage	= gfx::BufferUsage::Default;
	auto vertexBuffer	= graphicsFactory.createBuffer(vbDesc, vertices.data());

	gfx::BufferDescriptor ibDesc{};
	ibDesc.elementSize	= sizeof(uint32_t);
	ibDesc.bufferSize	= indices.size() * sizeof(uint32_t);
	ibDesc.bufferType	= gfx::BufferType::IndexBuffer;
	ibDesc.bufferUsage	= gfx::BufferUsage::Default;
	auto indexBuffer	= graphicsFactory.createBuffer(ibDesc, indices.data());

	const mth::vec2f screenSize = mainWindow->getSize();

	//Create a command buffer to encode graphics commands
	auto commandBuffer = graphicsFactory.createCommandBuffer();

	bool isWindowOpen = true;
	mainWindow->onWindowCloseDelegate.bind([&isWindowOpen](){ isWindowOpen = false; });

	while(isWindowOpen){
		//Tell the window that we've begun a frame
		mainWindow->beginFrame();

		//Put all commands inbetween a begin / end econding call
		commandBuffer->beginEncoding(mainWindow->getSurface()->getRenderTarget());

		//Clear the current render target
		commandBuffer->clearTarget();

		//Bind in the data the pipeline will need
		commandBuffer->setViewport({ 0, 0, static_cast<int32_t>(screenSize.x), static_cast<int32_t>(screenSize.y) });
		commandBuffer->bindPipelineObject(*pipeline);
		commandBuffer->bindVertexBuffer(*vertexBuffer, static_cast<uint32_t>(pipeline->getVertexLayout().size()));
		commandBuffer->bindIndexBuffer(*indexBuffer);

		//Issue a draw call
		commandBuffer->drawIndexed(indices.size());

		commandBuffer->endEncoding();

		//Let the window know that we've ended our frame
		mainWindow->endFrame();

		if(mainWindow->getKeyboard().isKeyPressed(Key::Escape)){
			break;
		}
	}

	return 0;
}