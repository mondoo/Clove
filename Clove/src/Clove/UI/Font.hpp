#pragma once

/*
Inteprets font files into some generic data that can be used to create tex coords ect.

TODO: put inside utility???
*/


/*
-a reusable class
-holds the data of the font, can be used to extract each character

*/

namespace clv::ui{
	class Font{ //I wonder if we need like a 2d drawable interface?? (although, what would that solve?)
		//VARIABLES
	private:

		//FUNCTIONS
	public:
		//TODO: others
		Font();

		Font(const std::string& filePath);


	};
}