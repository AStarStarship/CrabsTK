// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKImageImageH
#define _CrabsTKImageImageH
#include <_Config.h>
#include <ASCIICrabs/Autoject.h>

namespace CTK {
namespace Exo {

/* Pixel formats. The image does NOT prescribe a format — the task does.
A scientific-visualization canvas wants lossless 8-bit RGBA; a neural-net
tensor wants float32 or uint8 with a known channel order; a video frame
wants a packed chroma-subsampled layout (NV12/4:2:0). The format tag tells
every consumer (Plot, the GL uploader, an NN loader) how to read the bytes.
Channels-per-pixel is derived from the format, not stored separately. */
enum class PixelFormat : IUB {
  RGBA8 = 0,   //< 4 x IUB, straight alpha. The default viz/canvas format.
  RGB8  = 1,   //< 3 x IUB, no alpha.
  F32   = 2,   //< 4 x FPC, linear-light FPC. NN / optics working format.
  Gray8 = 3,   //< 1 x IUB. B&W CV.
  /* Video encodings (packed, chroma-subsampled) are a later addition; the
     tag is already the extension point for them. */
};

inline IUB PixelChannels(PixelFormat f) {
  switch (f) {
    case PixelFormat::RGB8:  return 3;
    case PixelFormat::Gray8: return 1;
    case PixelFormat::RGBA8:
    case PixelFormat::F32:   return 4;
  }
  return 4;
}

/* Bytes per pixel for a format. */
inline ISC PixelBytes(PixelFormat f) {
  switch (f) {
    case PixelFormat::F32:   return 4 * (ISC)sizeof(FPC);
    case PixelFormat::RGBA8: return 4;
    case PixelFormat::RGB8:  return 3;
    case PixelFormat::Gray8: return 1;
  }
  return 4;
}

/* A borrowed raster view. The caller owns the pixels; this struct only
describes how to read them. Every draw primitive takes one of these. */
struct CImage {
  IUA* data;            //< Row-major, top-left origin.
  ISC width;            //< Width in pixels.
  ISC height;           //< Height in pixels.
  PixelFormat format;   //< How the bytes in data[] are laid out.
};

/* An owning raster buffer.
Pixels live in one contiguous Autoject block, [ISW byte_count | IUA
pixels...], allocated through the RAMFactory the image was built with. That
is the whole memory model: one allocation per image, one free, no hidden
growth, and the factory can be a heap, an arena, or a test stub without the
image knowing. A view of the block is handed to the raster primitives, so
nothing in the draw path owns memory.

Resize() allocates a fresh block through the same factory, copies the rows
that survive with ArrayCopy, and releases the old block — an image is never
grown in place, because a realloc that moves would invalidate the CImage
views the caller is holding. */
class AImage {
 public:
  AImage();

  /* Empty image bound to a factory (a nil factory means the heap). */
  explicit AImage(RAMFactory ram);

  /* Allocates and zeroes a WxH buffer. */
  AImage(ISC width, ISC height, PixelFormat format = PixelFormat::RGBA8,
         RAMFactory ram = _NilP);

  ~AImage();

  AImage(const AImage&) = delete;
  AImage& operator=(const AImage&) = delete;

  /* Reallocates to WxH. Pixels that fit in both blocks are preserved.
  @return true on success; on failure the image keeps its old contents. */
  BOL Resize(ISC width, ISC height,
             PixelFormat format = PixelFormat::RGBA8);

  /* Releases the block. The image is empty afterwards. */
  void Release();

  ISC Width() const { return view_.width; }
  ISC Height() const { return view_.height; }
  PixelFormat Format() const { return view_.format; }
  /* Bytes per row. */
  ISC Stride() const { return view_.width * PixelBytes(view_.format); }
  /* Payload bytes (stride * height), header excluded. */
  ISC Bytes() const;

  IUA* Data() { return view_.data; }
  const IUA* Data() const { return view_.data; }

  CImage& View() { return view_; }
  const CImage& View() const { return view_; }

  BOL Empty() const { return view_.data == _NilP; }

  /* Fills the payload with one byte value (0 = transparent black). */
  void Clear(IUB fill = 0);

 private:
  ::_::Autoject ajt_;  //< [ISW byte_count | pixels...]
  CImage view_;    //< Describes ajt_.origin + sizeof(ISW).
};

}  //< namespace Exo


}  //< namespace CTK
#endif
