#pragma once

#include "Clove/SubSystem.hpp"

namespace clove {
    class EntityManager;
}

namespace clove {
    /**
     * @brief Calculates the world matrices of the entities.
     */
    class TransformSubSystem : public SubSystem {
        //VARIABLES
    private:
        EntityManager *entityManager{ nullptr };

        //FUNCTIONS
    public:
        TransformSubSystem() = delete;
        TransformSubSystem(EntityManager *entityManager);

        TransformSubSystem(TransformSubSystem const &other) = delete;
        TransformSubSystem(TransformSubSystem &&other) noexcept;

        TransformSubSystem &operator=(TransformSubSystem const &other) = delete;
        TransformSubSystem &operator=(TransformSubSystem &&other) noexcept;

        ~TransformSubSystem();

        Group getGroup() const override;

        void onAttach() override{};
        InputResponse onInputEvent(InputEvent const &inputEvent) override{ return InputResponse::Ignored; }
        void onUpdate(DeltaTime const deltaTime) override;
        void onDetach() override{};
    };
}
