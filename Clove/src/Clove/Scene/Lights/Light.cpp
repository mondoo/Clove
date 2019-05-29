#include "clvpch.hpp"
#include "Light.hpp"

namespace clv::scene{
	Light::Light() = default;

	Light::Light(const Light& other) = default;

	Light::Light(Light&& other) noexcept = default;

	Light& Light::operator=(const Light& other) = default;

	Light& Light::operator=(Light&&) noexcept = default;

	Light::~Light() = default;
}