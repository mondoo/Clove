#include "clvpch.hpp"
#include "DirectionalLight.hpp"

namespace clv{
	namespace scene{
		DirectionalLight::DirectionalLight() = default;

		DirectionalLight::DirectionalLight(const DirectionalLight& other) = default;

		DirectionalLight::DirectionalLight(DirectionalLight&& other) noexcept{
			direction = std::move(other.direction);
			Light(std::move(other));
		}

		DirectionalLight::~DirectionalLight() = default;

		DirectionalLight& DirectionalLight::operator=(const DirectionalLight& other) = default;

		DirectionalLight& DirectionalLight::operator=(DirectionalLight&& other) noexcept = default;
	}
}