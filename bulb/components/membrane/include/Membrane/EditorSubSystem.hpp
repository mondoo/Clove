#pragma once

#include "Membrane/Scene.hpp"

#include <Clove/ECS/Entity.hpp>
#include <Clove/SubSystem.hpp>
#include <Clove/Rendering/Camera.hpp>
#include <msclr/gcroot.h>
#include <string_view>
#include <vector>

namespace membrane {
    /**
     * @brief The sub system that controls the interaction between the editor and the engine.
     * Is active while 'editing' and allows the user to build a scene
     */
    class EditorSubSystem : public clove::SubSystem {
        friend ref class EditorSubSystemMessageProxy;

        //VARIABLES
    private:
        msclr::gcroot<EditorSubSystemMessageProxy ^> proxy;

        Scene currentScene;

        clove::Entity editorCamera{};

        bool moveMouse{ false };
        clove::vec2i mousePos{};
        clove::vec2i prevMousePos{};
        float mouseLookYaw{};
        float mouseLookPitch{};

        //FUNCTIONS
    public:
        EditorSubSystem();
        ~EditorSubSystem();

        void onAttach() override;
        clove::InputResponse onInputEvent(clove::InputEvent const &inputEvent) override;
        void onUpdate(clove::DeltaTime const deltaTime) override;
        void onDetach() override;

    //private:
        //TODO: Save and load scene to custom file
        void saveScene();
        void loadScene();

    private:
        clove::Entity createEntity(std::string_view name = "New Entity");
        void deleteEntity(clove::Entity entity);

        void addComponent(clove::Entity entity, std::string_view typeName);
        void modifyComponent(clove::Entity entity, std::string_view typeName, array<uint8_t> ^data);
        void removeComponent(clove::Entity entity, std::string_view typeName); 

        void updateName(clove::Entity entity, std::string name);
    };
}