#include "clvpch.hpp"
#include "DX11IndexBuffer.hpp"

#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Graphics/DirectX-11/DX11Exception.hpp"
//#include "Graphics/DirectX-11/DX11Renderer.hpp"
//#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/RenderCommand.hpp"

#include <d3d11.h>

namespace clv::gfx{
	DX11IndexBuffer::DX11IndexBuffer(DX11IndexBuffer&& other) noexcept = default;

	DX11IndexBuffer& DX11IndexBuffer::operator=(DX11IndexBuffer&& other) noexcept = default;

	DX11IndexBuffer::~DX11IndexBuffer() = default;

	DX11IndexBuffer::DX11IndexBuffer(const std::vector<unsigned int>& indices){
		DX11Renderer* dxrenderer = static_cast<DX11Renderer*>(&Application::get().getRenderer());
		DX11_INFO_PROVIDER(dxrenderer);

		count = static_cast<unsigned int>(indices.size());

		D3D11_BUFFER_DESC  ibd = { };
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0u;
		ibd.ByteWidth = static_cast<UINT>(indices.size() * sizeof(unsigned int));
		ibd.StructureByteStride = sizeof(unsigned int);

		D3D11_SUBRESOURCE_DATA isrd = { };
		isrd.pSysMem = indices.data();

		DX11_THROW_INFO(dxrenderer->getDevice().CreateBuffer(&ibd, &isrd, &indexBuffer));
	}

	void DX11IndexBuffer::bind(){
		RenderCommand::setIndexBuffer(*this); //TODO: This is how I'm thinking at the moment - could change
		//Thinking about it this _might_ be moved onto the render. It it's going to batch everything
		//That's not a problem for now though, because batching would require a rethink of how renderables
		//would store this data.

		//DX11Renderer* dxrenderer = static_cast<DX11Renderer*>(&renderer);
		//dxrenderer->getContext().IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);

		//should the context here literally just be the context class?
		//the device issues the commands and the context creates. so it could work?
		//how are the opengl context expected to work?
		//How should the context class in general work now?
		//	-it's the other way around. Device creates, context issue commands
		//	-opeengl context is really only there to handle different platforms

		//How do I get access to the device or context now though? The render api is tucked away

		/*
		Do we event want objects to be able to get access to the raw renderapi like that?
		I have a feeling that everything should go through the command so it won't flim flam the threading.
		Although, it's only supposed to represent 'commands' so can we bypass it to create our objects?

		Is there a part of DX I can use to get around this?

		Do I call off to the render command to create these?
			-Haven't seen people do that, usually they pass the devices / contexts down here

		Do I just pass the actual context down instead of the renderer?
			-How does creation / binding of objects work for multi threading

		Do I make a render command to set the index buffer??????
			-Looking like I might have to. In DX it's the context that gets deffered where as the device isn't
				-And it is technically a render command
				-How do I abstract this though?
				-The only way I think this wouldn't be like this is if there was another class involved?
			-Will I even use the in built dx functionality for rendering?
				-I mean I should, and I'll at least follow the structure (command lists etc)


		Also should the context be called the RenderPlatformContext?
		*/
	}

	void DX11IndexBuffer::unbind(){
	}

	unsigned int DX11IndexBuffer::getIndexCount() const{
		return count;
	}
	
	ID3D11Buffer* DX11IndexBuffer::getBuffer() const{
		return indexBuffer.Get();
	}
}