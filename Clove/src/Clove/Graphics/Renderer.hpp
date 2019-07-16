#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

#include "Clove/Graphics/Bindables/ShaderBufferObject.hpp"

namespace clv::gfx{
	class Bindable;

	struct VertexData{
		math::Matrix4f model;
		math::Matrix4f normalMatrix;
	};

	struct MaterialData{
		alignas(16) float sininess;
	};

	struct SpriteShaderData{
		math::Matrix4f modelProjection;
	};

	struct SubmitData{ //TODO: Should I just make a Drawable or something that the RenderableComponents hold?
		unsigned int indexCount = 0;
		math::Matrix4f modelData{};
		std::vector<std::shared_ptr<Bindable>> bindables; //Will require an atomic lock. Could be quite expensive
	};

	//We'll need the scene data, primarily from the camera
	struct SceneData{
		math::Matrix4f view;
		math::Matrix4f projection;
	};

	class Renderer{
		//VARIABLES
	protected:
		static std::shared_ptr<gfx::ShaderBufferObject<VertexData>> vertSBO;
		static std::shared_ptr<gfx::ShaderBufferObject<MaterialData>> materialSBO;
		static std::shared_ptr<gfx::ShaderBufferObject<SpriteShaderData>> spriteSBO;

		static std::vector<SubmitData> meshSubmissionData;
		static std::vector<SubmitData> spriteSubmissionData;

		//FUNCTIONS
	public:
		static void initialise();

		static void beginScene(); //How would I begin scene with the ecs?
		static void endScene();

		//TODO: Make const ref? relying on the invoking call to move
		static void submitMesh(SubmitData data);
		static void submitSprite(SubmitData data);

		//Below is how the data is going to come in
		//static void submitMesh(const math::Matrix4f& modelData, unsigned int indexCount, const std::vector<std::shared_ptr<Bindable>>& bindables);
		//static void submitSptrite(const math::Matrix4f& modelData, unsigned int indexCount, const std::shared_ptr<Bindable>& texture, const std::vector<std::shared_ptr<Bindable>>& bindables);
	
		//But how do I want it to look? is this fine?
		//Do we assume the bindable set data to it's own shader and we just update the globals?
		//Or do we have the shader to ourselves and do everything
		//	-I think the systems can update the data on the components. 
		//	The systems will prep as much as possible for the renderer so all it can worry about is optimisation / drawing

		//So we get something like this then:
		//static void submitMesh(unsigned int indexCount, const std::vector<std::shared_ptr<Bindable>>& bindables);
		//static void submitSptrite(unsigned int indexCount, const std::shared_ptr<Bindable>& texture, const std::vector<std::shared_ptr<Bindable>>& bindables);
	
		//Is there a way we can reduce the sprite to just the bindables?
		//I think if this renderer holds the 2D IB / VB then we can just grab the shader and texture from the sprite
		//That way we won't need the index count anymore

		//Making it like this
		//static void submitMesh(unsigned int indexCount, const std::vector<std::shared_ptr<Bindable>>& bindables);
		//static void submitSptrite(const std::vector<std::shared_ptr<Bindable>>& bindables);

		//Although keep in mind if we are setting shader values from here then we'll need to know which bindable
		//is the shader
		//	This seems to be the weakest system with the bindables. We generally need to know which one is which
		//	but never have an easy way to do this.
		//		I would consider having just one shader on here but then that limits me to updating everything
		//		the shader has in here. Ideally the RenderXDSystem would do all of that then in here we'd just
		//		do the view and lights (and any other world/global things)

		//I think the reason why I thought the binables needed to be shared_ptr is because I need to move them
		//around. I guess is I had some container class then I wouldn't really need to

		//Lets try not to do to too much at once. Let's start with moving the more global things here
		//I think the object specific stuff could be handled by the render system. because that will treat 
		//objects indivdually. And the renderer should be more conearned with the global stuff.

		//Also I think that maybe they shouldn't be 'render system' because they don't do any rendering. They
		//just prep the renderer

		//Maybe the biggest question is how should I handle the camera?

		//I think the ECS is the biggest cause of friction right now. It was just kind of put in without
		//much regardin how it'll fit into the rest of the program. Or much design on how I wanted it to
		//work

		//I'm almost considering moving all of this over to the RenderSystem (still have the RenderCommand / RenderAPI)
		//I think it's weird the lights and cameras aren't handled on that already. They're all part of the render system
		//This way the render system can chern through each 'renderable' and update their data.
		//	I'd have to be careful about to do this correctly though. It's useful to have the begin and end scene still
		//	(we even use the end scene currently). It'd also requrie a big refactor on how the systems define what the 
		//	components are that they use. However it does make a bit more sense with how the lights and camera will work
	
		//I think the best solution is probably rename the render systems (because they aren't rendering) and
		//have some sort of prep phase where the camera and lights can submit data to the scene
		//	-Not reall sure how the prep phase will work though

		//Probably remove the 'MeshComponent' / 'SpriteComponent' as I don't really need to do this (or maybe ever)

		//Also, maybe not have an array of bindables on the renderable components and just call out the bindables
		//individually? we need to know what type they are a lot more than performing bulk operations on them
	};
}