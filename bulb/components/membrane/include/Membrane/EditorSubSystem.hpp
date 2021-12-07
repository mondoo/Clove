#pragma once

#include "Membrane/EditorTypes.hpp"
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

        Group getGroup() const override;

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

        void addComponent(clove::Entity entity, ComponentType componentType);
        void removeComponent(clove::Entity entity, ComponentType componentType);

        void updateTransform(clove::Entity entity, clove::vec3f position, clove::vec3f rotation, clove::vec3f scale);
        void updateStaticModel(clove::Entity entity, std::string meshPath, std::string diffusePath, std::string specularPath);
        void updateRigidBody(clove::Entity entity, float mass);
        void updateSphereShape(clove::Entity entity, float radius);
        void updateCubeShape(clove::Entity entity, clove::vec3f halfExtents);
        void updateName(clove::Entity entity, std::string name);
    };
}