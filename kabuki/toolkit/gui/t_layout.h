/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/*
 * @file /kabuki/toolkit/gui/layout.h
 *
 * @brief A collection of useful layout managers.  The @ref _::GridLayout
 *        was contributed by Christian Schueller.
 */

#pragma once
#include <pch.h>


#include "c_compat.h"
#include "t_object.h"
#include "t_widget.h"
#include "t_window.h"
#include "t_theme.h"
#include "t_label.h"

#include <unordered_map>

namespace _ {

// The different kinds of alignments a layout can perform.
enum class Alignment : uint8_t {
  Minimum = 0, //< Take only as much space as is required.
  Middle,      //< Center align.
  Maximum,     //< Take as much space as is allowed.
  Fill         //< Fill according to preferred sizes.
};

// The direction of data flow for a layout.
enum class Orientation {
  Horizontal = 0, //< Layout expands on horizontal axis.
  Vertical        //< Layout expands on vertical axis.
};

/*
 * \class Layout layout.h /kabuki/toolkit/gui/layout.h
 *
 * @brief Basic interface of a layout engine.
 */
class SDK Layout : public Object {
public:
  /*
   * Performs any and all resizing applicable.
   *
   * @param ctx
   *     The `NanoVG` context being used for drawing.
   *
   * @param widget
   *     The Widget this layout is controlling sizing for.
   */
  virtual void performLayout (NVGcontext *ctx, Widget *widget) const = 0;

  /*
   * The preferred size for this layout.
   *
   * @param ctx
   *     The `NanoVG` context being used for drawing.
   *
   * @param widget
   *     The Widget this layout's preferred size is considering.
   *
   * @return
   *     The preferred size, accounting for things such as spacing, padding
   *     for icons, etc.
   */
  virtual Vector2i preferredSize (NVGcontext *ctx, const Widget *widget)
    const = 0;

protected:
  // Default destructor (exists for inheritance).
  virtual ~Layout () { }
};

/*
 * \class BoxLayout layout.h /kabuki/toolkit/gui/layout.h
 *
 * @brief Simple horizontal/vertical box layout
 *
 * This widget stacks up a bunch of widgets horizontally or vertically. It adds
 * margins around the entire container and a custom spacing between adjacent
 * widgets.
 */
class SDK BoxLayout : public Layout {
public:
  /*
   * @brief Construct a box layout which packs widgets in the given \c Orientation
   *
   * @param orientation
   *     The Orientation this BoxLayout expands along
   *
   * @param alignment
   *     Widget alignment perpendicular to the chosen orientation
   *
   * @param margin
   *     Margin around the layout container
   *
   * @param spacing
   *     Extra spacing placed between widgets
   */
  BoxLayout (Orientation orientation, Alignment alignment = Alignment::Middle,
    int margin = 0, int spacing = 0)
    : mOrientation (orientation), mAlignment (alignment), mMargin (margin),
    mSpacing (spacing) {
  }

  // The Orientation this BoxLayout is using.
  Orientation orientation () const { return mOrientation; }

  // Sets the Orientation of this BoxLayout.
  void setOrientation (Orientation orientation) { mOrientation = orientation; }

  // The Alignment of this BoxLayout.
  Alignment alignment () const { return mAlignment; }

  // Sets the Alignment of this BoxLayout.
  void setAlignment (Alignment alignment) { mAlignment = alignment; }

  // The margin of this BoxLayout.
  int margin () const { return mMargin; }

  // Sets the margin of this BoxLayout.
  void setMargin (int margin) { mMargin = margin; }

  // The spacing this BoxLayout is using to pad in between widgets.
  int spacing () const { return mSpacing; }

  // Sets the spacing of this BoxLayout.
  void setSpacing (int spacing) { mSpacing = spacing; }

