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

        Model();

        void Draw(nshaders::Shader& shader);

        void load(std::string const& path);

    private:
        
    };
}