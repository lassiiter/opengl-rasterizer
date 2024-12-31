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
            mEquirectangularToCubemapShader = std::make_unique<nshaders::Shader>();
            mEquirectangularToCubemapShader->load("shaders/cubemap.vs", "shaders/cubemap.fs");

            mIrradianceShader = std::make_unique<nshaders::Shader>();
            mIrradianceShader->load("shaders/cubemap.vs", "shaders/irradiance_convolution.fs");

            mBackgroundShader = std::make_unique<nshaders::Shader>();
            mBackgroundShader->load("shaders/background.vs", "shaders/background.fs");

            mRenderBufferMgr = std::make_unique<nrender::OpenGL_VertexArrayBuffer>();
        }
        void render();
        void load_background_texture(const std::string&);
        void render_on_cube();
        
        nshaders::Shader* get_shader()
        {
            return mBackgroundShader.get();
        }

    private:
        std::unique_ptr<nshaders::Shader> mEquirectangularToCubemapShader;
        std::unique_ptr<nshaders::Shader> mIrradianceShader;
        std::unique_ptr<nshaders::Shader> mBackgroundShader;
        // Buffers manager
        std::unique_ptr<nrender::VertexArrayBuffer> mRenderBufferMgr;
        unsigned int cubeVAO = 0;
        unsigned int cubeVBO = 0;
    };

}