  /* Implementation of the layout interface */
  // See @ref Layout::preferredSize.
  virtual Vector2i preferredSize (NVGcontext *ctx, const Widget *widget)
    const override {
    Vector2i size = Vector2i::Constant (2 * mMargin);

    int yOffset = 0;
    const Window *window = dynamic_cast<const Window *>(widget);
    if (window && !window->title ().empty ()) {
      if (mOrientation == Orientation::Vertical)
        size[1] += widget->theme ()->mWindowHeaderHeight - mMargin / 2;
      else
        yOffset = widget->theme ()->mWindowHeaderHeight;
    }

    bool first = true;
    int axis1 = (int)mOrientation, axis2 = ((int)mOrientation + 1) % 2;
    for (auto w : widget->children ()) {
      if (!w->visible ())
        continue;
      if (first)
        first = false;
      else
        size[axis1] += mSpacing;

      Vector2i ps = w->preferredSize (ctx), fs = w->fixedSize ();
      Vector2i targetSize (
        fs[0] ? fs[0] : ps[0],
        fs[1] ? fs[1] : ps[1]
      );

      size[axis1] += targetSize[axis1];
      size[axis2] = std::max (size[axis2], targetSize[axis2] + 2 * mMargin);
      first = false;
    }
    return size + Vector2i (0, yOffset);
  }

  // See @ref Layout::performLayout.
  virtual void performLayout (NVGcontext *ctx, Widget *widget) const override {
    Vector2i fs_w = widget->fixedSize ();
    Vector2i containerSize (
      fs_w[0] ? fs_w[0] : widget->width (),
      fs_w[1] ? fs_w[1] : widget->height ()
    );

    int axis1 = (int)mOrientation, axis2 = ((int)mOrientation + 1) % 2;
    int position = mMargin;
    int yOffset = 0;

    const Window *window = dynamic_cast<const Window *>(widget);
    if (window && !window->title ().empty ()) {
      if (mOrientation == Orientation::Vertical) {
        position += widget->theme ()->mWindowHeaderHeight - mMargin / 2;
      } else {
        yOffset = widget->theme ()->mWindowHeaderHeight;
        containerSize[1] -= yOffset;
      }
    }

    bool first = true;
    for (auto w : widget->children ()) {
      if (!w->visible ())
        continue;
      if (first)
        first = false;
      else
        position += mSpacing;

      Vector2i ps = w->preferredSize (ctx), fs = w->fixedSize ();
      Vector2i targetSize (
        fs[0] ? fs[0] : ps[0],
        fs[1] ? fs[1] : ps[1]
      );
      Vector2i pos (0, yOffset);

      pos[axis1] = position;

      switch (mAlignment) {
      case Alignment::Minimum:
        pos[axis2] += mMargin;
        break;
      case Alignment::Middle:
        pos[axis2] += (containerSize[axis2] - targetSize[axis2]) / 2;
        break;
      case Alignment::Maximum:
        pos[axis2] += containerSize[axis2] - targetSize[axis2] - mMargin * 2;
        break;
      case Alignment::Fill:
        pos[axis2] += mMargin;
        targetSize[axis2] = fs[axis2] ? fs[axis2] : (containerSize[axis2] - mMargin * 2);
        break;
      }

      w->setPosition (pos);
      w->setSize (targetSize);
      w->performLayout (ctx);
      position += targetSize[axis1];
    }
  }

protected:
  // The Orientation of this BoxLayout.
  Orientation mOrientation;

  // The Alignment of this BoxLayout.
  Alignment mAlignment;

  // The margin of this BoxLayout.
  int mMargin;

  // The spacing between widgets of this BoxLayout.
  int mSpacing;
};

/*
 * \class GroupLayout layout.h /kabuki/toolkit/gui/layout.h
 *
 * @brief Special layout for widgets grouped by labels.
 *
 * This widget resembles a box layout in that it arranges a set of widgets
 * vertically. All widgets are indented on the horizontal axis except for
 * @ref Label widgets, which are not indented.
 *
 * This creates a pleasing layout where a number of widgets are grouped
 * under some high-level heading.
 */
class SDK GroupLayout : public Layout {
public:
  /*
   * Creates a GroupLayout.
   *
   * @param margin
   *     The margin around the widgets added.
   *
   * @param spacing
   *     The spacing between widgets added.
   *
   * @param groupSpacing
   *     The spacing between groups (groups are defined by each Label added).
   *
   * @param groupIndent
   *     The amount to indent widgets in a group (underneath a Label).
   */
  GroupLayout (int margin = 15, int spacing = 6, int groupSpacing = 14,
    int groupIndent = 20)
    : mMargin (margin), mSpacing (spacing), mGroupSpacing (groupSpacing),
    mGroupIndent (groupIndent) {}

