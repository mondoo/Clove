#pragma once

#include "Clove/InputResponse.hpp"
#include "Clove/InputEvent.hpp"

#include <Clove/DeltaTime.hpp>

namespace clove {
    /**
     * @brief A SubSystem represents a slice of functionality that can be injected into Clove.
     */
    class SubSystem {
        //VARIABLES
    protected:
        std::string debugName;

        //FUNCTIONS
    public:
        SubSystem(std::string name = "Layer");
        virtual ~SubSystem() = default;

        virtual void onAttach() {}
        virtual InputResponse onInputEvent(InputEvent const &inputEvent);
        virtual void onUpdate(DeltaTime const deltaTime) {}
        virtual void onDetach() {}

        inline const std::string &getName() const;
    };
}

#include "SubSystem.inl"