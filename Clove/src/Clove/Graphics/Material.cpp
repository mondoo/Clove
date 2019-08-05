#include "clvpch.hpp"
#include "Material.hpp"

namespace clv::gfx{
	Material::Material() = default;

	Material::Material(const Material& other) = default;

	Material& Material::operator=(const Material& other) = default;

	Material::Material(Material&& other) noexcept = default;

	Material& Material::operator=(Material&& other) noexcept = default;

	Material::~Material() = default;
}