  // The margin of this GroupLayout.
  int margin () const { return mMargin; }

  // Sets the margin of this GroupLayout.
  void setMargin (int margin) { mMargin = margin; }

  // The spacing between widgets of this GroupLayout.
  int spacing () const { return mSpacing; }

  // Sets the spacing between widgets of this GroupLayout.
  void setSpacing (int spacing) { mSpacing = spacing; }

  // The indent of widgets in a group (underneath a Label) of this GroupLayout.
  int groupIndent () const { return mGroupIndent; }

  // Sets the indent of widgets in a group (underneath a Label) of this GroupLayout.
  void setGroupIndent (int groupIndent) { mGroupIndent = groupIndent; }

  // The spacing between groups of this GroupLayout.
  int groupSpacing () const { return mGroupSpacing; }

  // Sets the spacing between groups of this GroupLayout.
  void setGroupSpacing (int groupSpacing) { mGroupSpacing = groupSpacing; }

  /* Implementation of the layout interface */
  // See @ref Layout::preferredSize.
  virtual Vector2i preferredSize (NVGcontext *ctx, const Widget *widget)
    const override {
    int height = mMargin, width = 2 * mMargin;

    const Window *window = dynamic_cast<const Window *>(widget);
    if (window && !window->title ().empty ())
      height += widget->theme ()->mWindowHeaderHeight - mMargin / 2;

    bool first = true, indent = false;
    for (auto c : widget->children ()) {
      if (!c->visible ())
        continue;
      const Label *label = dynamic_cast<const Label *>(c);
      if (!first)
        height += (label == nullptr) ? mSpacing : mGroupSpacing;
      first = false;

      Vector2i ps = c->preferredSize (ctx), fs = c->fixedSize ();
      Vector2i targetSize (
        fs[0] ? fs[0] : ps[0],
        fs[1] ? fs[1] : ps[1]
      );

      bool indentCur = indent && label == nullptr;
      height += targetSize.y ();
      width = std::max (width, targetSize.x () + 2 * mMargin + (indentCur ? mGroupIndent : 0));

      if (label)
        indent = !label->caption ().empty ();
    }
    height += mMargin;
    return Vector2i (width, height);
  }

  // See @ref Layout::performLayout.
  virtual void performLayout (NVGcontext *ctx, Widget *widget) const override {
    int height = mMargin, availableWidth =
      (widget->fixedWidth () ? widget->fixedWidth () : widget->width ()) - 2 * mMargin;

    const Window *window = dynamic_cast<const Window *>(widget);
    if (window && !window->title ().empty ())
      height += widget->theme ()->mWindowHeaderHeight - mMargin / 2;

    bool first = true, indent = false;
    for (auto c : widget->children ()) {
      if (!c->visible ())
        continue;
      const Label *label = dynamic_cast<const Label *>(c);
      if (!first)
        height += (label == nullptr) ? mSpacing : mGroupSpacing;
      first = false;

      bool indentCur = indent && label == nullptr;
      Vector2i ps = Vector2i (availableWidth - (indentCur ? mGroupIndent : 0),
        c->preferredSize (ctx).y ());
      Vector2i fs = c->fixedSize ();

      Vector2i targetSize (
        fs[0] ? fs[0] : ps[0],
        fs[1] ? fs[1] : ps[1]
      );

      c->setPosition (Vector2i (mMargin + (indentCur ? mGroupIndent : 0), height));
      c->setSize (targetSize);
      c->performLayout (ctx);

      height += targetSize.y ();

      if (label)
        indent = !label->caption ().empty ();
    }
  }

protected:
  // The margin of this GroupLayout.
  int mMargin;

  // The spacing between widgets of this GroupLayout.
  int mSpacing;

