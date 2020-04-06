#pragma once

namespace clv::gfx {
	class GraphicsFactory;
}

namespace clv::gfx {
	class FactoryChild {
		//FUNCTIONS
	public:
		virtual ~FactoryChild() = default;

		virtual std::shared_ptr<GraphicsFactory> getFactory() const = 0;
	};
}