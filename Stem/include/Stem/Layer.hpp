#pragma once

#include "Bulb/InputResponse.hpp"

namespace clv {
    struct InputEvent;
}

namespace garlic::inline stem {
    class Layer {
        //VARIABLES
    protected:
        std::string debugName;

        //FUNCTIONS
    public:
        Layer(std::string name = "Layer");
        virtual ~Layer() = default;

        virtual void onAttach() {}
        virtual blb::InputResponse onInputEvent(clv::InputEvent const &inputEvent);
        virtual void onUpdate(clv::utl::DeltaTime const deltaTime) {}
        virtual void onDetach() {}

        inline const std::string &getName() const;
    };
}

#include "Layer.inl"