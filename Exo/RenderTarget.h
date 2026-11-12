// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKGUIImageRenderTargetH
#define _CrabsTKGUIImageRenderTargetH
#include <_Config.h>
#include "../Exo/Image.h"

namespace CTK {
namespace Exo {

/* A CPU raster target. Mirrors the AWindow/ATexture frame discipline
 * (BeginFrame -> draw -> EndFrame) without any GL. The canvas is an AImage:
 * one Autoject block allocated through a RAMFactory and released by the
 * destructor. */
class ARenderTarget {
 public:
  ARenderTarget(ISC width, ISC height,
                CTK::Exo::VColor bg = {30, 30, 30, 255});
  ~ARenderTarget();

  ISC Width() { return width_; }
  ISC Height() { return height_; }
  /* The canvas (row-major RGBA, top-left origin). The buffer is owned by
   * canvas_; this hands out a view. */
  CTK::Exo::CImage& Canvas() { return canvas_.View(); }
  const CTK::Exo::CImage& Canvas() const { return canvas_.View(); }

  /* Frame discipline. BeginFrame clears to the background; the caller
   * draws (Plot into an overlay Figure, blit it in, draw primitives);
   * EndFrame is a no-op hook kept for API symmetry with AWindow. */
  void BeginFrame();
  void EndFrame();

  /* Blit a whole CImage into the canvas at (px, py). Clamps at the edge. */
  void Blit(CTK::Exo::CImage& src, ISC px, ISC py);
  /* Draw a filled rectangle. */
  void FillRect(ISC x0, ISC y0, ISC x1, ISC y1,
                CTK::Exo::VColor c);

 private:
  ISC width_;
  ISC height_;
  CTK::Exo::AImage canvas_;
  CTK::Exo::VColor bg_;
};

}  //< namespace Exo
}  //< namespace CTK
#endif  //< _CrabsTKGUIImageRenderTargetH
