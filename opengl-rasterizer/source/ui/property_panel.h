#pragma once

#include "elems/light.h"
#include "ui/scene_view.h"

#include "imgui.h"
#include "utils/imgui_widgets.h"
#include <ImFileBrowser.h>

namespace nui
{
  class Property_Panel
  {
  public:

    Property_Panel()
    {
      mCurrentFile = "< ... >";
      mTexCurrentFile = "< ... >";

      mFileDialog.SetTitle("Open mesh");
      mFileDialog.SetFileFilters({ ".fbx", ".obj",".gltf"});

      mTexFileDialog.SetTitle("Open texture");
    }

    void render(nui::SceneView* mScene);

    void set_mesh_load_callback(const std::function<void(const std::string&)>& callback)
    {
      mMeshLoadCallback = callback;
    }

    void set_texture_load_callback(const std::function<void(const std::string&, const std::string&)>& callback)
    {
        mTexLoadCallback = callback;
    }

  private:
    // create a file browser instance
    ImGui::FileBrowser mFileDialog;
    ImGui::FileBrowser mTexFileDialog;

    std::function<void(const std::string&)> mMeshLoadCallback;
    std::function<void(const std::string&, const std::string&)> mTexLoadCallback;

    std::string mCurrentFile;
    std::string mTexCurrentFile;
    bool mDiffuseIBLEnabled = true;
    bool mSpecularIBLEnabled = true;
    bool mNormalEnabled = true;
  };
}

