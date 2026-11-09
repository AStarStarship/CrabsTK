// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKGUIImageGuiH
#define _CrabsTKGUIImageGuiH
#include "Window.h"
namespace CTK {
namespace GUI {

/* Intermediate Mode Graphical User Interface ASCIICrabs style.
Owns the ImGui context and its two backends (SDL3, OpenGL3). Call Init
once after AWindow::Init, then NewFrame() at the top of each frame and
Render() after all ImGui drawing is done. */
class IMGui {
 public:
  /* Create the ImGui context and bind both backends to the window's GL
  context. @return false if backend init failed. */
  BOL Init(AWindow& window);

  /* Destroy the backends and the context. */
  void Shutdown();

  /* Top of every frame: pump events and start the new frame. */
  void NewFrame(AWindow& window);
  /* Bottom of every frame: render the ImGui draw data into the GL context.
   * Call after all ImGui:: calls and before AWindow::Swap(). */
  void Render(AWindow& window);

  /* True once Init has succeeded. */
  BOL Ready();

 private:
  BOL ready_;
};

}       //< namespace GUI
}       //< namespace CTK
#endif  //< _CrabsTKGUIImageGuiH