  // The spacing between groups of this GroupLayout.
  int mGroupSpacing;

  // The indent amount of a group under its defining Label of this GroupLayout.
  int mGroupIndent;
};

/*
 * \class GridLayout layout.h /kabuki/toolkit/gui/layout.h
 *
 * @brief Grid layout.
 *
 * Widgets are arranged in a grid that has a fixed grid resolution \c resolution
 * along one of the axes. The layout orientation indicates the fixed dimension;
 * widgets are also appended on this axis. The spacing between items can be
 * specified per axis. The horizontal/vertical alignment can be specified per
 * row and column.
 */
class SDK GridLayout : public Layout {
public:
  /*
   * Create a 2-column grid layout by default.
   *
   * @param orientation
   *     The fixed dimension of this GridLayout.
   *
   * @param resolution
   *     The number of rows or columns in the grid (depending on the Orientation).
   *
   * @param alignment
   *     How widgets should be aligned within each grid cell.
   *
   * @param margin
   *     The amount of spacing to add around the border of the grid.
   *
   * @param spacing
   *     The amount of spacing between widgets added to the grid.
   */
  GridLayout (Orientation orientation = Orientation::Horizontal, int resolution = 2,
    Alignment alignment = Alignment::Middle,
    int margin = 0, int spacing = 0)
    : mOrientation (orientation), mResolution (resolution), mMargin (margin) {
    mDefaultAlignment[0] = mDefaultAlignment[1] = alignment;
    mSpacing = Vector2i::Constant (spacing);
  }

  // The Orientation of this GridLayout.
  Orientation orientation () const { return mOrientation; }

  // Sets the Orientation of this GridLayout.
  void setOrientation (Orientation orientation) {
    mOrientation = orientation;
  }

  // The number of rows or columns (depending on the Orientation) of this GridLayout.
  int resolution () const { return mResolution; }

  // Sets the number of rows or columns (depending on the Orientation) of this GridLayout.
  void setResolution (int resolution) { mResolution = resolution; }

  // The spacing at the specified axis (row or column number, depending on the Orientation).
  int spacing (int axis) const { return mSpacing[axis]; }

  // Sets the spacing for a specific axis.
  void setSpacing (int axis, int spacing) { mSpacing[axis] = spacing; }

  // Sets the spacing for all axes.
  void setSpacing (int spacing) { mSpacing[0] = mSpacing[1] = spacing; }

  // The margin around this GridLayout.
  int margin () const { return mMargin; }

  // Sets the margin of this GridLayout.
  void setMargin (int margin) { mMargin = margin; }

  /*
   * The Alignment of the specified axis (row or column number, depending on
   * the Orientation) at the specified index of that row or column.
   */
  Alignment alignment (int axis, int item) const {
    if (item < (int)mAlignment[axis].size ())
      return mAlignment[axis][item];
    else
      return mDefaultAlignment[axis];
  }

  // Sets the Alignment of the columns.
  void setColAlignment (Alignment value) { mDefaultAlignment[0] = value; }

  // Sets the Alignment of the rows.
  void setRowAlignment (Alignment value) { mDefaultAlignment[1] = value; }

  // Use this to set variable Alignment for columns.
  void setColAlignment (const std::vector<Alignment> &value) { mAlignment[0] = value; }

  // Use this to set variable Alignment for rows.
  void setRowAlignment (const std::vector<Alignment> &value) { mAlignment[1] = value; }

  /* Implementation of the layout interface */
  // See @ref Layout::preferredSize.
  virtual Vector2i preferredSize (NVGcontext *ctx, const Widget *widget) const
    override {
    /* Compute minimum row / column sizes */
    std::vector<int> grid[2];
    computeLayout (ctx, widget, grid);

    Vector2i size (
      2 * mMargin + std::accumulate (grid[0].begin (), grid[0].end (), 0)
      + std::max ((int)grid[0].size () - 1, 0) * mSpacing[0],
      2 * mMargin + std::accumulate (grid[1].begin (), grid[1].end (), 0)
      + std::max ((int)grid[1].size () - 1, 0) * mSpacing[1]
    );

    const Window *window = dynamic_cast<const Window *>(widget);
    if (window && !window->title ().empty ())
      size[1] += widget->theme ()->mWindowHeaderHeight - mMargin / 2;

    return size;
  }

