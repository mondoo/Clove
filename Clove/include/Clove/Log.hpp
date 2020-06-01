#pragma once

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

namespace clv {
	namespace Log {
		//TYPES
		enum class Level {
			Trace,
			Debug,
			Warning,
			Error,
			Critical
		};

		struct Context {
		private:
			spdlog::logger logger;
		};

		Context createContext(std::string_view name) {
		}

		template<typename... Args>
		void log(const Context& context, Level level, Args&&... args) {
		}
	};
}

#define GARLIC_LOG(context, level, ...) ::clv::Log::log(contest, level, __VA_ARGS__);