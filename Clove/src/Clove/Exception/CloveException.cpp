#include "CloveException.hpp"

namespace clv{
	CloveException::CloveException(int32 lineNum, const char* file)
		: lineNum(lineNum)
		, file(file){
	}

	CloveException::~CloveException() = default;

	const char* CloveException::what() const noexcept{
		std::ostringstream oss;
		oss << getType() << std::endl
			<< getOriginString();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* CloveException::getType() const noexcept{
		return "Clove Exception";
	}

	int32 CloveException::getLineNum() const noexcept{
		return lineNum;
	}

	const std::string& CloveException::getFile() const noexcept{
		return file;
	}

	std::string CloveException::getOriginString() const noexcept{
		std::ostringstream oss;
		oss << "[FILE] " << file << std::endl
			<< "[LINE] " << lineNum;
		return oss.str();
	}
}