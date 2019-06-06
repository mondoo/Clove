#pragma once

namespace clv::gfx{
	class Context{
		//FUNCTIONS
	public:
		Context() = default;
		Context(const Context& other) = delete;
		Context(Context&& other);
		Context& operator=(const Context& other) = delete;
		Context& operator=(Context&& other);
		virtual ~Context();

		virtual void present() = 0;
	};
}
