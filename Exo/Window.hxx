// Copyright AStarship <https://astarship.net>.
#pragma once
// AWindow / ATexture implementation. SDL3 + OpenGL 3.3 core: the window owns
// the context, and the texture blit uses one shader and one vertex array.
#include "Window.h"

namespace CTK {
namespace Exo {

BOL AWindow::Init(ISC width, ISC height, const CHA* title) {
  width_ = width;
  height_ = height;
  if (!SDL_Init(SDL_INIT_VIDEO)) return false;
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
    SDL_GL_DestroyContext(gl_context_);
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
  if (vbo_ != 0) {
    glDeleteBuffers(1, &vbo_);
    vbo_ = 0;
  }
  if (vao_ != 0) {
    glDeleteVertexArrays(1, &vao_);
    vao_ = 0;
  }
  if (program_ != 0) {
    glDeleteProgram(program_);
    program_ = 0;
  }
  width_ = height_ = 0;
}

/* --- presentation quad -------------------------------------------------------
The window path is this quad and nothing else: the app rasterizes its Room
canvas on the CPU and the texture blits it. Positions arrive in the unit
square with the image's top-left at (0, 0); the uniforms map that square onto
a pixel rectangle inside the viewport, flipping y so image row 0 is the top
row of the window. */

namespace {

const CHA* const ATextureVertexShader =
    "#version 330 core\n"
    "layout(location = 0) in vec2 a_pos;\n"
    "uniform vec2 u_scale;\n"
    "uniform vec2 u_offset;\n"
    "out vec2 v_uv;\n"
    "void main() {\n"
    "  v_uv = a_pos;\n"
    "  gl_Position = vec4(a_pos * u_scale + u_offset, 0.0, 1.0);\n"
    "}\n";

const CHA* const ATextureFragmentShader =
    "#version 330 core\n"
    "in vec2 v_uv;\n"
    "uniform sampler2D u_tex;\n"
    "out vec4 o_color;\n"
    "void main() { o_color = texture(u_tex, v_uv); }\n";

/* GL object names and enums are 32-bit; IUC keeps them from being truncated
by an 8-bit handle type. */
IUC CompileShader(IUC type, const CHA* source) {
  IUC shader = glCreateShader(type);
  if (shader == 0) return 0;
  const GLchar* text = (const GLchar*)source;
  glShaderSource(shader, 1, &text, nullptr);
  glCompileShader(shader);
  ISC compiled = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

}  //< namespace

BOL ATexture::InitProgram_() {
  if (program_ != 0) return true;
  IUC vertex = CompileShader(GL_VERTEX_SHADER, ATextureVertexShader);
  IUC fragment = CompileShader(GL_FRAGMENT_SHADER, ATextureFragmentShader);
  if (vertex == 0 || fragment == 0) {
    if (vertex != 0) glDeleteShader(vertex);
    if (fragment != 0) glDeleteShader(fragment);
    return false;
  }
  IUC program = glCreateProgram();
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  ISC linked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &linked);
  if (!linked) {
    glDeleteProgram(program);
    return false;
  }
  program_ = program;
  /* Unit quad as a triangle strip: (0,0) (1,0) (0,1) (1,1), image top-left
  first, so the texture's first row lands on the top row of the target. */
  const FPC quad[8] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};
  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);
  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(FPC), nullptr);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return true;
}

void ATexture::Draw(ISC target_width, ISC target_height, ISC x, ISC y, ISC w,
                    ISC h) {
  if (id_ == 0 || w <= 0 || h <= 0 || target_width <= 0 || target_height <= 0)
    return;
  if (!InitProgram_()) return;
  FPC scale_x = 2.0f * FPC(w) / FPC(target_width);
  FPC scale_y = 2.0f * FPC(h) / FPC(target_height);
  FPC offset_x = 2.0f * FPC(x) / FPC(target_width) - 1.0f;
  FPC offset_y = 1.0f - 2.0f * FPC(y) / FPC(target_height);
  glUseProgram(program_);
  glUniform2f(glGetUniformLocation(program_, "u_scale"), scale_x, -scale_y);
  glUniform2f(glGetUniformLocation(program_, "u_offset"), offset_x, offset_y);
  glUniform1i(glGetUniformLocation(program_, "u_tex"), 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, id_);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBindVertexArray(vao_);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
  glUseProgram(0);
}

}  //< namespace Exo
}  //< namespace CTK
