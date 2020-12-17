#pragma once

#include <Clove/ECS/Entity.hpp>
#include <Clove/Layer.hpp>
#include <Clove/Rendering/Camera.hpp>

namespace garlic::membrane {
    /**
     * @brief The layer that holds all the editor time components
     * @details This will be things such as the gizmos, editor camera etc. 
     */
    class EditorLayer : public clove::Layer {
    private:
        clove::Entity editorCamera;
        clove::Viewport viewport;

    public:
        EditorLayer(clove::vec2ui size);

        void onAttach() override;
        void onUpdate(clove::DeltaTime const deltaTime) override;
        void onDetach() override;

        void resizeViewport(clove::vec2ui size);

    };
}