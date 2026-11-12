// Copyright AStarship <https://astarship.net>.
#pragma once
#include "RenderTarget.h"
#include <ASCIICrabs/Array.hxx>  // ArrayCopy / ArrayFill definitions

namespace CTK {
namespace Exo {

ARenderTarget::ARenderTarget(ISC width, ISC height, Exo::VColor bg)
    : width_(width),
      height_(height),
      canvas_(width, height, Exo::PixelFormat::RGBA8),
      bg_(bg) {}

ARenderTarget::~ARenderTarget() {}

void ARenderTarget::BeginFrame() {
  Exo::Fill(canvas_.View(), bg_);
}

void ARenderTarget::EndFrame() {
  /* No GPU present; nothing to present. */
}

void ARenderTarget::Blit(Exo::CImage& src, ISC px, ISC py) {
  /* Only same-format blits are defined: a format conversion here would be a
   * per-pixel path, and the point of this target is the plain copy. */
  if (src.format != canvas_.Format()) return;
  Exo::CImage& dst = canvas_.View();
  ISC pixel_bytes = Exo::PixelBytes(src.format);
  ISC src_stride = src.width * pixel_bytes;
  ISC dst_stride = dst.width * pixel_bytes;
  for (ISC y = 0; y < src.height; ++y) {
    ISC dy = py + y;
    if (dy < 0 || dy >= dst.height) continue;
    ISC x0 = (px < 0) ? 0 : px;
    ISC x1 = px + src.width;
    if (x1 > dst.width) x1 = dst.width;
    if (x1 <= x0) continue;
    ISC run = (x1 - x0) * pixel_bytes;
    ::_::ArrayCopy(dst.data + (IUC)dy * (IUC)dst_stride + (IUC)x0 * (IUC)pixel_bytes,
              run,
              src.data + (IUC)y * (IUC)src_stride + (IUC)(x0 - px) *
                  (IUC)pixel_bytes,
              run);
  }
}

void ARenderTarget::FillRect(ISC x0, ISC y0, ISC x1, ISC y1, Exo::VColor c) {
  Exo::FillRect(canvas_.View(), x0, y0, x1, y1, c);
}

}  //< namespace Exo
}  //< namespace CTK
