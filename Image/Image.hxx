// Copyright AStarship <https://astarship.net>.
#include "Image.h"

namespace _ {

Image::Image()
    : image_{nullptr, 0, 0, 0} {}

Image::Image(const CHA* uri)
    : image_{nullptr, 0, 0, 0} {
  (void)uri;  //< TODO: load from file when the in-house decoder lands.
}

Image::Image(ISN width, ISN height)
    : image_{nullptr, width, height, 4} {}

}  //< namespace _
