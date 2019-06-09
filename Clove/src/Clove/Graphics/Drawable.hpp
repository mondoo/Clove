#pragma once

namespace clv::gfx{
	class Renderer;
	class Bindable;
	class IndexBuffer;

	class [[deprecated("Being replaced by ecs::RenderableComponent")]] Drawable{
		//VARIABLES
	private:
		std::vector<std::unique_ptr<Bindable>> bindables;

		IndexBuffer* indexBuffer = nullptr;

		//FUNCTIONS
	public:
		Drawable();
		Drawable(const Drawable& other) = delete;
		Drawable(Drawable&& other) noexcept;
		Drawable& operator=(const Drawable& other) = delete;
		Drawable& operator=(Drawable&& other) noexcept;
		virtual ~Drawable();

		void draw(Renderer& renderer);

	protected:
		void addBindable(std::unique_ptr<Bindable> bindable);
		void addIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer);
	};
}