  // See @ref Layout::performLayout.
  virtual void performLayout (NVGcontext *ctx, Widget *widget) const override {
    Vector2i fs_w = widget->fixedSize ();
    Vector2i containerSize (
      fs_w[0] ? fs_w[0] : widget->width (),
      fs_w[1] ? fs_w[1] : widget->height ()
    );

    /* Compute minimum row / column sizes */
    std::vector<int> grid[2];
    computeLayout (ctx, widget, grid);
    int dim[2] = { (int)grid[0].size (), (int)grid[1].size () };

    Vector2i extra = Vector2i::Zero ();
    const Window *window = dynamic_cast<const Window *>(widget);
    if (window && !window->title ().empty ())
      extra[1] += widget->theme ()->mWindowHeaderHeight - mMargin / 2;

    /* Strech to size provided by \c widget */
    for (int i = 0; i < 2; i++) {
      int gridSize = 2 * mMargin + extra[i];
      for (int s : grid[i]) {
        gridSize += s;
        if (i + 1 < dim[i])
          gridSize += mSpacing[i];
      }

      if (gridSize < containerSize[i]) {
        /* Re-distribute remaining space evenly */
        int gap = containerSize[i] - gridSize;
        int g = gap / dim[i];
        int rest = gap - g * dim[i];
        for (int j = 0; j < dim[i]; ++j)
          grid[i][j] += g;
        for (int j = 0; rest > 0 && j < dim[i]; --rest, ++j)
          grid[i][j] += 1;
      }
    }

    int axis1 = (int)mOrientation, axis2 = (axis1 + 1) % 2;
    Vector2i start = Vector2i::Constant (mMargin) + extra;

    size_t numChildren = widget->children ().size ();
    size_t child = 0;

    Vector2i pos = start;
    for (int i2 = 0; i2 < dim[axis2]; i2++) {
      pos[axis1] = start[axis1];
      for (int i1 = 0; i1 < dim[axis1]; i1++) {
        Widget *w = nullptr;
        do {
          if (child >= numChildren)
            return;
          w = widget->children ()[child++];
        } while (!w->visible ());

        Vector2i ps = w->preferredSize (ctx);
        Vector2i fs = w->fixedSize ();
        Vector2i targetSize (
          fs[0] ? fs[0] : ps[0],
          fs[1] ? fs[1] : ps[1]
        );

        Vector2i itemPos (pos);
        for (int j = 0; j < 2; j++) {
          int axis = (axis1 + j) % 2;
          int item = j == 0 ? i1 : i2;
          Alignment align = alignment (axis, item);

          switch (align) {
          case Alignment::Minimum:
            break;
          case Alignment::Middle:
            itemPos[axis] += (grid[axis][item] - targetSize[axis]) / 2;
            break;
          case Alignment::Maximum:
            itemPos[axis] += grid[axis][item] - targetSize[axis];
            break;
          case Alignment::Fill:
            targetSize[axis] = fs[axis] ? fs[axis] : grid[axis][item];
            break;
          }
        }
        w->setPosition (itemPos);
        w->setSize (targetSize);
        w->performLayout (ctx);
        pos[axis1] += grid[axis1][i1] + mSpacing[axis1];
      }
      pos[axis2] += grid[axis2][i2] + mSpacing[axis2];
    }
  }

protected:
  // Compute the maximum row and column sizes
  void computeLayout (NVGcontext *ctx, const Widget *widget,
    std::vector<int> *grid) const {
    int axis1 = (int)mOrientation, axis2 = (axis1 + 1) % 2;
    size_t numChildren = widget->children ().size (), visibleChildren = 0;
    for (auto w : widget->children ())
      visibleChildren += w->visible () ? 1 : 0;

    Vector2i dim;
    dim[axis1] = mResolution;
    dim[axis2] = (int)((visibleChildren + mResolution - 1) / mResolution);

    grid[axis1].clear (); grid[axis1].resize (dim[axis1], 0);
    grid[axis2].clear (); grid[axis2].resize (dim[axis2], 0);

    size_t child = 0;
    for (int i2 = 0; i2 < dim[axis2]; i2++) {
      for (int i1 = 0; i1 < dim[axis1]; i1++) {
        Widget *w = nullptr;
        do {
          if (child >= numChildren)
            return;
          w = widget->children ()[child++];
        } while (!w->visible ());

        Vector2i ps = w->preferredSize (ctx);
        Vector2i fs = w->fixedSize ();
        Vector2i targetSize (
          fs[0] ? fs[0] : ps[0],
          fs[1] ? fs[1] : ps[1]
        );

        grid[axis1][i1] = std::max (grid[axis1][i1], targetSize[axis1]);
        grid[axis2][i2] = std::max (grid[axis2][i2], targetSize[axis2]);
      }
    }
  }

protected:
  // The Orientation defining this GridLayout.
  Orientation mOrientation;

