#pragma once

#include "Membrane/EditorTypes.hpp"
#include "Membrane/Scene.hpp"

#include <Clove/ECS/Entity.hpp>
#include <Clove/Layer.hpp>
#include <Clove/Rendering/Camera.hpp>
#include <msclr/gcroot.h>
#include <string_view>
#include <vector>

namespace garlic::membrane {
    //TODO: Runtime and editor are very similar except they are attached / detached and don't run together?
    //Editor layer pops physics - runtime pushes physics
    //Editor attaches the editor camera + controls etc.

    /**
     * @brief The layer that controls the interaction between the editor and the engine.
     * Is active while 'editing' and allows the user to build a scene
     */
    class EditorLayer : public clove::Layer {
        friend ref class EditorLayerMessageProxy;

        //VARIABLES
    private:
        msclr::gcroot<EditorLayerMessageProxy ^> proxy;

        Scene currentScene;

        clove::Entity editorCamera;

        bool moveMouse{ false };
        clove::vec2i mousePos{};
        clove::vec2i prevMousePos{};
        float mouseLookYaw{};
        float mouseLookPitch{};

        //FUNCTIONS
    public:
        EditorLayer();

        void onAttach() override;
        clove::InputResponse onInputEvent(clove::InputEvent const &inputEvent) override;
        void onUpdate(clove::DeltaTime const deltaTime) override;
        void onDetach() override;

    private:
        //TODO: Save and load scene to custom file
        void saveScene();
        void loadScene();

        clove::Entity createEntity(std::string_view name = "New Entity");
        void createComponent(clove::Entity entity, ComponentType componentType);

        void updateTransform(clove::Entity entity, clove::vec3f position, clove::vec3f rotation, clove::vec3f scale);
        void updateRigidBody(clove::Entity entity, float mass);
        void updateName(clove::Entity entity, std::string name);
    };
}