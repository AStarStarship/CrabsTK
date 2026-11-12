// Copyright AStarship <https://astarship.net>.
#pragma once
#include "Image.h"
#include <ASCIICrabs/Autoject.hpp>  // RAMFactory / TObjectFactory
#include <ASCIICrabs/Array.hxx>    // ArrayFill / ArrayCopy definitions

namespace CTK {
namespace Exo {
using namespace ::_;

/* The factory an image uses when the caller did not supply one. Heap, not
Stack: a RAMFactory is called with (origin, 0) to free, and TObjectFactory's
Stack allocates on that call. */
RAMFactory FactoryOf(RAMFactory ram) {
  return (ram != _NilP) ? ram : RAMFactory(TObjectFactory<ISW>::Heap);
}

AImage::AImage() : ajt_{_NilP, _NilP}, view_{_NilP, 0, 0, PixelFormat::RGBA8} {}

AImage::AImage(RAMFactory ram)
    : ajt_{_NilP, FactoryOf(ram)}, view_{_NilP, 0, 0, PixelFormat::RGBA8} {}

AImage::AImage(ISC width, ISC height, PixelFormat format, RAMFactory ram)
    : ajt_{_NilP, FactoryOf(ram)}, view_{_NilP, 0, 0, format} {
  (void)Resize(width, height, format);
}

AImage::~AImage() { Release(); }

ISC AImage::Bytes() const {
  if (Empty()) return 0;
  return Stride() * view_.height;
}

void AImage::Release() {
  if (ajt_.ram != _NilP && !IsError(ajt_.origin))
    ajt_.ram(ajt_.origin, RAMFactoryDelete);
  ajt_.origin = _NilP;
  view_.data = _NilP;
  view_.width = 0;
  view_.height = 0;
}

BOL AImage::Resize(ISC width, ISC height, PixelFormat format) {
  if (width <= 0 || height <= 0) {
    Release();
    return false;
  }
  ISC bytes = PixelBytes(format) * width * height;
  RAMFactory factory = FactoryOf(ajt_.ram);
  IUW* block = factory(_NilP, (ISW)sizeof(ISW) + bytes);
  if (IsError(block)) return false;

  IUA* pixels = TRef<IUA>(block, (ISW)sizeof(ISW));
  ArrayFill(pixels, bytes, 0);

  /* Carry over the region the old image shares with the new one. */
  if (!Empty()) {
    ISC old_stride = Stride();
    ISC new_stride = width * PixelBytes(format);
    ISC row_bytes = (old_stride < new_stride) ? old_stride : new_stride;
    ISC rows = (view_.height < height) ? view_.height : height;
    for (ISC y = 0; y < rows; ++y)
      ArrayCopy(pixels + (IUC)y * (IUC)new_stride, row_bytes,
                view_.data + (IUC)y * (IUC)old_stride, row_bytes);
    factory(ajt_.origin, RAMFactoryDelete);
  }

  ajt_.origin = block;
  ajt_.ram = factory;
  view_.data = pixels;
  view_.width = width;
  view_.height = height;
  view_.format = format;
  return true;
}

void AImage::Clear(IUB fill) {
  if (Empty()) return;
  ArrayFill(view_.data, Bytes(), (CHA)fill);
}

}  //< namespace Exo
}  //< namespace CTK