  // The default Alignment for this GridLayout.
  Alignment mDefaultAlignment[2];

  // The actual Alignment being used.
  std::vector<Alignment> mAlignment[2];

  // The number of rows or columns before starting a new one, depending on the Orientation.
  int mResolution;

  // The spacing used for each dimension.
  Vector2i mSpacing;

  // The margin around this GridLayout.
  int mMargin;
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

/*
 * \class AdvancedGridLayout layout.h /kabuki/toolkit/gui/layout.h
 *
 * @brief Advanced Grid layout.
 *
 * The is a fancier grid layout with support for items that span multiple rows
 * or columns, and per-widget alignment flags. Each row and column additionally
 * stores a stretch factor that controls how additional space is redistributed.
 * The downside of this flexibility is that a layout anchor data structure must
 * be provided for each widget.
 *
 * An example:
 *
 * \rst
 * @code
 *
 *    using AdvancedGridLayout::Anchor;
 *    Label *label = new Label(window, "A label");
 *    // Add a centered label at grid position (1, 5), which spans two horizontal cells
 *    layout->setAnchor(label, Anchor(1, 5, 2, 1, Alignment::Middle, Alignment::Middle));
 *
 * \endrst
 *
 * The grid is initialized with user-specified column and row size vectors
 * (which can be expanded later on if desired). If a size value of zero is
 * specified for a column or row, the size is set to the maximum preferred size
 * of any widgets contained in the same row or column. Any remaining space is
 * redistributed according to the row and column stretch factors.
 *
 * The high level usage somewhat resembles the classic HIG layout:
 *
 * - https://web.archive.org/web/20070813221705/http://www.autel.cz/dmi/tutorial.html
 * - https://github.com/jaapgeurts/higlayout
 */
class SDK AdvancedGridLayout : public Layout {
public:
  /*
   * \struct Anchor layout.h /kabuki/toolkit/gui/layout.h
   *
   * @brief Helper struct to coordinate anchor points for the layout.
   */
  struct Anchor {
    uint8_t pos[2];    //< The `(x, y)` position.
    uint8_t size[2];   //< The `(x, y)` size.
    Alignment align[2];//< The `(x, y)` Alignment.

    // Creates a `0` Anchor.
    Anchor () { }

    // Create an Anchor at position `(x, y)` with specified Alignment.
    Anchor (int x, int y, Alignment horiz = Alignment::Fill,
      Alignment vert = Alignment::Fill) {
      pos[0] = (uint8_t)x; pos[1] = (uint8_t)y;
      size[0] = size[1] = 1;
      align[0] = horiz; align[1] = vert;
    }

    // Create an Anchor at position `(x, y)` of size `(w, h)` with specified alignments.
    Anchor (int x, int y, int w, int h,
      Alignment horiz = Alignment::Fill,
      Alignment vert = Alignment::Fill) {
      pos[0] = (uint8_t)x; pos[1] = (uint8_t)y;
      size[0] = (uint8_t)w; size[1] = (uint8_t)h;
      align[0] = horiz; align[1] = vert;
    }

