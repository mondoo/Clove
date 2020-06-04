#pragma once

#include <spdlog/spdlog.h>

namespace clv {
	namespace Log {
		//TYPES
		enum class Level {
			Trace,
			Debug,
			Info,
			Warning,
			Error,
			Critical
		};

		struct Context {
		private:
			std::shared_ptr<spdlog::logger> logger;

		public:
			Context() = delete;
			Context(std::shared_ptr<spdlog::logger> logger)
				: logger(std::move(logger)) {
			}

			template<typename... Args>
			void log(Level level, const char* msg, Args&&... args);

			template<typename T>
			void log(Level level, const T& msg);
		};

		Context createContext(Log::Level consoleLogLevel, std::string_view loggerName, std::string_view fileName);
	};
}

#include "Log.inl"

#define GARLIC_LOG(context, level, ...) context.log(level, __VA_ARGS__);

extern clv::Log::Context garlicLogContext;