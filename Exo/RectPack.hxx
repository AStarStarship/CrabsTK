// Copyright AStarship <https://astarship.net>.
//
// RectPack.hxx — skyline rectangle packer. The algorithm is Barrett's
// stb_rect_pack skyline, kept whole because the heuristics are the whole
// value: the sort is an in-place heapsort instead of qsort, and the nodes
// come from a Room Wall instead of a caller array.
#pragma once
#ifndef _CrabsTKExoRectPackHXX
#define _CrabsTKExoRectPackHXX
#include "RectPack.h"
namespace CTK {
namespace Exo {

/* The List parameter set a Wall is built on: ISS, ISR, DTB. The LST_P and
WAL_P macros only expand inside a template, so the set is spelled out. */
typedef _::TList<ISS, ISR, DTB> PackerList;

/* The value the packer writes into a rectangle it could not place. The
skyline sentinel is 1 << 30, so both stay far inside the coordinate range. */
enum : ISC {
  RectPackMaxCoord = 0x7FFFFFFF,
  RectPackSentinelY = 1 << 30,
};

/* Type code for the node block: an 8 byte POD is aligned for both the
pointer in PackerNode and the ISC pair. */
enum : DTB { RectPackNodeType = _::_IUD };

RectPack::RectPack()
  : nodes_(_NilP),
    node_count_(0),
    align_(1),
    width_(0),
    height_(0),
    order_(PackOrder::SkylineBottomLeft),
    active_head_(_NilP),
    free_head_(_NilP) {
  extra_[0].x = extra_[0].y = 0;
  extra_[0].next = _NilP;
  extra_[1].x = extra_[1].y = 0;
  extra_[1].next = _NilP;
}

BOL RectPack::Start(_::TWall<>* wall, ISC width, ISC height,
                     ISC nodes) {
  if (wall == _NilP || width <= 0 || height <= 0 || nodes <= 0) return false;
  PackerList* root = _::TWallRoot<>(wall);
  if (root == _NilP) return false;
  ISC bytes = nodes * ISC(sizeof(PackerNode));
  ISR index = _::TListAlloc<ISS, ISR, DTB>(root, DTB(RectPackNodeType),
                                          ISS(bytes));
  if (index < 0) {
    nodes_ = _NilP;
    node_count_ = 0;
    return false;
  }
  nodes_ = _::TListValue<ISS, ISR, DTB, PackerNode>(root, index);
  if (nodes_ == _NilP) {
    node_count_ = 0;
    return false;
  }
  node_count_ = nodes;
  return Reset(width, height);
}

BOL RectPack::Reset(ISC width, ISC height) {
  if (nodes_ == _NilP || node_count_ <= 0) return false;
  if (width <= 0 || height <= 0) return false;
  width_ = width;
  height_ = height;
  align_ = 1;
  order_ = PackOrder::SkylineBottomLeft;

  for (ISC i = 0; i < node_count_ - 1; ++i) nodes_[i].next = &nodes_[i + 1];
  nodes_[node_count_ - 1].next = _NilP;

  free_head_ = &nodes_[0];
  active_head_ = &extra_[0];
  SetAllowOutOfMem(false);

  /* Node 0 spans the sheet; node 1 is the sentinel that ends every walk,
  so a walk never has to test the sheet width. */
  extra_[0].x = 0;
  extra_[0].y = 0;
  extra_[0].next = &extra_[1];
  extra_[1].x = width;
  extra_[1].y = RectPackSentinelY;
  extra_[1].next = _NilP;
  return true;
}

BOL RectPack::Place(ISC width, ISC height, ISC& x, ISC& y) {
  if (nodes_ == _NilP || width <= 0 || height <= 0) return false;
  Placement found = Place_(width, height);
  if (found.link == _NilP) return false;
  x = found.x;
  y = found.y;
  return true;
}

void RectPack::SetAllowOutOfMem(BOL allow) {
  /* Running out of nodes is fine, so leave the widths alone: that packs
  tighter. Otherwise quantize so node_count * align is always >= width and
  the algorithm cannot exhaust its nodes. */
  align_ = allow ? 1 : (width_ + node_count_ - 1) / node_count_;
  if (align_ < 1) align_ = 1;
}

/* ── Ordering ────────────────────────────────────────────────────────
Height descending, then width descending: the tall rectangles go in first
while the skyline is still flat, which is where the algorithm does best. */
BOL RectPack::HeightFirst_(const Rect& a, const Rect& b) {
  if (a.h != b.h) return a.h > b.h;
  return a.w > b.w;
}

/* packed holds the caller's index while the array is out of order. */
BOL RectPack::SlotFirst_(const Rect& a, const Rect& b) {
  return a.packed < b.packed;
}

/* Heapsort: in place, no allocation, no comparator callbacks, worst case
O(n log n) on a glyph sheet that is already in atlas order. Before_ is the
final order, so the heap keeps the last element at the root and extraction
walks the array back into that order. */
BOL RectPack::Before_(const Rect& a, const Rect& b, BOL by_id) {
  return by_id ? SlotFirst_(a, b) : HeightFirst_(a, b);
}

void RectPack::Sift_(Rect* rects, ISC root, ISC count, BOL by_id) {
  for (;;) {
    ISC child = 2 * root + 1;
    if (child >= count) return;
    /* The larger child is the one that does not come first. */
    if (child + 1 < count && Before_(rects[child], rects[child + 1], by_id))
      ++child;
    /* The heap holds as soon as the parent does not come before the child. */
    if (!Before_(rects[root], rects[child], by_id)) return;
    Rect swap = rects[root];
    rects[root] = rects[child];
    rects[child] = swap;
    root = child;
  }
}

void RectPack::Sort_(Rect* rects, ISC count, BOL by_id) {
  if (count < 2) return;
  for (ISC root = (count - 2) / 2; root >= 0; --root)
    Sift_(rects, root, count, by_id);
  for (ISC end = count - 1; end > 0; --end) {
    Rect swap = rects[0];
    rects[0] = rects[end];
    rects[end] = swap;
    Sift_(rects, 0, end, by_id);
  }
}

/* ── Skyline ─────────────────────────────────────────────────────────

The lowest y a rectangle of `width` reaches when its left edge sits at x0,
plus the area it wastes underneath it. Walking stops at the sentinel: the
caller has already stepped the node list to the first node whose next spans
x0. */
ISC RectPack::MinY_(PackerNode* first, ISC x0, ISC width, ISC* waste) const {
  PackerNode* node = first;
  ISC x1 = x0 + width;
  ISC min_y = 0;
  ISC visited_width = 0;
  ISC waste_area = 0;

  while (node->x < x1) {
    if (node->y > min_y) {
      /* Raising the floor accounts for every width visited so far. */
      waste_area += visited_width * (node->y - min_y);
      min_y = node->y;
      if (node->x < x0)
        visited_width += node->next->x - x0;
      else
        visited_width += node->next->x - node->x;
    } else {
      ISC under_width = node->next->x - node->x;
      if (under_width + visited_width > width)
        under_width = width - visited_width;
      waste_area += under_width * (min_y - node->y);
      visited_width += under_width;
    }
    node = node->next;
  }

  *waste = waste_area;
  return min_y;
}

/* The cheapest left edge for this rectangle, or a nil link when it fits
nowhere. */
RectPack::Placement RectPack::Find_(ISC width, ISC height) {
  Placement found;
  PackerNode* node = _NilP;
  PackerNode* tail = _NilP;
  PackerNode** prev = _NilP;
  PackerNode** best = _NilP;
  ISC best_x = 0;
  ISC best_y = RectPackSentinelY;
  ISC best_waste = RectPackSentinelY;

  /* Round the width up to the node quantum so the node budget holds. */
  width = width + align_ - 1;
  width -= width % align_;

  if (width > width_ || height > height_) {
    found.x = found.y = 0;
    found.link = _NilP;
    return found;
  }

  node = active_head_;
  prev = &active_head_;
  while (node->x + width <= width_) {
    ISC waste = 0;
    ISC y = MinY_(node, node->x, width, &waste);
    if (order_ == PackOrder::SkylineBottomLeft) {
      if (y < best_y) {
        best_y = y;
        best = prev;
      }
    } else if (y + height <= height_) {
      /* Best fit can only take a spot the rectangle fits into vertically. */
      if (y < best_y || (y == best_y && waste < best_waste)) {
        best_y = y;
        best_waste = waste;
        best = prev;
      }
    }
    prev = &node->next;
    node = node->next;
  }

  best_x = best == _NilP ? 0 : (*best)->x;

  /* Bottom left always left-aligns, so best fit also tries right-aligning
  against every node: the same rectangle pushed right can waste less. */
  if (order_ == PackOrder::SkylineBestFit) {
    tail = active_head_;
    node = active_head_;
    prev = &active_head_;
    while (tail->x < width) tail = tail->next;
    while (tail) {
      ISC xpos = tail->x - width;
      ISC waste = 0;
      while (node->next->x <= xpos) {
        prev = &node->next;
        node = node->next;
      }
      ISC y = MinY_(node, xpos, width, &waste);
      if (y + height <= height_ && y <= best_y) {
        if (y < best_y || waste < best_waste ||
            (waste == best_waste && xpos < best_x)) {
          best_x = xpos;
          best_y = y;
          best_waste = waste;
          best = prev;
        }
      }
      tail = tail->next;
    }
  }

  found.x = best_x;
  found.y = best_y;
  found.link = best;
  return found;
}

/* Reserves the skyline under the rectangle, recycles every node it covers,
and stitches the skyline back together. */
RectPack::Placement RectPack::Place_(ISC width, ISC height) {
  Placement found = Find_(width, height);
  PackerNode* node = _NilP;
  PackerNode* cur = _NilP;

  if (found.link == _NilP || found.y + height > height_ ||
      free_head_ == _NilP) {
    found.link = _NilP;
    return found;
  }

  node = free_head_;
  node->x = found.x;
  node->y = found.y + height;
  free_head_ = node->next;

  /* Insert the new skyline node at the found link, keeping the node that
  already starts there if it starts further right. */
  cur = *found.link;
  if (cur->x < found.x) {
    PackerNode* next = cur->next;
    cur->next = node;
    cur = next;
  } else {
    *found.link = node;
  }

  /* Every node the rectangle now covers goes back on the free list. */
  while (cur->next && cur->next->x <= found.x + width) {
    PackerNode* next = cur->next;
    cur->next = free_head_;
    free_head_ = cur;
    cur = next;
  }

  node->next = cur;
  if (cur->x < found.x + width) cur->x = found.x + width;
  return found;
}

BOL RectPack::Pack(Rect* rects, ISC count) {
  if (count <= 0) return true;
  if (rects == _NilP || nodes_ == _NilP) return false;

  /* packed carries the caller's index while the array is in height order,
  so the order can be restored at the end. */
  for (ISC i = 0; i < count; ++i) rects[i].packed = i;
  Sort_(rects, count, false);

  for (ISC i = 0; i < count; ++i) {
    if (rects[i].w == 0 || rects[i].h == 0) {
      rects[i].x = rects[i].y = 0;  //< an empty rectangle needs no space.
      continue;
    }
    Placement found = Place_(rects[i].w, rects[i].h);
    if (found.link != _NilP) {
      rects[i].x = found.x;
      rects[i].y = found.y;
    } else {
      rects[i].x = rects[i].y = RectPackMaxCoord;
    }
  }

  Sort_(rects, count, true);

  BOL all_packed = true;
  for (ISC i = 0; i < count; ++i) {
    rects[i].packed =
      !(rects[i].x == RectPackMaxCoord && rects[i].y == RectPackMaxCoord);
    if (!rects[i].packed) all_packed = false;
  }
  return all_packed;
}

}  //< namespace Exo
}  //< namespace CTK
#endif  //< _CrabsTKExoRectPackHXX
