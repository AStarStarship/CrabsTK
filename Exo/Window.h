// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKGUIImageWindowH
#define _CrabsTKGUIImageWindowH
#include <_Config.h>
#include <SDL3/SDL.h>
#include "../Exo/Image.h"

namespace CTK {
namespace GUI {

/* An SDL3 window with an OpenGL 3.3 core context. Owns the window, the GL
context, and the swap interval. The context is current while the window
lives; callers issue GL calls between BeginFrame/EndFrame. */
class AWindow {
 public:
  /* Create the window and make its GL context current.
  @return false if the window or context could not be created. */
  BOL Init(ISC width, ISC height, const CHA* title);

  /* True while the window is open and the context is usable. */
  BOL Open() { return open_; }
  ISC Width() { return width_; }
  ISC Height() { return height_; }
  /* The native window surface (SDL_WindowID), for backend binding. */
  IUA Native() { return (IUA)SDL_GetWindowID(window_); }

  /* One frame boundary: pump events, mark the frame start. The caller
  renders (ImGui or raw GL) and then calls Swap(). */
  void BeginFrame();
  /* Clear to the given RGBA (0..255) and end the frame. */
  void EndFrame(IUB r, IUB g, IUB b, IUB a);
  /* Swap the buffers. */
  void Swap();
  /* Poll events; returns false when the user closed the window. */
  BOL Poll();
  /* Shut the window down and release the context. */
  void Close();

  /* The underlying SDL window (for backend init). */
  SDL_Window* SdlWindow() { return window_; }
  /* The GL context handle. */
  void* GlContext() { return gl_context_; }

 private:
  SDL_Window* window_ = nullptr;
  void* gl_context_ = nullptr;
  ISC width_ = 0, height_ = 0;
  BOL open_ = false;
};

/* A GL texture that mirrors a CImage (RGBA, row-major, top-left origin).
Call Upload() after the image is rasterized; the texture is ready for
ImGui::Image or raw GL draws until the next Upload. */
class ATexture {
 public:
  ATexture();
  
  ~ATexture();

  /* (Re)create the texture and upload the image's pixels. */
  void Upload(CTK::Exo::CImage& img);

  /* The GL texture id (0 if not yet uploaded). */
  IUA Id() { return id_; }

  ISC Width() { return width_; }

  ISC Height() { return height_; }

  /* Free the GPU resource. */
  void Release();

 private:
  IUA id_ = 0;
  ISC width_ = 0, height_ = 0;
};

}  //< namespace GUI
}  //< namespace CTK
#endif  //< _CrabsTKGUIImageWindowH
