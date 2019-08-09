#pragma once

namespace clv::gfx{
	class Material;

	class Sprite{
		//VARIABLES
	private:
		std::shared_ptr<Material> material;

		//FUNCTIONS
	public:
		Sprite();
		Sprite(const Sprite& other);
		Sprite& operator=(const Sprite& other);
		Sprite(Sprite&& other) noexcept;
		Sprite& operator=(Sprite&& other) noexcept;
		~Sprite();

		void setMaterial(const std::shared_ptr<Material>& material);
		//TODO: shared_ptr or ref?
		Material& getMaterial(){
			return *material;
		}

		void bind();
	};
}
