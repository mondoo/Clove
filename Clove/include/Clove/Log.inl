namespace clv {
	namespace Log {
		template<typename... Args>
		void Context::log(Level level, const char* msg, Args&&... args) {
			switch(level) {
				case Level::Trace:
					logger->trace(msg, args...);
					break;
				case Level::Debug:
					logger->debug(msg, args...);
					break;
				case Level::Info:
					logger->info(msg, args...);
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
		void Context::log(Level level, const T& msg) {
			switch(level) {
				case Level::Trace:
					logger->trace(msg);
					break;
				case Level::Debug:
					logger->debug(msg);
					break;
				case Level::Info:
					logger->info(msg);
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
	}
}