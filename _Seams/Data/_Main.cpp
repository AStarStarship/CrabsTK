// Copyright AStarship <https://astarship.net>.
#include "_Config.h"
#include <unistd.h>
#include "01.Record.h"
#include "02.Dataset.h"
#include "03.AsciiPlot.h"

namespace _ {
namespace DataTests {

// Same const-CHA* seam result convention as the project's test tree.
ISC Report(const CHA* error) {
  const CHA* text = error ? error : "PASS: Data record/dataset/plot seams";
  IUC length = 0;
  while (text[length]) ++length;
  if (write(error ? 2 : 1, text, length) != ISW(length) ||
      write(error ? 2 : 1, "\n", 1) != 1)
    return 2;
  return error ? 1 : 0;
}

}  //< namespace DataTests
}  //< namespace _

ISC main(ISC arg_count, CHA** args) {
  const CHA* path = arg_count > 1 ? args[1] : "";
  const CHA* error = _::DataTests::Record(path);
  if (!error) error = _::DataTests::Dataset(path);
  if (!error) error = _::DataTests::AsciiPlot(path);
  return _::DataTests::Report(error);
}
