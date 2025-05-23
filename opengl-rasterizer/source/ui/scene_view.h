#pragma once

#include "elems/camera.h"
#include "elems/mesh.h"
#include "elems/model.h"
#include "elems/light.h"
#include "elems/ibl.h"
#include "shader/shader_util.h"
#include "render/opengl_buffer_manager.h"
#include "elems/input.h"

namespace nui
{
  class SceneView
  {
  public:
    SceneView() : 
      mCamera(nullptr), mFrameBuffer(nullptr), mShader(nullptr),
      mLight(nullptr), mSceneEnvIBL(nullptr), mSize(800, 600)
    {

      mFrameBuffer = std::make_unique<nrender::OpenGL_FrameBuffer>();
      mFrameBuffer->create_buffers(800, 600);
      mShader = std::make_unique<nshaders::Shader>();
      mShader->load("shaders/pbr.vs", "shaders/pbr.fs");
      mLight = std::make_unique<nelems::Light>();
      mSceneEnvIBL = std::make_unique<nelems::IBL>();
      mCamera = std::make_unique<nelems::Camera>(glm::vec3(0, 0, 3), 45.0f, 1.3f, 0.1f, 100.0f);

      this->load_starting_scene();
    }

    ~SceneView()
    {
      mShader->unload();
    }

    nelems::Light* get_light() { return mLight.get(); }

    void resize(int32_t width, int32_t height);


    void render();

    void load_model(const std::string& filepath);
    void load_texture(const std::string& filepath, const std::string& texture_name);
    void load_ibl(const std::string& ibl_skybox_filepath, const std::string& ibl_irradiance_filepath, const std::string& ibl_radiance_filepath);

    void load_starting_scene();

    void set_model(std::shared_ptr<nelems::Model> mesh)
    {
      mModel = mesh;
    }

    std::shared_ptr<nelems::Model> get_model() { return mModel; }
    std::shared_ptr<nshaders::Shader> get_shader() { return mShader; }

    
    void on_mouse_move(double x, double y, nelems::EInputButton button);

    void on_mouse_wheel(double delta);

    void reset_view()
    {
      mCamera->reset();
    }

  private:
    std::unique_ptr<nelems::Camera> mCamera;
    std::unique_ptr<nrender::OpenGL_FrameBuffer> mFrameBuffer;
    std::unique_ptr<nelems::IBL> mSceneEnvIBL;
    std::unique_ptr<nelems::Light> mLight;
    std::shared_ptr<nshaders::Shader> mShader;
    std::shared_ptr<nelems::Model> mModel;
    glm::vec2 mSize;
  };
}

