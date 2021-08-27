#pragma once

#include <memory>

namespace clove {
    template<typename GhaObjectType, typename... Args>
    std::unique_ptr<GhaObjectType> createGhaObject(Args &&...args);
}

#include "Helpers.inl"