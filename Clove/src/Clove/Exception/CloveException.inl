namespace clv{
	inline const char* CloveException::getType() const noexcept{
		return "Clove Exception";
	}

	inline int CloveException::getLineNum() const noexcept{
		return lineNum;
	}

	inline const std::string& CloveException::getFile() const noexcept{
		return file;
	}
}