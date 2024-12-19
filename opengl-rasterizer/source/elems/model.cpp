#include "pch.h"
#include "mesh.h"
#include "model.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <vector>
namespace nelems
{
    void Model::render()
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
        {
            std::cout << meshes.size() << std::endl;
            meshes[i]->render();
        }
    }

    void Model::load(std::string const& filepath)
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
        // process ASSIMP's root node recursively
        aiNode* node = pScene->mRootNode;
        for (unsigned int i = 0; i < pScene->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* ai_mesh = pScene->mMeshes[i];
            std::shared_ptr<nelems::Mesh> mesh = std::make_shared<nelems::Mesh>();
            mesh->load(ai_mesh);
            std::cout << "adding meshes here" << std::endl;
            meshes.push_back(mesh);
        }
    }
}