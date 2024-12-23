#pragma once

#include "pch.h"
#include "render/render_base.h"
#include "vertex_holder.h"
#include "elems/element.h"
#include <assimp/scene.h>

namespace nelems
{
  class Mesh : public Element
  {
    
  public:

    Mesh() = default;

    virtual ~Mesh();

    void load(aiMesh* ai_mesh);

    void add_vertex(const VertexHolder& vertex) { mVertices.push_back(vertex);  }

    void add_vertex_index(unsigned int vertex_idx) { mVertexIndices.push_back(vertex_idx); }

    std::vector<unsigned int> get_vertex_indices() { return mVertexIndices; }

    void init();

    void create_buffers();

    void delete_buffers();

    void render();

    void bind();

    void unbind();

    void update(nshaders::Shader* shader) override{  }

  private:
    
    // Buffers manager
    std::unique_ptr<nrender::VertexIndexBuffer> mRenderBufferMgr;
    
    // Vertices and indices
    std::vector<VertexHolder> mVertices;
    std::vector<unsigned int> mVertexIndices;


  };
}

