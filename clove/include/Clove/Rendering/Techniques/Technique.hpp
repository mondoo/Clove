#pragma once

#include "Clove/Rendering/ForwardRenderer3D.hpp"

#include <vector>

namespace garlic::clove {
    class Mesh;
    class ForwardRenderer3D;
}

namespace garlic::clove {
    class TechniqueInterface {
        //FUNCTIONS
    public:
        virtual void submit(ForwardRenderer3D &renderer, std::shared_ptr<Mesh> mesh) = 0;
    };

    /**
     * @brief Describes the steps involved to render geometry in a certain way.
     * @details A Technique's main responsibility is distribute a renderable into the 
     * the correct individual GeometryPasses. For example a lighting technique will
     * add a mesh into a shadow pass and then a colour pass.
     */
    template<typename... GeometryPasses>
    class Technique : public TechniqueInterface{
        //TYPES
    public:
        /**
         * @brief Links to a specific GeometryPass.
         */
        /* struct Step {
            //Pass id? or is this the pass itself?
        }; */

        //VARIABLES
    private:
        //std::vector<Step> steps;

        //FUNCTIONS
    public:
        //TODO: Ctors
        //TODO: Inl

        /**
         * @brief Creates the jobs required to perform this technique.
         * @param renderer Renderer to render the technique with.
         * @param mesh Mesh to perform the technique on.
         */
        void submit(ForwardRenderer3D &renderer, std::shared_ptr<Mesh> mesh) override {
            if(renderer.supportsPass<GeometryPasses>() && ...) {
                renderer.submitMesh<GeometryPasses...>(std::move(mesh));
            }
        }
    };
}