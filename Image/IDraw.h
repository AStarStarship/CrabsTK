// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_IMAGE_IDRAW_DECL
#define CRABSTK_IMAGE_IDRAW_DECL
#include "Image.h"

namespace _ {

/* Raster draw primitives that render into a CImage buffer. Grouped in the
   ImageDraw namespace (not "Image" — that name is taken by the Image class
   in Image.h). All ops are alpha-blended and clip out-of-bounds writes. */
namespace ImageDraw {

/* A packed RGBA color. */
struct TColor {
  IUB r;
  IUB g;
  IUB b;
  IUB a;
};

inline TColor MakeColor(IUB r, IUB g, IUB b, IUB a = 0xff) {
  return TColor{r, g, b, a};
}

/* Predefined palette (inline so each TU gets its own copy, no ODR issue). */
inline const TColor IWhite = TColor{255, 255, 255, 255};
inline const TColor IBlack = TColor{0, 0, 0, 255};
inline const TColor IRed = TColor{255, 0, 0, 255};
inline const TColor IGreen = TColor{0, 255, 0, 255};
inline const TColor IBlue = TColor{0, 0, 255, 255};
inline const TColor IYellow = TColor{255, 255, 0, 255};
inline const TColor ICyan = TColor{0, 255, 255, 255};
inline const TColor IMagenta = TColor{255, 0, 255, 255};
inline const TColor IGray = TColor{128, 128, 128, 255};
inline const TColor ITransparent = TColor{0, 0, 0, 0};

/* Pack a color to a little-endian RGBA pixel (A|B<<8|G<<16|R<<24). */
IUN PackColor(const TColor& c);

/* Fill the whole image with a solid color. */
void Fill(CImage& img, const TColor& c);

/* Clear the image to transparent black. */
void Clear(CImage& img);

/* Put a single pixel (alpha blended over the destination). Clipped. */
void PutPixel(CImage& img, ISN x, ISN y, const TColor& c);

/* Bresenham line, alpha blended. (x0,y0)-(x1,y1) inclusive. */
void Line(CImage& img, ISN x0, ISN y0, ISN x1, ISN y1, const TColor& c,
          ISN thickness = 1);

/* Filled axis-aligned rectangle [x0..x1]x[y0..y1], alpha blended. */
void FillRect(CImage& img, ISN x0, ISN y0, ISN x1, ISN y1, const TColor& c);

/* Filled circle (midpoint), center (cx,cy), radius r, alpha blended. */
void FillCircle(CImage& img, ISN cx, ISN cy, ISN r, const TColor& c);

/* Bounding box of an axis-aligned rectangle (for clip math). */
struct TRect {
  ISN x0;
  ISN y0;
  ISN x1;
  ISN y1;
};

/* Clip a rectangle against the image bounds. Returns false if empty. */
BOL ClipRect(CImage& img, TRect* r);

}  // namespace ImageDraw
}  // namespace _
#endif
