#pragma once

#include <exception>

namespace clv{
	class CloveException : public std::exception{
		//VARIABLES
	protected:
		mutable std::string whatBuffer;

	private:
		int lineNum = -1;
		std::string file = "";

		//FUNCTIONS
	public:
		CloveException() = delete;
		CloveException(const CloveException& other) = delete;
		CloveException(CloveException&& other) noexcept = delete;

		CloveException(int lineNum, const char* file);

		virtual ~CloveException();

		virtual const char* what() const noexcept override;
		
		virtual const char* getType() const noexcept;

		inline int getLineNum() const noexcept;
		inline const std::string& getFile() const noexcept;
		std::string getOriginString() const noexcept;

		CloveException& operator=(const CloveException& other) = delete;
		CloveException& operator=(CloveException&& other) noexcept = delete;
	};
}

#include "CloveException.inl"