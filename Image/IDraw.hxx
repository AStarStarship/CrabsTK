// Copyright AStarship <https://astarship.net>.
#include "IDraw.h"

namespace _ {
namespace ImageDraw {

IUN PackColor(const TColor& c) {
  return (IUN)c.a | ((IUN)c.b << 8) | ((IUN)c.g << 16) | ((IUN)c.r << 24);
}

/* Alpha blend the source color over the destination pixel. */
static void BlendPixel(CImage& img, ISN x, ISN y, const TColor& src) {
  if (img.data == nullptr) return;
  if (x < 0 || y < 0 || x >= img.width || y >= img.height) return;
  if (img.channels != 4) return;  // blending defined for RGBA.
  ISN idx = (y * img.width + x) * 4;
  IUA dr = img.data[idx];
  IUA dg = img.data[idx + 1];
  IUA db = img.data[idx + 2];
  IUA da = img.data[idx + 3];
  IUA sa = src.a;
  if (sa == 0xff) {
    img.data[idx] = src.r;
    img.data[idx + 1] = src.g;
    img.data[idx + 2] = src.b;
    img.data[idx + 3] = 0xff;
    return;
  }
  if (sa == 0 || da == 0) return;
  // Standard "over" blend, integer math.
  IUA oa = (IUA)(sa + da - (IUN)sa * da / 255);
  IUA nr = (IUA)(((IUN)src.r * sa + (IUN)dr * da * (255 - sa)) / oa);
  IUA ng = (IUA)(((IUN)src.g * sa + (IUN)dg * da * (255 - sa)) / oa);
  IUA nb = (IUA)(((IUN)src.b * sa + (IUN)db * da * (255 - sa)) / oa);
  img.data[idx] = nr;
  img.data[idx + 1] = ng;
  img.data[idx + 2] = nb;
  img.data[idx + 3] = oa;
}

void Fill(CImage& img, const TColor& c) {
  if (img.data == nullptr || img.channels != 4) return;
  IUN px = (IUN)img.width * (IUN)img.height;
  for (IUN i = 0; i < px; ++i) {
    img.data[i * 4] = c.r;
    img.data[i * 4 + 1] = c.g;
    img.data[i * 4 + 2] = c.b;
    img.data[i * 4 + 3] = c.a;
  }
}

void Clear(CImage& img) {
  if (img.data == nullptr) return;
  IUN px = (IUN)img.width * (IUN)img.height;
  IUN bytes = px * 4;
  for (IUN i = 0; i < bytes; ++i) img.data[i] = 0;
}

void PutPixel(CImage& img, ISN x, ISN y, const TColor& c) {
  BlendPixel(img, x, y, c);
}

void Line(CImage& img, ISN x0, ISN y0, ISN x1, ISN y1, const TColor& c,
          ISN thickness) {
  ISN dx = x1 > x0 ? x1 - x0 : x0 - x1;
  ISN dy = y1 > y0 ? y1 - y0 : y0 - y1;
  ISN sx = x0 < x1 ? 1 : -1;
  ISN sy = y0 < y1 ? 1 : -1;
  ISN err = dx - dy;
  for (;;) {
    if (thickness <= 1) {
      PutPixel(img, x0, y0, c);
    } else {
      ISN h = thickness / 2;
      for (ISN oy = -h; oy <= h; ++oy)
        for (ISN ox = -h; ox <= h; ++ox) PutPixel(img, x0 + ox, y0 + oy, c);
    }
    if (x0 == x1 && y0 == y1) break;
    ISN e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y0 += sy;
    }
  }
}

void FillRect(CImage& img, ISN x0, ISN y0, ISN x1, ISN y1, const TColor& c) {
  if (img.data == nullptr) return;
  if (x0 > x1) {
    ISN t = x0; x0 = x1; x1 = t;
  }
  if (y0 > y1) {
    ISN t = y0; y0 = y1; y1 = t;
  }
  for (ISN y = y0; y <= y1; ++y)
    for (ISN x = x0; x <= x1; ++x) PutPixel(img, x, y, c);
}

void FillCircle(CImage& img, ISN cx, ISN cy, ISN r, const TColor& c) {
  if (r < 0) return;
  // Midpoint circle: step the radius in octants, drawing horizontal spans.
  ISN x = 0;
  ISN y = r;
  ISN d = 3 - 2 * r;
  while (x <= y) {
    Line(img, cx - x, cy + y, cx + x, cy + y, c, 1);
    Line(img, cx - y, cy + x, cx + y, cy + x, c, 1);
    Line(img, cx - y, cy - x, cx + y, cy - x, c, 1);
    Line(img, cx - x, cy - y, cx + x, cy - y, c, 1);
    if (d < 0) {
      d += 4 * x + 6;
    } else {
      d += 4 * (x - y) + 10;
      --y;
    }
    ++x;
  }
}

BOL ClipRect(CImage& img, TRect* r) {
  if (r == nullptr) return false;
  if (r->x0 < 0) r->x0 = 0;
  if (r->y0 < 0) r->y0 = 0;
  if (r->x1 >= img.width) r->x1 = img.width - 1;
  if (r->y1 >= img.height) r->y1 = img.height - 1;
  if (r->x0 > r->x1 || r->y0 > r->y1) return false;
  return true;
}

}  // namespace ImageDraw
}  // namespace _
