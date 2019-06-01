#include "clvpch.hpp"
#include "Texture.hpp"

namespace clv::gfx{
	Texture::Texture() = default;

	Texture::Texture(Texture&& other) noexcept = default;

	Texture& Texture::operator=(Texture&& other) noexcept = default;

	Texture::~Texture() = default;
}