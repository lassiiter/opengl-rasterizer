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
            // update uniforms
            shader->set_vec3(mColor, "u_tint");
            shader->set_f1(mRoughness, "u_roughness");
            shader->set_f1(mMetallic, "u_metallic");
            shader->set_f1(mDiffuseIBLEnabled, "u_enable_IBL_diffuse");
            shader->set_f1(mSpecularIBLEnabled, "u_enable_IBL_specular");
            shader->set_f1(mNormalEnabled, "u_enable_normal");
        }

        glm::vec3 mColor = { 1.0f, 1.0f, 1.0f };
        float mRoughness = 1.0f;
        float mMetallic = 1.0f;
        float mDiffuseIBLEnabled = 1.0f;
        float mSpecularIBLEnabled = 1.0f;
        float mNormalEnabled = 1.0f;
    };
}