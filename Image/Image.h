// Copyright AStarship <https://astarship.net>.
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include "../../Script2/Operand.h"

namespace _ {

struct CImage {

};
class Image : public Operand {
 public:
  Image();

  Image(const CHA* uri);

  Image(ISN width, ISN height);

  ISN Width();

  ISN Height();

  /* Scrip2 operations. */
  virtual const Op* Star(CHN index, Crabs* crabs);

  private:

  CImage* buffer_;
};
}
