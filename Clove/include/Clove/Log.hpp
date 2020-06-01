#pragma once

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

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
			void log(Level level, const char* msg, Args&&... args) {
				switch(level) {
					case Level::Trace:
						logger->trace(msg, args...);
						break;
					case Level::Debug:
						logger->debug(msg, args...);
						break;
					case Level::Info:
						logger->debug(msg, args...);
						break;
					case Level::Warning:
						logger->warn(msg, args...);
						break;
					case Level::Error:
						logger->error(msg, args...);
						break;
					case Level::Critical:
						logger->critical(msg, args...);
						break;
				}
			}

			template<typename T>
			void log(Level level, const T& msg) {
				switch(level) {
					case Level::Trace:
						logger->trace(msg);
						break;
					case Level::Debug:
						logger->debug(msg);
						break;
					case Level::Info:
						logger->debug(msg);
						break;
					case Level::Warning:
						logger->warn(msg);
						break;
					case Level::Error:
						logger->error(msg);
						break;
					case Level::Critical:
						logger->critical(msg);
						break;
				}
			}
		};

		Context createContext(std::string_view loggerName, std::string_view fileName);
	};
}

#define GARLIC_LOG(context, level, ...) context.log(level, __VA_ARGS__);

//Temp
static inline clv::Log::Context garlicLogContext = clv::Log::createContext("GARLIC", "GarlicLog.txt");