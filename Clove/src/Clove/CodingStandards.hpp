#pragma once

/*
coding standards!

*/

//[IWYU] please keep includes in header files as small as possible

//[namespace.header] always wrap the class in the clv namespace
namespace clv{
	class CodingStandards{
		//[order.section.variables] variables first with comment please!
		//VARIABLES
	public:

	protected:

	private:
		//[initalisation] always initialise variables in the header when possible
		int myInt = 0;

		//[raw pointer] avoid raw pointers - try and use shared_ptr or unique_ptr
		int* rawPtrInt = nullptr;

		//[order.section.functions] functions second with comment please!
		//FUNCTIONS
	public:
		CodingStandards();
		//[destructor.default] no empty constructors! either use default or just remove the definition
		~CodingStandards() = default;

		//[function.inline] please do not do this!
		inline void badInlineFunction() { /*do some thing*/ }

		//[function.inline] if you need it inline, mark it as such and declare in an inl file
		inline void goodInlineFunction();

		//[function.const correct] please keep functiosn const correct
		void nonMutable() const;

	protected:
		void constInFunction();

	private:
	};
}

//[inline.include] make sure the .inl is included (if exists)
#include "CodingStandards.inl"