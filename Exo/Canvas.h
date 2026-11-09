// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKGUIImageCanvasH
#define _CrabsTKGUIImageCanvasH
#include <_Config.h>
#include "../Exo/Image.h"
#include "../Exo/IDraw.h"
#include "VG.h"

namespace CTK {
namespace GUI {

/* Backend state handed to the Exo::RenderParams vtable as userPtr. It must
  outlive the per-frame render callbacks and delete, so it lives with the
  canvas as a member rather than a constructor local. */
struct SHeadless {
  Exo::CImage* img;
  IUA* pixels;
  ISC w, h;
};

/* A vector canvas. Construct headless (it owns a CImage) or over an existing
GL context. The path is built with BeginPath/MoveTo/LineTo/Circle then
committed with Fill/Stroke. */
class AVCanvas {
 public:
  /* Headless: create a WxH RGBA8 canvas. */
  AVCanvas(ISC width, ISC height, Exo::VColor bg = Exo::IWhite);
  ~AVCanvas();

  /* The rasterized canvas (headless backend). */
  Exo::CImage& Image() { return img_; }
  const Exo::CImage& Image() const { return img_; }

  /* One drawing pass: BeginFrame -> paths -> EndFrame (flushes to the image
  or the GL context). */
  void BeginFrame();
  void EndFrame();

  // Path API
  void BeginPath();
  void MoveTo(FPD x, FPD y);
  void LineTo(FPD x, FPD y);
  void Circle(FPD cx, FPD cy, FPD r);
  void ClosePath();
  /* Fill or stroke the current path with the given colour. */
  void Fill(Exo::VColor c);
  void Stroke(Exo::VColor c, FPD width);
  /* Fill the whole canvas with a colour (background). */
  void Clear(Exo::VColor c);

 private:
  Exo::RenderContext* ctx_ = nullptr;
  Exo::AImage owned_;   //< one Autoject block: [ISW bytes | RGBA pixels]
  Exo::CImage img_;     //< view of owned_, handed to the renderer
  SHeadless headless_;  //< backend state for the vtable; outlives ctx_
  ISC width_ = 0, height_ = 0;
};

}  //< namespace GUI
}  //< namespace CTK
#endif  //< _CrabsTKGUIImageCanvasH
