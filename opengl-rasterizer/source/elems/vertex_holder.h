#pragma once

namespace nelems
{
  class VertexHolder
  {

  public:
    VertexHolder() : mPos(), mNormal(), mTexCoords() {}
    
    // Initialize by string to be tokenized
    // TODO: Better handle outside of this class
    VertexHolder(const std::vector<std::string> tokens)
    {
    }

    VertexHolder(const glm::vec3& pos, const glm::vec3& normal, const glm::vec2& uvs)
      : mPos(pos), mNormal(normal), mTexCoords(uvs)
    {
    }

    ~VertexHolder() = default;


    glm::vec3 mPos;

    glm::vec3 mNormal;

    glm::vec2 mTexCoords;
  };
}

