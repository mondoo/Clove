#pragma once

#include "Clove/ECS/System.hpp"

#include "Clove/ECS/UI/Components/TextComponent.hpp"
#include "Clove/ECS/2D/Components/TransformComponent.hpp" //TODO: This will probably have to be a UI position component

//Forward dec (see freetype.h)
typedef struct FT_LibraryRec_* FT_Library;
typedef struct FT_FaceRec_* FT_Face;

namespace clv::ecs::ui{
	class TextSystem : public System<TextComponent, d2::TransformComponent>{
		//VARIABLES
	public:
		FT_Library ft;
		FT_Face face;

		//FUNCTIONS
	public:
		//TODO: The rest
		TextSystem();
		~TextSystem();

		virtual void update(utl::DeltaTime deltaTime) override;
	};
}