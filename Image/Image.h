// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_IMAGE_IMAGE_DECL
#define CRABSTK_IMAGE_IMAGE_DECL
#include "_Config.h"

namespace _ {

/* A raster image buffer. Width/height in pixels, 3 or 4 channels.
No heap allocation — the caller owns the pixel data. */
struct CImage {
  IUA* data;       //< Pixel data (RGBA or RGB), row-major, top-left origin.
  ISN width;       //< Width in pixels.
  ISN height;      //< Height in pixels.
  IUB channels;    //< 3 (RGB) or 4 (RGBA).
};

/* Image operations. */
class Image {
 public:
  Image();

  Image(const CHA* uri);

  Image(ISN width, ISN height);

  ISN Width() const { return image_.width; }
  ISN Height() const { return image_.height; }

  CImage& Buffer() { return image_; }
  const CImage& Buffer() const { return image_; }

 private:
  CImage image_;
};

}  //< namespace _
#endif
