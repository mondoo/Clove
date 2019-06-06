#include "clvpch.hpp"
#include "Context.hpp"

namespace clv::gfx{
	Context::Context(Context&& other) = default;

	Context& Context::operator=(Context&& other) = default;

	Context::~Context() = default;
}