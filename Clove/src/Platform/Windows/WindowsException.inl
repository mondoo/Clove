namespace clv{
	inline const char* WindowsException::getType() const noexcept{
		return "Clove Windows Exception";
	}

	inline HRESULT WindowsException::getErrorCode() const noexcept{
		return hr;
	}

	inline std::string WindowsException::getErrorString() const noexcept{
		return translateErrorCode(hr);
	}
}