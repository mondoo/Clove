#pragma once

namespace clv::gfx{
	enum class API;

	class Context{
		//FUNCTIONS
	public:
		Context() = default;
		Context(const Context& other) = delete;
		Context(Context&& other) noexcept;
		Context& operator=(const Context& other) = delete;
		Context& operator=(Context&& other) noexcept;
		virtual ~Context();

		static std::unique_ptr<Context> createContext(void* windowData, API api);

		virtual void present() = 0;
	};
}
