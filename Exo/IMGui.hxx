// Copyright AStarship <https://astarship.net>.
// IMGui implementation over the vendored backends.
#include "IMGui.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
namespace CTK {
namespace GUI {
using namespace ::_;

BOL IMGui::ready_ = false;

BOL IMGui::Init(AWindow& window) {
  if (ready_) return true;
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui::StyleColorsDark();
  if (!ImGui_ImplSDL3_InitForOpenGL(window.SdlWindow(), nullptr))
    return false;
  if (!ImGui_ImplOpenGL3_Init("#version 130"))
    return false;
  ready_ = true;
  return true;
}

void IMGui::Shutdown() {
  if (!ready_) return;
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();
  ready_ = false;
}

void IMGui::NewFrame(AWindow& window) {
  ImGui_ImplSDL3_NewFrame();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui::NewFrame();
}

void IMGui::Render(AWindow& window) {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

BOL IMGui::Ready() { return ready_; }

}  //< namespace GUI
}  //< namespace CTK
