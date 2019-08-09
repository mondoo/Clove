#pragma once

namespace clv::gfx{
	class Material;

	class Sprite{
		//VARIABLES
	private:
		std::shared_ptr<Material> material;

		//FUNCTIONS
	public:
		//TODO ctors

		void setMaterial(const std::shared_ptr<Material>& material);
		//TODO: shared_ptr or ref?
		Material& getMaterial(){
			return *material;
		}

		void bind();
	};
}
