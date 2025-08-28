// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if SEAM >= KABUKITOOLKIT_TOUCH_CORE
#ifndef KABUKITOOLKIT_TOUCH_CONTROLMATRIX
#define KABUKITOOLKIT_TOUCH_CONTROLMATRIX
#include "ParamSet.hpp"
namespace _ {

/* A grid of Controls.
Each Parameter<ISC> Layer on the Symmetry Live can either have 8 pots or
encoders, or can have 4 pots and 4 encoders. This class LIB_MEMBER helps us swap
the values out quickly and efficiently.

A TControlMatrix must have the option of being bifurcated. */
class LIB_MEMBER TControlMatrix {
 public:
  enum {
    cMinNumRows = 1,                             //< The min number of rows.
    cMinNumColumns = 2,                          //< The min number of columns.
    cMaxNumRows = ParamSet::kElementCountMax,  //< The max number of rows.
    cMaxNumColumns =
        ParamSet::kElementCountMax,  //< The max number of columns.
  };

  /* Default constructor. */
  TControlMatrix(ISC column_count = 8, ISC num_rows = 6) {
    col_count_ = BoundRange (column_count, kMinNumColumns, kMaxNumColumns);
    num_rows = BoundRange (num_rows, kMinNumRows, kMaxNumRows);

    rows = new ParamSet[num_rows];

    for (ISC i = 0; i < num_rows; ++i) rows[i] = ParamSet (column_count);
  }

  /* Copy constructor. */
  TControlMatrix(const TControlMatrix& o)
    : row_count_ (o.row_count_), col_count_ (o.col_count_) {
    rows = new ParamSet (row_count_);
  }

  /* Virtual Destructor. */
  virtual ~TControlMatrix () {}

  /* Gets the number of rows. */
  ISC GetNumRows() const { return row_count_; }

  /* Gets the number of columns. */
  ISC GetNumColumns() const { return col_count_; }

  /* Sets the pointer to knobs[index] pointer to the newKnob. */
  void SetParameter(ISC column, ISC row, Parameter<ISC>* control) {
    if (row < 0 || column < 0 || row >= row_count_ || column >= col_count_)
      return;
    rows[row].SetControl (column, control);
  }

  /* Gets the row at the given index.
  @return gets nullptr if the index is invalid. */
  ParamSet* GetRow(ISC row) {
    if (row < 0 || row >= row_count_) return nullptr;
    return rows[row];
  }

  /* Prints this object to a AString. */
  template<typename Printer>
  Printer& Print(Printer& o) const {
    o << LineString ('-', Parameter<ISC>::kMacroHeaderLength)
      << "Control Grid: rows:" << row_count_
      << " columns:" << col_count_
      << LineString('-', Parameter<ISC>::kMacroHeaderLength);
    PrintMacroHeader (o);
    o << LineString ('-', Parameter<ISC>::kMacroHeaderLength)
      << "\n!!!!row_count_" << row_count_;

    for (ISC i = 0; i < row_count_; ++i) {
      o << "Row:" << i;
      rows[i].Print (o);
    }

    return o << LineString ('-', Parameter<ISC>::kMacroHeaderLength);
  }

  private:

  ISC row_count_,       //< Number of rows.
      col_count_;       //< Number of columns.
  ParamSet* rows_;      //< Rows of control array columns.
};
}  //< namespace _
#endif
#endif
