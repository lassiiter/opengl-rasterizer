#include "pch.h"
#include "shader/shader_util.h"
#include "elems/element.h"
#include "vertex_holder.h"
#include "render/opengl_buffer_manager.h"

namespace nelems
{
    class IBL : public Element
    {
    public:
        IBL()
        {
            mSkyboxShader = std::make_unique<nshaders::Shader>();
            mSkyboxShader->load("shaders/ibl.vs", "shaders/ibl.fs");

            mRenderBufferMgr = std::make_unique<nrender::OpenGL_VertexArrayBuffer>();
        }
        void render();
        
        nshaders::Shader* get_shader()
        {
            return mSkyboxShader.get();
        }

    private:
        std::unique_ptr<nshaders::Shader> mSkyboxShader;

        // Buffers manager
        std::unique_ptr<nrender::VertexArrayBuffer> mRenderBufferMgr;
    };

}