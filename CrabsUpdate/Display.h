// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_TOOLKIT_UPDATE_DISPLAY_H
#define CRABS_TOOLKIT_UPDATE_DISPLAY_H

#include "ASCIITypes.h"

#include <SDL3/SDL.h>

namespace CT {

/* A pixel in the framebuffer: 8-bit RGBA. */
struct Pixel {
  IUA r, g, b, a;
};

/* A contiguous framebuffer: one heap allocation, no heap vector. */
class Framebuffer {
 public:
  Framebuffer() : width_(0), height_(0), pixels_(nullptr) {}

  ~Framebuffer() { Free(); }

  Framebuffer(const Framebuffer&) = delete;
  Framebuffer& operator=(const Framebuffer&) = delete;

  Framebuffer(Framebuffer&& other) noexcept :
      width_(other.width_), height_(other.height_),
      pixels_(other.pixels_) {
    other.width_ = other.height_ = 0;
    other.pixels_ = nullptr;
  }

  /* Allocates the framebuffer. Returns false on failure. */
  BOL Init(IUC width, IUC height) {
    Free();
    width_ = width;
    height_ = height;
    pixels_ = static_cast<Pixel*>(SDL_malloc(width * height * sizeof(Pixel)));
    if (!pixels_) {
      width_ = height_ = 0;
      return false;
    }
    Clear(0, 0, 0, 255);
    return true;
  }

  IUC Width() const { return width_; }
  IUC Height() const { return height_; }
  Pixel* Pixels() { return pixels_; }
  const Pixel* Pixels() const { return pixels_; }

  /* Sets a single pixel. */
  void SetPixel(IUC x, IUC y, IUA r, IUA g, IUA b, IUA a = 255) {
    if (x >= width_ || y >= height_) return;
    pixels_[y * width_ + x] = {r, g, b, a};
  }

  /* Clears the entire framebuffer. */
  void Clear(IUA r, IUA g, IUA b, IUA a = 255) {
    for (IUC i = 0; i < width_ * height_; ++i)
      pixels_[i] = {r, g, b, a};
  }

  /* Fills a rectangle. */
  void FillRect(IUC x, IUC y, IUC w, IUC h, IUA r, IUA g, IUA b, IUA a = 255) {
    for (IUC dy = 0; dy < h; ++dy) {
      for (IUC dx = 0; dx < w; ++dx) {
        SetPixel(x + dx, y + dy, r, g, b, a);
      }
    }
  }

  /* Draws a horizontal line. */
  void HLine(IUC x, IUC y, IUC w, IUA r, IUA g, IUA b, IUA a = 255) {
    FillRect(x, y, w, 1, r, g, b, a);
  }

  /* Draws a vertical line. */
  void VLine(IUC x, IUC y, IUC h, IUA r, IUA g, IUA b, IUA a = 255) {
    FillRect(x, y, 1, h, r, g, b, a);
  }

  /* Copies the framebuffer to an SDL texture. */
  void BlitToTexture(SDL_Texture* texture) const {
    SDL_UpdateTexture(texture, nullptr, pixels_,
                      static_cast<int>(width_ * sizeof(Pixel)));
  }

 private:
  void Free() {
    if (pixels_) {
      SDL_free(pixels_);
      pixels_ = nullptr;
    }
    width_ = height_ = 0;
  }

  IUC width_;
  IUC height_;
  Pixel* pixels_;
};

/* An SDL3 display: window + renderer + texture + framebuffer. */
class Display {
 public:
  Display() :
      window_(nullptr), renderer_(nullptr), texture_(nullptr) {}

  ~Display() { Shutdown(); }

  Display(const Display&) = delete;
  Display& operator=(const Display&) = delete;

  /* Initializes the display. Returns false on failure. */
  BOL Init(IUC width, IUC height, const char* title) {
    if (!SDL_Init(SDL_INIT_VIDEO)) return false;
    window_ = SDL_CreateWindow(title, static_cast<int>(width),
                               static_cast<int>(height), 0);
    if (!window_) {
      SDL_Quit();
      return false;
    }
    renderer_ = SDL_CreateRenderer(window_, nullptr);
    if (!renderer_) {
      SDL_DestroyWindow(window_);
      SDL_Quit();
      return false;
    }
    texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888,
                                 SDL_TEXTUREACCESS_STREAMING,
                                 static_cast<int>(width),
                                 static_cast<int>(height));
    if (!texture_) {
      SDL_DestroyRenderer(renderer_);
      SDL_DestroyWindow(window_);
      SDL_Quit();
      return false;
    }
    if (!framebuffer_.Init(width, height)) {
      SDL_DestroyTexture(texture_);
      SDL_DestroyRenderer(renderer_);
      SDL_DestroyWindow(window_);
      SDL_Quit();
      return false;
    }
    return true;
  }

  /* Renders the framebuffer to the screen. */
  void Present() {
    framebuffer_.BlitToTexture(texture_);
    SDL_RenderClear(renderer_);
    SDL_RenderTexture(renderer_, texture_, nullptr, nullptr);
    SDL_RenderPresent(renderer_);
  }

  /* Polls one event. Returns the event. */
  SDL_Event PollEvent() {
    SDL_Event event;
    SDL_WaitEventTimeout(&event, 16);  // ~60 FPS.
    return event;
  }

  /* The framebuffer. */
  Framebuffer& FB() { return framebuffer_; }

  /* The window. */
  SDL_Window* Window() { return window_; }
  SDL_Renderer* Renderer() { return renderer_; }

  /* Shuts down the display. */
  void Shutdown() {
    if (texture_) SDL_DestroyTexture(texture_);
    if (renderer_) SDL_DestroyRenderer(renderer_);
    if (window_) SDL_DestroyWindow(window_);
    SDL_Quit();
    texture_ = nullptr;
    renderer_ = nullptr;
    window_ = nullptr;
  }

 private:
  SDL_Window* window_;
  SDL_Renderer* renderer_;
  SDL_Texture* texture_;
  Framebuffer framebuffer_;
};

}  // namespace CT
#endif
