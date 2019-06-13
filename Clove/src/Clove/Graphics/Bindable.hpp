#pragma once

namespace clv::gfx{
	class Renderer;

	class Bindable{
		//FUNCTIONS
	public:
		Bindable();
		Bindable(const Bindable& other) = delete;
		Bindable(Bindable&& other) noexcept;
		Bindable& operator=(const Bindable& other) = delete;
		Bindable& operator=(Bindable&& other) noexcept;
		virtual ~Bindable();

		virtual void bind(Renderer& renderer) = 0;
		virtual void unbind() = 0;
	};
}
