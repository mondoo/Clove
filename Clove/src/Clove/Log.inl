namespace clv{
	inline std::shared_ptr<spdlog::logger>& Log::getCoreLogger(){
		return coreLogger;
	}

	inline std::shared_ptr<spdlog::logger>& Log::getClientLogger(){
		return clientLogger;
	}
}