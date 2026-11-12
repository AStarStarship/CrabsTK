// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CrabsTKExoIDrawH
#define CrabsTKExoIDrawH
#include "Image.h"
namespace CTK {
namespace Exo {

/* Raster draw primitives that render into a CImage buffer. They live beside
the image types in CTK::Exo — one namespace owns the whole raster layer. All
ops are alpha-blended and clip out-of-bounds writes. */

/* A packed RGBA color. */
struct VColor {
  IUB r;
  IUB g;
  IUB b;
  IUB a;
};

inline VColor MakeColor(IUB r, IUB g, IUB b, IUB a = 0xff) {
  return VColor{r, g, b, a};
}

/* Predefined palette (inline so each TU gets its own copy, no ODR issue). */
inline const VColor IWhite = VColor{255, 255, 255, 255};
inline const VColor IBlack = VColor{0, 0, 0, 255};
inline const VColor IRed = VColor{255, 0, 0, 255};
inline const VColor IGreen = VColor{0, 255, 0, 255};
inline const VColor IBlue = VColor{0, 0, 255, 255};
inline const VColor IYellow = VColor{255, 255, 0, 255};
inline const VColor ICyan = VColor{0, 255, 255, 255};
inline const VColor IMagenta = VColor{255, 0, 255, 255};
inline const VColor IGray = VColor{128, 128, 128, 255};
inline const VColor ISlateGray = VColor{112, 128, 144, 255};
inline const VColor IViolet = VColor{124, 77, 255, 255};
inline const VColor IAmber = VColor{255, 191, 0, 255};
inline const VColor ISteel = VColor{70, 130, 180, 255};
inline const VColor ITransparent = VColor{0, 0, 0, 0};

/* Pack a color to a little-endian RGBA pixel (A|B<<8|G<<16|R<<24). */
IUC PackColor(const VColor& c);

/* Fill the whole image with a solid color. */
void Fill(CImage& img, const VColor& c);

/* Clear the image to transparent black. */
void Clear(CImage& img);

/* Put a single pixel (alpha blended over the destination). Clipped. */
void PutPixel(CImage& img, ISC x, ISC y, const VColor& c);

/* Bresenham line, alpha blended. (x0,y0)-(x1,y1) inclusive. */
void Line(CImage& img, ISC x0, ISC y0, ISC x1, ISC y1, const VColor& c,
          ISC thickness = 1);

/* Filled axis-aligned rectangle [x0..x1]x[y0..y1], alpha blended. */
void FillRect(CImage& img, ISC x0, ISC y0, ISC x1, ISC y1, const VColor& c);

/* Filled circle (midpoint), center (cx,cy), radius r, alpha blended. */
void FillCircle(CImage& img, ISC cx, ISC cy, ISC r, const VColor& c);

/* Bounding box of an axis-aligned rectangle (for clip math). */
struct RectangleISC {
  ISC x0;
  ISC y0;
  ISC x1;
  ISC y1;
};

/* Clip a rectangle against the image bounds. Returns false if empty. */
BOL ClipRect(CImage& img, RectangleISC* r);

}       //< namespace Exo
}       //< namespace CTK
#endif  //< CrabsTKExoIDrawH
