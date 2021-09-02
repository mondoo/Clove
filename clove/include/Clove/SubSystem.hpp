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
        inline SubSystem(std::string name);
        virtual ~SubSystem() = default;

        virtual void onAttach() = 0;
        virtual InputResponse onInputEvent(InputEvent const &inputEvent) = 0;
        virtual void onUpdate(DeltaTime const deltaTime)                 = 0;
        virtual void onDetach()                                          = 0;

        inline const std::string &getName() const;
    };
}

#include "SubSystem.inl"