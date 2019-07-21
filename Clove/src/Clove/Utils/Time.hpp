#pragma once

namespace clv{
	class Application;
}

namespace clv::utl{
	class DeltaTime{
		//VARIABLES
	private:
		float deltaSeconds = 0.0f;

		//FUNCTIONS
	public:
		DeltaTime();
		DeltaTime(const DeltaTime& other);
		DeltaTime& operator=(const DeltaTime& other);
		DeltaTime(DeltaTime&& other) noexcept;
		DeltaTime& operator=(DeltaTime&& other) noexcept;
		~DeltaTime();

		DeltaTime(float deltaSeconds);

		float getDeltaSeconds() const;

		operator float() const;
	};
}