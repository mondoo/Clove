#pragma once

#include "Clove/Serialisation/Serialiser.hpp"

namespace garlic::clove{
    class YamlSerialiser : public Serialiser{
        //VARIABLES
    private:
        //FUNCTIONS
    public:
        std::string emitt() final;
    };
}