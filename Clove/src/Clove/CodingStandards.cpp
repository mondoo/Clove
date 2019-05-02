#include "clvpch.hpp"
#include "CodingStandards.hpp" //Keep own file inlcudes short

//use verbose includes for other files

//[namespace.source] always wrap the function declarations in clv namespace
namespace clv{
	CodingStandards::CodingStandards(){
	}

	CodingStandards::CodingStandards(CodingStandards&& other) noexcept = default;

	CodingStandards::CodingStandards(int a, int b){
		//custom ctor
	}

	CodingStandards::~CodingStandards() = default;

	void CodingStandards::nonMutable() const{
	}

	CodingStandards & CodingStandards::operator=(CodingStandards&& other) noexcept = default;

	void CodingStandards::constInFunction(){
		int const variablesToUseLayer = 4;


	}
}