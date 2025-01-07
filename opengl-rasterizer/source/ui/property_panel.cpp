#include "pch.h"
#include "property_panel.h"

namespace nui
{
  void Property_Panel::render(nui::SceneView* scene_view)
  {
    std::shared_ptr<nelems::Model> model = scene_view->get_model();

    ImGui::Begin("Properties");
    if (ImGui::CollapsingHeader("Mesh"))
    {
      if (ImGui::Button("Open..."))
      {
        mFileDialog.Open();
      }
      ImGui::SameLine(0, 5.0f);
      ImGui::Text(mCurrentFile.c_str());
    }

    if (ImGui::CollapsingHeader("Material") && model)
    {
      ImGui::ColorPicker3("Tint", (float*)&model->mColor, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);
      ImGui::SliderFloat("Roughness Scalar", &model->mRoughness, 0.0f, 10.0f);
      ImGui::SliderFloat("Metallic Scalar", &model->mMetallic, 0.0f, 10.0f);
      if (ImGui::Button("Albedo"))
      {
          mTexFileDialog.Open();
      }
      ImGui::SameLine(0, 5.0f);
      ImGui::Text(mTexCurrentFile.c_str());
      if (ImGui::Checkbox("Enable Normal Map", &mNormalEnabled)) {
          model->mNormalEnabled = mNormalEnabled ? 1.0 : 0.0;
      }
    }

    if (ImGui::CollapsingHeader("Lighting"))
    {
        if (ImGui::Button("Canary Wharf", ImVec2(210.,30.))) {
            std::string ibl_skybox_filepath = "resources/ibl/canary_wharf/output_skybox.hdr";
            std::string ibl_irradiance_filepath = "resources/ibl/canary_wharf/output_iem.hdr";
            std::string ibl_radiance_filepath = "resources/ibl/canary_wharf/output_pmrem.hdr";

            mIBLLoadCallback(ibl_skybox_filepath, ibl_irradiance_filepath, ibl_radiance_filepath);
        }
        if (ImGui::Button("Warm Restaurant", ImVec2(210., 30.))) {
            std::string ibl_skybox_filepath = "resources/ibl/warm_restaurant/output_skybox.hdr";
            std::string ibl_irradiance_filepath = "resources/ibl/warm_restaurant/output_iem.hdr";
            std::string ibl_radiance_filepath = "resources/ibl/warm_restaurant/output_pmrem.hdr";

            mIBLLoadCallback(ibl_skybox_filepath, ibl_irradiance_filepath, ibl_radiance_filepath);
        }
        if (ImGui::Button("Small Studio", ImVec2(210., 30.))) {
            std::string ibl_skybox_filepath = "resources/ibl/studio_small/output_skybox.hdr";
            std::string ibl_irradiance_filepath = "resources/ibl/studio_small/output_iem.hdr";
            std::string ibl_radiance_filepath = "resources/ibl/studio_small/output_pmrem.hdr";

            mIBLLoadCallback(ibl_skybox_filepath, ibl_irradiance_filepath, ibl_radiance_filepath);
        }
        if (ImGui::Button("Gym Entrace", ImVec2(210., 30.))) {
            std::string ibl_skybox_filepath = "resources/ibl/gym_entrace/output_skybox.hdr";
            std::string ibl_irradiance_filepath = "resources/ibl/gym_entrace/output_iem.hdr";
            std::string ibl_radiance_filepath = "resources/ibl/gym_entrace/output_pmrem.hdr";

            mIBLLoadCallback(ibl_skybox_filepath, ibl_irradiance_filepath, ibl_radiance_filepath);
        }
        if (ImGui::Button("Studio Garden", ImVec2(210., 30.))) {
            std::string ibl_skybox_filepath = "resources/ibl/studio_garden/output_skybox.hdr";
            std::string ibl_irradiance_filepath = "resources/ibl/studio_garden/output_iem.hdr";
            std::string ibl_radiance_filepath = "resources/ibl/studio_garden/output_pmrem.hdr";

            mIBLLoadCallback(ibl_skybox_filepath, ibl_irradiance_filepath, ibl_radiance_filepath);
        }

      ImGui::Separator();
      ImGui::Text("Direct Light Position");
      ImGui::Separator();
      nimgui::draw_vec3_widget("Position", scene_view->get_light()->mPosition, 80.0f);

      if (ImGui::Checkbox("Enable IBL Diffuse", &mDiffuseIBLEnabled)) {
          model->mDiffuseIBLEnabled = mDiffuseIBLEnabled ? 1.0 : 0.0;
      }
      if (ImGui::Checkbox("Enable IBL Specular", &mSpecularIBLEnabled)) {
          model->mSpecularIBLEnabled = mSpecularIBLEnabled ? 1.0 : 0.0;
      }
    }

    ImGui::End();

    mFileDialog.Display();
    if (mFileDialog.HasSelected())
    {
      auto file_path = mFileDialog.GetSelected().string();
      mCurrentFile = file_path.substr(file_path.find_last_of("/\\") + 1);

      mMeshLoadCallback(file_path);

      mFileDialog.ClearSelected();
    }
    mTexFileDialog.Display();

    if (mTexFileDialog.HasSelected())
    {
        auto file_path = mTexFileDialog.GetSelected().string();
        mTexCurrentFile = file_path.substr(file_path.find_last_of("/\\") + 1);

        mTexLoadCallback(file_path, "albedo");

        mTexFileDialog.ClearSelected();
    }

  }
}
