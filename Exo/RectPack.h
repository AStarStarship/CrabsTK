// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKExoRectPackH
#define _CrabsTKExoRectPackH
#include <_Config.h>
#include <ASCIICrabs/_Config.h>
#include <ASCIICrabs/Room.hpp>
namespace CTK {
namespace Exo {

/* A rectangle to place on a sheet. The packer shuffles the array while it
runs and puts the caller's order back before returning, so id survives as
the caller's own tag. */
struct Rect {
  ISC id;      //< caller tag, never read or written.
  ISC w, h;    //< input: size in whole units.
  ISC x, y;    //< output: top left corner, y growing downward.
  ISC packed;  //< output: non-zero when x and y hold a placement.
};

/* One step of the skyline. The packer owns these; callers never see them. */
struct PackerNode {
  ISC x, y;
  PackerNode* next;
};

/* Placement rule the skyline scan prefers. Both always try the lowest
edge first; the best fit rule then breaks the tie on wasted area. */
enum class PackOrder : ISC {
  SkylineBottomLeft = 0,  //< lowest edge, then leftmost. The default.
  SkylineBestFit,         //< lowest edge, then least waste.
};                        //< (best fit is about twice as slow, packs tighter.)

/* Skyline rectangle packer for font atlases and glyph sheets.
Every node comes out of a Wall in the Room: the packer allocates nothing
and never touches the C heap, so its memory cost is one Wall the caller
already owns. A Wall holding width + 2 nodes packs the sheet without
quantizing any width; with fewer nodes the packer rounds each width up to a
multiple so it cannot run out of nodes mid-sheet.

@code
  _::TWall<> wall;
  _::TWallInit<>(&wall, 16 * 1024);
  RectPack pack;
  pack.Start(&wall, 512, 512, 512);
  BOL all_fit = pack.Pack(rects, count);
@endcode
*/
class RectPack {
 public:
  RectPack();

  /* Binds the packer to a Wall and a width by height sheet, then carves
  `nodes` skyline nodes out of that Wall. Starting drops the previous sheet;
  there is no way to pack two sheets at once. Returns false when the Wall
  cannot hold the nodes. */
  BOL Start(_::TWall<>* wall, ISC width, ISC height, ISC nodes);

  /* Re-arms the node block for a new sheet without taking more Wall space,
  which is what an atlas reset is: the pixels are recycled, the nodes are
  not. Fails until Start has succeeded. */
  BOL Reset(ISC width, ISC height);

  /* Places one rectangle in arrival order, unlike Pack, which sorts a whole
  batch by height. This is the glyph atlas case: one glyph at a time, and the
  skyline is what keeps the sheet compact. Returns false when it fits
  nowhere. */
  BOL Place(ISC width, ISC height, ISC& x, ISC& y);

  void SetOrder(PackOrder order) { order_ = order; }

  /* Accepts the unquantized algorithm, which packs tighter but can run out
  of nodes and leave a rectangle unplaced. Off by default. */
  void SetAllowOutOfMem(BOL allow);

  /* Places every rectangle: x and y are set where it fit, packed is clear
  where it did not. Returns true when all of them fit. */
  BOL Pack(Rect* rects, ISC count);

  ISC GetWidth() const { return width_; }
  ISC GetHeight() const { return height_; }
  ISC GetAlign() const { return align_; }

 private:
  /* One skyline search result: where it fits, and the link that has to be
  spliced to insert the new node there. */
  struct Placement {
    ISC x, y;
    PackerNode** link;
  };

  void Sort_(Rect* rects, ISC count, BOL by_id);
  static void Sift_(Rect* rects, ISC root, ISC count, BOL by_id);
  static BOL Before_(const Rect& a, const Rect& b, BOL by_id);
  static BOL HeightFirst_(const Rect& a, const Rect& b);
  static BOL SlotFirst_(const Rect& a, const Rect& b);

  ISC MinY_(PackerNode* first, ISC x0, ISC width, ISC* waste) const;
  Placement Find_(ISC width, ISC height);
  Placement Place_(ISC width, ISC height);

  PackerNode* nodes_;      //< node block, carved out of the Wall.
  ISC node_count_;          //< nodes in that block.
  ISC align_;               //< width quantum; 1 means unquantized.
  ISC width_, height_;      //< sheet size.
  PackOrder order_;        //< placement rule.
  PackerNode* active_head_;  //< skyline, one node per step.
  PackerNode* free_head_;    //< nodes the skyline has given back.
  PackerNode extra_[2];      //< the full width node and the sentinel.
};

}  //< namespace Exo
}  //< namespace CTK
#endif  //< _CrabsTKExoRectPackH
