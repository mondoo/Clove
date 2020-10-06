namespace garlic::inline root {
    template<typename... Args>
    void LogContext::log(LogLevel level, const char* msg, Args&&... args) {
        switch(level) {
            case LogLevel::Trace:
                logger->trace(msg, args...);
                break;
            case LogLevel::Debug:
                logger->debug(msg, args...);
                break;
            case LogLevel::Info:
                logger->info(msg, args...);
                break;
            case LogLevel::Warning:
                logger->warn(msg, args...);
                break;
            case LogLevel::Error:
                logger->error(msg, args...);
                break;
            case LogLevel::Critical:
                logger->critical(msg, args...);
                break;
        }
    }

    template<typename T>
    void LogContext::log(LogLevel level, const T& msg) {
        switch(level) {
            case LogLevel::Trace:
                logger->trace(msg);
                break;
            case LogLevel::Debug:
                logger->debug(msg);
                break;
            case LogLevel::Info:
                logger->info(msg);
                break;
            case LogLevel::Warning:
                logger->warn(msg);
                break;
            case LogLevel::Error:
                logger->error(msg);
                break;
            case LogLevel::Critical:
                logger->critical(msg);
                break;
        }
    }
}