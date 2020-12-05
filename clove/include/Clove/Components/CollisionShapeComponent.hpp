#pragma once

#include <Clove/ECS/Component.hpp>
#include <Clove/Maths/Vector.hpp>
#include <memory>
#include <variant>

class btCollisionShape;
class btGhostObject;

namespace garlic::clove{
    enum class ShapeType {
        Sphere,
        Cube
    };

    struct SphereShape{
        float radius{ 1.0f };
    };
    struct CubeShape{
        vec3f halfExtents{ 1.0f, 1.0f, 1.0f };
    };

    using ShapeVariant = std::variant<SphereShape, CubeShape>;
}

namespace garlic::clove {
    /**
     * @brief Provides a cube collision shape for an entity.
     * @details On it's own a CollisionShapeComponent can act like a trigger.
     * If an Entity also has a RigidBodyComponent then the Entity will respond
     * to gravity and other physics events using this component as it's collision
     * shape.
     * @see CollisionResponseComponent
     * @see RigidBodyComponent
     */
    class CollisionShapeComponent : public Component<CollisionShapeComponent> {
        friend class PhysicsSystem;

        //VARIABLES
    private:
        std::unique_ptr<btCollisionShape> collisionShape;
        std::unique_ptr<btGhostObject> collisionObject; /**< Acts as a stand in until the Entity has a RigidBodyComponent. */

        ShapeVariant shape;

        //FUNCTIONS
    public:
        CollisionShapeComponent() = delete;
        CollisionShapeComponent(ShapeVariant shape);

        CollisionShapeComponent(CollisionShapeComponent const &other);
        CollisionShapeComponent(CollisionShapeComponent &&other) noexcept;

        CollisionShapeComponent &operator=(CollisionShapeComponent const &other);
        CollisionShapeComponent &operator=(CollisionShapeComponent &&other) noexcept;

        ~CollisionShapeComponent();

        void setShape(ShapeVariant shape);
        ShapeVariant const &getShape() const;

    private:
        void initialiseCollisionShape();

        void constructCollisionObject();
    };
}