    // Allows for printing out Anchor position, size, and alignment.
    operator std::string () const {
      char buf[50];
      NANOGUI_SNPRINTF (buf, 50, "Format[pos=(%i, %i), size=(%i, %i), align=(%i, %i)]",
        pos[0], pos[1], size[0], size[1], (int)align[0], (int)align[1]);
      return buf;
    }
  };

  // Creates an AdvancedGridLayout with specified columns, rows, and margin.
  AdvancedGridLayout (const std::vector<int> &cols = {}, const std::vector<int> &rows = {}, int margin = 0)
    : mCols (cols), mRows (rows), mMargin (margin) {
    mColStretch.resize (mCols.size (), 0);
    mRowStretch.resize (mRows.size (), 0);
  }

  // The margin of this AdvancedGridLayout.
  int margin () const { return mMargin; }

  // Sets the margin of this AdvancedGridLayout.
  void setMargin (int margin) { mMargin = margin; }

  // Return the number of cols
  int colCount () const { return (int)mCols.size (); }

  // Return the number of rows
  int rowCount () const { return (int)mRows.size (); }

  // Append a row of the given size (and stretch factor)
  void appendRow (int size, float stretch = 0.f) { mRows.push_back (size); mRowStretch.push_back (stretch); };

  // Append a column of the given size (and stretch factor)
  void appendCol (int size, float stretch = 0.f) { mCols.push_back (size); mColStretch.push_back (stretch); };

  // Set the stretch factor of a given row
  void setRowStretch (int index, float stretch) { mRowStretch.at (index) = stretch; }

  // Set the stretch factor of a given column
  void setColStretch (int index, float stretch) { mColStretch.at (index) = stretch; }

  // Specify the anchor data structure for a given widget
  void setAnchor (const Widget *widget, const Anchor &anchor) { mAnchor[widget] = anchor; }

  // Retrieve the anchor data structure for a given widget
  Anchor anchor (const Widget *widget) const {
    auto it = mAnchor.find (widget);
    if (it == mAnchor.end ())
      throw std::runtime_error ("Widget was not registered with the grid layout!");
    return it->second;
  }

  /* Implementation of the layout interface */
  // See @ref Layout::preferredSize.
  virtual Vector2i preferredSize (NVGcontext *ctx, const Widget *widget) const
    override {
    // Compute minimum row / column sizes
    std::vector<int> grid[2];
    computeLayout (ctx, widget, grid);

    Vector2i size (
      std::accumulate (grid[0].begin (), grid[0].end (), 0),
      std::accumulate (grid[1].begin (), grid[1].end (), 0));

    Vector2i extra = Vector2i::Constant (2 * mMargin);
    const Window *window = dynamic_cast<const Window *>(widget);
    if (window && !window->title ().empty ())
      extra[1] += widget->theme ()->mWindowHeaderHeight - mMargin / 2;

    return size + extra;
  }

