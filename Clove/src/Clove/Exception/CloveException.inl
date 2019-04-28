namespace clv{
	inline int CloveException::getLineNum() const noexcept{
		return lineNum;
	}

	inline const std::string& CloveException::getFile() const noexcept{
		return file;
	}
}