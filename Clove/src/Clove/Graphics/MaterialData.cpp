#include "clvpch.hpp"
#include "MaterialData.hpp"

namespace clv::gfx{
	MaterialData::MaterialData(const MaterialData& other) = default;

	MaterialData& MaterialData::operator=(const MaterialData& other) = default;

	MaterialData::MaterialData(MaterialData&& other) noexcept = default;

	MaterialData& MaterialData::operator=(MaterialData&& other) noexcept = default;

	MaterialData::~MaterialData() = default;

	MaterialData::MaterialData(unsigned int bindingPoint)
		: bindingPoint(bindingPoint){
	}
	
	size_t MaterialData::getSize() const{
		return size;
	}
}