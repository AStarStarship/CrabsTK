// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKFormsTextEditH
#define _CrabsTKFormsTextEditH
#include <_Config.h>
#include <ASCIICrabs/_Config.h>
#include <ASCIICrabs/Uniprinter.hpp>
namespace CTK {
namespace Forms {

/* Undo depth, and the character buffer the undo and redo stacks share. */
enum : ISC {
  TextEditUndoCount = 99,
  TextEditUndoChars = 999,
};

/* Navigation and edit keys. OR TextEditKey::Shift onto one of the others. */
enum class TextEditKey : ISC {
  Left      = 0x200000,
  Right,
  Up,
  Down,
  LineStart,
  LineEnd,
  TextStart,
  TextEnd,
  Delete,
  Backspace,
  Undo,
  Redo,
  WordLeft,
  WordRight,
  PageUp,
  PageDown,
  Shift     = 0x400000,
};

constexpr TextEditKey operator|(TextEditKey a, TextEditKey b) {
  return TextEditKey(ISC(a) | ISC(b));
}

/* One laid-out row of text. */
struct TextEditRow {
  FPC x0,               //< first x of the row.
  x1,                   //< last x of the row.
  baseline_y_delta,     //< baseline offset from the previous row.
  ymin,                 //< row top above the baseline.
  ymax;                 //< row bottom below the baseline.
  ISC character_count;  //< characters in the row.
};

/* One undo step: what changed, and where its saved characters live. */
struct TextEditUndoRecord {
  ISC where;            //< character index the edit started at.
  ISC insert_length;    //< characters inserted.
  ISC delete_length;    //< characters deleted.
  ISC char_storage;     //< index into TextEditUndoState::character, or -1.
};

/* The undo and redo stacks share one character buffer. */
struct TextEditUndoState {
  TextEditUndoRecord records[TextEditUndoCount];
  CHR character[TextEditUndoChars];
  ISB undo_point, redo_point;
  ISC undo_char_point, redo_char_point;
};

/* Cursor, selection, and undo state for one text field. */
struct TextEditState {
  ISC cursor;             //< caret character index.
  ISC select_start;       //< selection anchor.
  ISC select_end;         //< selection end; equal to select_start if none.
  ISC row_count_per_page; //< rows a page up/down jumps; 0 disables paging.
  BOL insert_mode;        //< insert (1) or overwrite (0).
  BOL single_line;        //< newlines are rejected when set.
  BOL initialized;        //< Clear has run.
  BOL has_preferred_x;    //< preferred_x holds the up/down goal column.
  FPC preferred_x;        //< goal column for up and down.
  TextEditUndoState undo;
};

/* Internal layout cursor used while walking rows. */
struct TextEditFind {
  FPC x, y, height;
  ISC first_char, length, prev_first;
};

/* Layout hooks. Leave a hook nil to use the built-in monospace default:
8 per ASCII character, one row per line, 20 tall. Decode the character at the
index with SScan from Uniprinter when a real font is used. */
struct TextEditMetrics {
  FPC (*Width)(const CHR* text, ISC at);              //< advance of at.
  void (*Row)(const CHR* text, ISC start, TextEditRow& row);  //< lay out row.
};

/* A single-line or multi-line text editor over one contiguous CHR buffer.
The editor does not allocate: Start binds it to a caller-owned nil-terminated
buffer and its capacity in CHR units, and every edit shifts that buffer in
place. Character indices are CHR unit offsets; Next and Prev step whole UTF-8
or UTF-16 characters with SScan, so a caret never lands mid-character.

@code
CHR buffer[128] = "hello";
TextEdit edit;
edit.Start(buffer, sizeof(buffer), metrics);
edit.Click(x, y);
edit.Key(TextEditKey::Left | TextEditKey::Shift);
edit.Input(CHR("!"), 1);
@endcode
*/
class TextEdit {
 public:
  TextEditState state;
  TextEditMetrics Metrics;

  TextEdit();

  /* Binds the editor to a caller-owned buffer of capacity CHR units. */
  void Start(CHR* text, ISC capacity, const TextEditMetrics& metrics);

  /* Resets the state for a fresh single-line (1) or multi-line (0) field. */
  void Clear(BOL single_line);

  /* Places the caret at the click and drops the selection. */
  ISC Click(FPC x, FPC y);

  /* Moves the caret and the selection end to the drag position. */
  ISC Drag(FPC x, FPC y);

  /* Inserts units at the caret, replacing any selection. */
  ISC Input(const CHR* insert, ISC count);

  /* Applies one navigation or edit key. Returns 1 if it was handled. */
  ISC Key(TextEditKey key);

  /* Deletes the selection. Returns 1 if there was one. */
  ISC Cut();

  /* Replaces the selection with units. Returns 1 on success. */
  ISC Paste(const CHR* insert, ISC count);

  void Undo();
  void Redo();

  /* Characters in the buffer, trailing nil excluded. */
  ISC Length() const;

  BOL HasSelection() const { return state.select_start != state.select_end; }

 private:
  CHR* text_;      //< caller-owned nil-terminated buffer.
  ISC capacity_;   //< buffer size in CHR units, nil included.

  CHC CharAt_(ISC at) const;
  ISC Next_(ISC at) const;
  ISC Prev_(ISC at) const;
  FPC Width_(ISC at) const;
  void Row_(ISC start, TextEditRow& row) const;
  BOL IsNewline_(ISC at) const;
  BOL IsSpace_(ISC at) const;
  ISC InsertChars_(ISC at, const CHR* insert, ISC count);
  void DeleteChars_(ISC at, ISC count);

  ISC LocateCoord_(FPC x, FPC y, ISC* side_on_line) const;
  void FindCharPos_(ISC at, TextEditFind& find) const;
  void Clamp_();
  void Delete_(ISC where, ISC count);
  void DeleteSelection_();
  void SortSelection_();
  void MoveToFirst_();
  void MoveToLast_();
  ISC MoveLineStart_(ISC at) const;
  ISC MoveLineEnd_(ISC at) const;
  BOL IsWordBoundary_(ISC at) const;
  ISC MoveWordLeft_(ISC at) const;
  ISC MoveWordRight_(ISC at) const;
  void PrepSelection_();
  ISC PasteInternal_(const CHR* insert, ISC count);
  static void CopyUp_(void* dest, const void* src, ISW bytes);
  static void FlushRedo_(TextEditUndoState& undo);
  static void DiscardUndo_(TextEditUndoState& undo);
  static void DiscardRedo_(TextEditUndoState& undo);
  static TextEditUndoRecord* CreateRecord_(TextEditUndoState& undo, ISC count);
  static CHR* CreateUndo_(TextEditUndoState& undo, ISC where, ISC insert_count,
                          ISC delete_count);
  void MakeUndoInsert_(ISC where, ISC count);
  void MakeUndoDelete_(ISC where, ISC count);
  void MakeUndoReplace_(ISC where, ISC old_count, ISC new_count);
};

}       //< namespace Forms
}       //< namespace CTK
#endif  //< _CrabsTKFormsTextEditH
