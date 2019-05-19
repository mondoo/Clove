#pragma once

#include <queue>

namespace clv{
	class Window;
}

namespace clv::gfx{
	class Drawable;

	enum class API{
		OpenGL,
		DirectX11
	};

	class Renderer{
		friend class Drawable;

		//VARIABLES
	private:
		std::queue<std::weak_ptr<Drawable>> drawables;

		//FUNCTIONS
	public:
		Renderer();
		Renderer(const Renderer& other) = delete;
		Renderer(Renderer&& other) noexcept = delete;
		Renderer& operator=(const Renderer& other) = delete;
		Renderer& operator=(Renderer&& other) noexcept = delete;
		virtual ~Renderer();

		static std::unique_ptr<Renderer> createRenderer(const Window& window, API api);

		void submit(const std::shared_ptr<Drawable>& drawable);

		virtual void clear() = 0;
		void draw();

		inline virtual API getAPI() const = 0;

	protected:
		virtual void drawIndexed(const unsigned int count) = 0;
	};
}