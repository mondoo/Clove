#pragma once

#include "Clove/ECS/System.hpp"

#include "Clove/ECS/UI/Components/FontComponent.hpp"
#include "Clove/ECS/2D/Components/TransformComponent.hpp" //TODO: This will probably have to be a UI position component


//Temp
//#include "Clove/Graphics/Sprite.hpp"

//Forward dec (see freetype.h)
typedef struct FT_LibraryRec_* FT_Library;
typedef struct FT_FaceRec_* FT_Face;

namespace clv::ecs::ui{
	class FontSystem : public System<FontComponent, d2::TransformComponent>{
		//VARIABLES
	public:
		//TEMP:
		//std::shared_ptr<gfx::Sprite> sprite;

		FT_Library ft;
		FT_Face face;

		//FUNCTIONS
	public:
		//TODO: The rest
		FontSystem();
		~FontSystem();

		virtual void update(utl::DeltaTime deltaTime) override;
	};
}