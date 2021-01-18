#pragma once

#include "Clove/Reflection/Serialiser.hpp"

namespace garlic::clove{
    class YamlSerialiser : public Serialiser{
        //VARIABLES
    private:
        //FUNCTIONS
    public:
        std::string emitt() override;
    };
}