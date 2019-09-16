#pragma once

/*
Inteprets font files into some generic data that can be used to create tex coords ect.

TODO: put inside utility???
*/

namespace clv::ui{
	class Font{
		//VARIABLES
	private:

		//FUNCTIONS
	public:
		//TODO: others
		Font();

		Font(const std::string& filePath);
	};
}