#include "pch.h"
#include "scene_view.h"

#include "imgui.h"

namespace nui
{

  void SceneView::resize(int32_t width, int32_t height)
  {
    mSize.x = width;
    mSize.y = height;

    mFrameBuffer->create_buffers((int32_t)mSize.x, (int32_t) mSize.y);
  }

  void SceneView::on_mouse_move(double x, double y, nelems::EInputButton button)
  {
    mCamera->on_mouse_move(x, y, button);
  }

  void SceneView::on_mouse_wheel(double delta)
  {
    mCamera->on_mouse_wheel(delta);
  }

  void SceneView::load_model(const std::string& filepath)
  {
      std::cout << "Loading model at" << filepath << std::endl;
      if (!mModel)
          mModel = std::make_shared<nelems::Model>();

      mModel->load(filepath);
  }

  void SceneView::load_texture(const std::string& filepath, const std::string& texture_name)
  {
      std::cout << "Loading texture at" << filepath << std::endl;
      mShader->set_tex(filepath, texture_name);
  }

  void SceneView::load_ibl(const std::string& ibl_skybox_filepath, const std::string& ibl_irradiance_filepath, const std::string& ibl_radiance_filepath)
  {
      std::cout << "Loading IBL at" << ibl_skybox_filepath << std::endl;

      mShader->set_tex_hdr(ibl_irradiance_filepath, "irradianceTex");
      mShader->set_tex_hdr(ibl_radiance_filepath, "radianceTex");
      nshaders::Shader* mSkyboxShader = mSceneEnvIBL->get_shader();
      mSkyboxShader->set_tex_hdr(ibl_skybox_filepath, "skyboxTex");
  }

  void SceneView::load_starting_scene()
  {
      //TODO im sure these make more sense somewhere else
      //AKA lots of hardcoded resource loading for testing
      std::string model_filepath = "resources/damaged_helmet/DamagedHelmet.fbx";
      std::string albedo_filepath = "resources/damaged_helmet/Default_albedo.jpg";
      std::string orm_filepath = "resources/damaged_helmet/Default_ORM.png";
      std::string emissive_filepath = "resources/damaged_helmet/Default_emissive.jpg";
      std::string normal_filepath = "resources/damaged_helmet/Default_normal.jpg";

      std::string ibl_skybox_filepath = "resources/ibl/warm_restaurant/output_skybox.hdr";
      std::string ibl_irradiance_filepath = "resources/ibl/warm_restaurant/output_iem.hdr";
      std::string ibl_radiance_filepath = "resources/ibl/warm_restaurant/output_pmrem.hdr";
      
      this->load_model(model_filepath);
      
      mShader->use();
      mShader->set_tex(albedo_filepath, "albedoTex");
      mShader->set_tex(orm_filepath, "ormTex");
      mShader->set_tex(emissive_filepath, "emissiveTex");
      mShader->set_tex(normal_filepath, "normalTex");
      mShader->set_tex_hdr(ibl_irradiance_filepath, "irradianceTex");
      mShader->set_tex_hdr(ibl_radiance_filepath, "radianceTex");

      nshaders::Shader* mSkyboxShader = mSceneEnvIBL->get_shader();
      mSkyboxShader->set_tex_hdr(ibl_skybox_filepath, "skyboxTex");
  }

  void SceneView::render()
  {

    mShader->use();

    mLight->update(mShader.get());

    mFrameBuffer->bind();

    if (mModel)
    {
       mModel->update(mShader.get());
       mModel->render();
    }

    mSceneEnvIBL->render();

    mFrameBuffer->unbind();

    ImGui::Begin("Scene");

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    mSize = { viewportPanelSize.x, viewportPanelSize.y };

    mCamera->set_aspect(mSize.x / mSize.y);
    
    mCamera->update(mShader.get(), mSceneEnvIBL->get_shader());

    // add rendered texture to ImGUI scene window
    uint64_t textureID = mFrameBuffer->get_texture();
    ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ mSize.x, mSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    ImGui::End();
  }
}
