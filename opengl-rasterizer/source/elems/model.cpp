#include "pch.h"
#include "mesh.h"
#include "shader/shader_util.cpp"


#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <vector>
namespace nelems
{
    class Model
    {
    public:
        // model data 
        std::vector<Mesh> meshes;

        Model(std::string const& filepath)
        {
           load(filepath);
        }

        void Draw(nshaders::Shader& shader);

    private:
        void load(std::string const& filepath)
        {
            const uint32_t cMeshImportFlags =
                aiProcess_CalcTangentSpace |
                aiProcess_Triangulate |
                aiProcess_SortByPType |
                aiProcess_GenNormals |
                aiProcess_GenUVCoords |
                aiProcess_OptimizeMeshes |
                aiProcess_ValidateDataStructure;

            Assimp::Importer Importer;
            const aiScene* pScene = Importer.ReadFile(filepath.c_str(), cMeshImportFlags);

            // check for errors
            if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode) // if is Not Zero
            {
                std::cout << "ERROR::ASSIMP:: " << Importer.GetErrorString() << std::endl;
                return;
            }
        }
    };
}