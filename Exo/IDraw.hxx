// Copyright AStarship <https://astarship.net>.
#pragma once
#include "IDraw.h"
namespace CTK {
namespace Exo {

IUC PackColor(const VColor& c) {
  return (IUC)c.a | ((IUC)c.b << 8) | ((IUC)c.g << 16) | ((IUC)c.r << 24);
}

/* Alpha blend the source color over the destination pixel. */
void BlendPixel(CImage& img, ISC x, ISC y, const VColor& src) {
  if (img.data == nullptr) return;
  if (x < 0 || y < 0 || x >= img.width || y >= img.height) return;
  if (img.format != PixelFormat::RGBA8) return;  // blending defined for RGBA8.
  ISC idx = (y * img.width + x) * 4;
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
  IUA oa = IUA(sa + da - IUC(sa) * da / 255);
  IUA nr = IUA((IUC(src.r) * sa + IUC(dr) * da * (255 - sa)) / oa);
  IUA ng = IUA((IUC(src.g) * sa + IUC(dg) * da * (255 - sa)) / oa);
  IUA nb = IUA((IUC(src.b) * sa + IUC(db) * da * (255 - sa)) / oa);
  img.data[idx] = nr;
  img.data[idx + 1] = ng;
  img.data[idx + 2] = nb;
  img.data[idx + 3] = oa;
}

void Fill(CImage& img, const VColor& c) {
  if (img.data == nullptr || img.format != PixelFormat::RGBA8) return;
  IUC px = (IUC)img.width * (IUC)img.height;
  for (IUC i = 0; i < px; ++i) {
    img.data[i * 4] = c.r;
    img.data[i * 4 + 1] = c.g;
    img.data[i * 4 + 2] = c.b;
    img.data[i * 4 + 3] = c.a;
  }
}

void Clear(CImage& img) {
  if (img.data == nullptr) return;
  IUC px = IUC(img.width) * IUC(img.height);
  IUC bytes = px * 4;
  for (IUC i = 0; i < bytes; ++i) img.data[i] = 0;
}

void PutPixel(CImage& img, ISC x, ISC y, const VColor& c) {
  BlendPixel(img, x, y, c);
}

void Line(CImage& img, ISC x0, ISC y0, ISC x1, ISC y1, const VColor& c,
          ISC thickness) {
  ISC dx = x1 > x0 ? x1 - x0 : x0 - x1;
  ISC dy = y1 > y0 ? y1 - y0 : y0 - y1;
  ISC sx = x0 < x1 ? 1 : -1;
  ISC sy = y0 < y1 ? 1 : -1;
  ISC err = dx - dy;
  for (;;) {
    if (thickness <= 1) {
      PutPixel(img, x0, y0, c);
    } else {
      ISC h = thickness / 2;
      for (ISC oy = -h; oy <= h; ++oy)
        for (ISC ox = -h; ox <= h; ++ox) PutPixel(img, x0 + ox, y0 + oy, c);
    }
    if (x0 == x1 && y0 == y1) break;
    ISC e2 = 2 * err;
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

void FillRect(CImage& img, ISC x0, ISC y0, ISC x1, ISC y1, const VColor& c) {
  if (img.data == nullptr) return;
  if (x0 > x1) {
    ISC t = x0; x0 = x1; x1 = t;
  }
  if (y0 > y1) {
    ISC t = y0; y0 = y1; y1 = t;
  }
  for (ISC y = y0; y <= y1; ++y)
    for (ISC x = x0; x <= x1; ++x) PutPixel(img, x, y, c);
}

void FillCircle(CImage& img, ISC cx, ISC cy, ISC r, const VColor& c) {
  if (r < 0) return;
  // Midpoint circle: step the radius in octants, drawing horizontal spans.
  ISC x = 0;
  ISC y = r;
  ISC d = 3 - 2 * r;
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

BOL ClipRect(CImage& img, RectangleISC* r) {
  if (r == nullptr) return false;
  if (r->x0 < 0) r->x0 = 0;
  if (r->y0 < 0) r->y0 = 0;
  if (r->x1 >= img.width) r->x1 = img.width - 1;
  if (r->y1 >= img.height) r->y1 = img.height - 1;
  if (r->x0 > r->x1 || r->y0 > r->y1) return false;
  return true;
}

}  //< namespace Exo
}  //< namespace CTK
