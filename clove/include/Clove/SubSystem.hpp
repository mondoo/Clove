#pragma once

#include "Clove/InputEvent.hpp"
#include "Clove/InputResponse.hpp"

#include <Clove/DeltaTime.hpp>

namespace clove {
    /**
     * @brief A SubSystem represents a slice of functionality that can be injected into Clove.
     */
    class SubSystem {
        //TYPES
    public:
        enum class Group {
            Initialisation, /**< For layers that need to perform logic before anything else. */
            Core,           /**< The default group. For layers that contain core / general functionality. */
            Interface,      /**< For layers that contain interface logic. Usually UI */
            Render,         /**< For layers that need be run very last. Usually for some form of rendering logic. */
        };

        //VARIABLES
    protected:
        std::string debugName;

        //FUNCTIONS
    public:
        inline SubSystem(std::string name);
        virtual ~SubSystem() = default;

        virtual Group getGroup() const = 0;

        virtual void onAttach()                                          = 0;
        virtual InputResponse onInputEvent(InputEvent const &inputEvent) = 0;
        virtual void onUpdate(DeltaTime const deltaTime)                 = 0;
        virtual void onDetach()                                          = 0;

        inline const std::string &getName() const;
    };
}

#include "SubSystem.inl"