  // See @ref Layout::performLayout.
  virtual void performLayout (NVGcontext *ctx, Widget *widget) const override {
    std::vector<int> grid[2];
    computeLayout (ctx, widget, grid);

    grid[0].insert (grid[0].begin (), mMargin);
    const Window *window = dynamic_cast<const Window *>(widget);
    if (window && !window->title ().empty ())
      grid[1].insert (grid[1].begin (), widget->theme ()->mWindowHeaderHeight + mMargin / 2);
    else
      grid[1].insert (grid[1].begin (), mMargin);

    for (int axis = 0; axis < 2; ++axis) {
      for (size_t i = 1; i < grid[axis].size (); ++i)
        grid[axis][i] += grid[axis][i - 1];

      for (Widget *w : widget->children ()) {
        if (!w->visible ())
          continue;
        Anchor anchor = this->anchor (w);

        int itemPos = grid[axis][anchor.pos[axis]];
        int cellSize = grid[axis][anchor.pos[axis] + anchor.size[axis]] - itemPos;
        int ps = w->preferredSize (ctx)[axis], fs = w->fixedSize ()[axis];
        int targetSize = fs ? fs : ps;

        switch (anchor.align[axis]) {
        case Alignment::Minimum:
          break;
        case Alignment::Middle:
          itemPos += (cellSize - targetSize) / 2;
          break;
        case Alignment::Maximum:
          itemPos += cellSize - targetSize;
          break;
        case Alignment::Fill:
          targetSize = fs ? fs : cellSize;
          break;
        }

        Vector2i pos = w->position (), size = w->size ();
        pos[axis] = itemPos;
        size[axis] = targetSize;
        w->setPosition (pos);
        w->setSize (size);
        w->performLayout (ctx);
      }
    }
  }

protected:
  // Computes the layout
  void computeLayout (NVGcontext *ctx, const Widget *widget,
    std::vector<int> *grid) const {
    Vector2i fs_w = widget->fixedSize ();
    Vector2i containerSize (
      fs_w[0] ? fs_w[0] : widget->width (),
      fs_w[1] ? fs_w[1] : widget->height ()
    );

    Vector2i extra = Vector2i::Constant (2 * mMargin);
    const Window *window = dynamic_cast<const Window *>(widget);
    if (window && !window->title ().empty ())
      extra[1] += widget->theme ()->mWindowHeaderHeight - mMargin / 2;

    containerSize -= extra;

    for (int axis = 0; axis < 2; ++axis) {
      std::vector<int> &grid = _grid[axis];
      const std::vector<int> &sizes = axis == 0 ? mCols : mRows;
      const std::vector<float> &stretch = axis == 0 ? mColStretch : mRowStretch;
      grid = sizes;

      for (int phase = 0; phase < 2; ++phase) {
        for (auto pair : mAnchor) {
          const Widget *w = pair.first;
          if (!w->visible ())
            continue;
          const Anchor &anchor = pair.second;
          if ((anchor.size[axis] == 1) != (phase == 0))
            continue;
          int ps = w->preferredSize (ctx)[axis], fs = w->fixedSize ()[axis];
          int targetSize = fs ? fs : ps;

          if (anchor.pos[axis] + anchor.size[axis] > (int)grid.size ())
            throw std::runtime_error (
              "Advanced grid layout: widget is out of bounds: " +
              (std::string) anchor);

          int currentSize = 0;
          float totalStretch = 0;
          for (int i = anchor.pos[axis];
            i < anchor.pos[axis] + anchor.size[axis]; ++i) {
            if (sizes[i] == 0 && anchor.size[axis] == 1)
              grid[i] = std::max (grid[i], targetSize);
            currentSize += grid[i];
            totalStretch += stretch[i];
          }
          if (targetSize <= currentSize)
            continue;
          if (totalStretch == 0)
            throw std::runtime_error (
              "Advanced grid layout: no space to place widget: " +
              (std::string) anchor);
          float amt = (targetSize - currentSize) / totalStretch;
          for (int i = anchor.pos[axis];
            i < anchor.pos[axis] + anchor.size[axis]; ++i) {
            grid[i] += (int)std::round (amt * stretch[i]);
          }
        }
      }

      int currentSize = std::accumulate (grid.begin (), grid.end (), 0);
      float totalStretch = std::accumulate (stretch.begin (), stretch.end (), 0.0f);
      if (currentSize >= containerSize[axis] || totalStretch == 0)
        continue;
      float amt = (containerSize[axis] - currentSize) / totalStretch;
      for (size_t i = 0; i < grid.size (); ++i)
        grid[i] += (int)std::round (amt * stretch[i]);
    }
  }

protected:
  // The columns of this AdvancedGridLayout.
  std::vector<int> mCols;

  // The rows of this AdvancedGridLayout.
  std::vector<int> mRows;

  // The stretch for each column of this AdvancedGridLayout.
  std::vector<float> mColStretch;

  // The stretch for each row of this AdvancedGridLayout.
  std::vector<float> mRowStretch;

  // The mapping of widgets to their specified anchor points.
  std::unordered_map<const Widget *, Anchor> mAnchor;

  // The margin around this AdvancedGridLayout.
  int mMargin;
};

} //< namespace _
