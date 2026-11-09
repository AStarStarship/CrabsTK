// Copyright AStarship <https://astarship.net>.
// AWindow / ATexture implementation. SDL3 + legacy OpenGL (works over
// Xvfb / llvmpipe / Mesa / NVIDIA alike; the GL calls are GL 1.1 core,
// which every driver implements).
#include "Window.h"

namespace CTK {
namespace GUI {

BOL AWindow::Init(ISC width, ISC height, const CHA* title) {
  width_ = width;
  height_ = height;
  if (SDL_Init(SDL_INIT_VIDEO) != 0) return false;
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  window_ = SDL_CreateWindow(title, width, height,
                             SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (window_ == nullptr) return false;
  gl_context_ = SDL_GL_CreateContext(window_);
  if (gl_context_ == nullptr) {
    SDL_DestroyWindow(window_);
    window_ = nullptr;
    return false;
  }
  SDL_GL_SetSwapInterval(1);
  open_ = true;
  return true;
}

void AWindow::BeginFrame() {
  /* Keep width/height in sync with the (possibly resized) window. */
  SDL_GetWindowSize(window_, &width_, &height_);
  SDL_GL_MakeCurrent(window_, gl_context_);
}

void AWindow::EndFrame(IUB r, IUB g, IUB b, IUB a) {
  glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
  glViewport(0, 0, width_, height_);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void AWindow::Swap() {
  SDL_GL_SwapWindow(window_);
}

BOL AWindow::Poll() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_EVENT_QUIT) return false;
    if (e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) return false;
  }
  return true;
}

void AWindow::Close() {
  if (window_ != nullptr) {
    SDL_GL_DeleteContext(gl_context_);
    SDL_DestroyWindow(window_);
    window_ = nullptr;
    gl_context_ = nullptr;
  }
  SDL_Quit();
  open_ = false;
}

/* --- ATexture ---------------------------------------------------------------- */
ATexture::ATexture() = default;

ATexture::~ATexture() {
  Release();
}

void ATexture::Upload(CTK::Exo::CImage& img) {
  if (id_ != 0) {
    glDeleteTextures(1, &id_);
    id_ = 0;
  }
  glGenTextures(1, &id_);
  glBindTexture(GL_TEXTURE_2D, id_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, img.data);
  width_ = img.width;
  height_ = img.height;
}

void ATexture::Release() {
  if (id_ != 0) {
    glDeleteTextures(1, &id_);
    id_ = 0;
  }
}

}  //< namespace GUI
}  //< namespace CTK
