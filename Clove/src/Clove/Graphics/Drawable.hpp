#pragma once

namespace clv{
	namespace gfx{
		class Bindable;

		class Drawable{
			//VARIABLES
		private:
			std::vector<std::unique_ptr<Bindable>> bindables;

			//FUNCTIONS
		public:
			Drawable();
			Drawable(const Drawable& other) = delete;
			Drawable(Drawable&& other) noexcept;

			virtual ~Drawable();

			//Draw?

			//Will need a way to get the index count

			void addBindable(std::unique_ptr<Bindable> bindable);

			Drawable& operator=(const Drawable& other) = delete;
			Drawable& operator=(Drawable&& other) noexcept;
		};
	}
}