#include "pch.h"
#include "mesh.h"
#include "shader/shader_util.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <vector>
namespace nelems
{
    class Model : public Element
    {
    public:
        // model data 
        std::vector<std::shared_ptr<nelems::Mesh>> meshes;

        void render();

        void load(std::string const& filepath);

        void update(nshaders::Shader* shader)
        {
            // pbr color
            shader->set_vec3(mColor, "albedo");
            shader->set_f1(mRoughness, "roughness");
            shader->set_f1(mMetallic, "metallic");
            shader->set_f1(1.0f, "ao");
        }

        glm::vec3 mColor = { 1.0f, 0.0f, 0.0f };
        float mRoughness = 0.2f;
        float mMetallic = 0.1f;
    };
}