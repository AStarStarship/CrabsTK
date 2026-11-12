// Copyright AStarship <https://astarship.net>.
#pragma once
#include "../_Config.h"
#if _Seam == _CrabsTKFormsCore
#include <ASCIICrabs/_Debug.h>
#else
#include <ASCIICrabs/_Release.h>
#endif
#include <ASCIICrabs/Test.h>
#include "../../Forms/TextEdit.hxx"
namespace CTKFormsTest {
using namespace ::_;
using namespace ::CTK;

/* TextEdit binds to a caller-owned buffer and shifts it in place, so every
   assertion below reads the buffer the test owns, not editor internals. */
inline const CHA* Core(const CHA* args) {
#if _Seam >= _CrabsTKFormsCore
  _ATestBegin;
  (void)args;

  // Start binds the buffer; typing shifts that same buffer in place
  {
    CHR buffer[64] = "hello";
    Forms::TextEdit edit;
    Forms::TextEditMetrics metrics = {_NilP, _NilP};
    edit.Start(buffer, 64, metrics);
    _DCheck(edit.Length() == 5, "Start reports the bound length");
    _DCheck(edit.state.cursor == 0, "Start puts the caret at the beginning");

    edit.Key(Forms::TextEditKey::TextEnd);
    edit.Input(CHR(" world"), 6);
    _DCheck(edit.Length() == 11, "Input grows the bound buffer");
    _DCheck(buffer[5] == CHR(' ') && buffer[10] == CHR('d'),
            "Input writes through the caller's buffer");
    _DCheck(buffer[11] == 0, "buffer stays nil-terminated");
    _DCheck(edit.state.cursor == 11, "caret follows the insertion");
  }

  // Backspace deletes before the caret
  {
    CHR buffer[32] = "abc";
    Forms::TextEdit edit;
    Forms::TextEditMetrics metrics = {_NilP, _NilP};
    edit.Start(buffer, 32, metrics);
    edit.Key(Forms::TextEditKey::TextEnd);
    edit.Key(Forms::TextEditKey::Backspace);
    _DCheck(edit.Length() == 2 && buffer[0] == CHR('a') && buffer[1] == CHR('b'),
            "Backspace removes the character before the caret");
    _DCheck(edit.state.cursor == 2, "Backspace moves the caret left");
  }

  // Shift + TextEnd selects all; Cut removes the selection
  {
    CHR buffer[32] = "select me";
    Forms::TextEdit edit;
    Forms::TextEditMetrics metrics = {_NilP, _NilP};
    edit.Start(buffer, 32, metrics);
    edit.Key(Forms::TextEditKey::TextEnd | Forms::TextEditKey::Shift);
    _DCheck(edit.HasSelection(), "Shift + TextEnd selects");
    _DCheck(edit.Cut() == 1, "Cut reports the selection it removed");
    _DCheck(edit.Length() == 0 && buffer[0] == 0,
            "Cut clears the selected characters");
    _DCheck(!edit.HasSelection(), "Cut drops the selection");
  }

  // Paste replaces the selection
  {
    CHR buffer[32] = "abcdef";
    Forms::TextEdit edit;
    Forms::TextEditMetrics metrics = {_NilP, _NilP};
    edit.Start(buffer, 32, metrics);
    edit.Key(Forms::TextEditKey::TextEnd | Forms::TextEditKey::Shift);
    _DCheck(edit.Paste(CHR("xy"), 2) == 1, "Paste accepts a replacement");
    _DCheck(edit.Length() == 2 && buffer[0] == CHR('x') && buffer[1] == CHR('y'),
            "Paste replaces the selected characters");
  }

  // Undo and redo round-trip one edit
  {
    CHR buffer[32] = "abc";
    Forms::TextEdit edit;
    Forms::TextEditMetrics metrics = {_NilP, _NilP};
    edit.Start(buffer, 32, metrics);
    edit.Key(Forms::TextEditKey::TextEnd);
    edit.Input(CHR("d"), 1);
    _DCheck(edit.Length() == 4, "edit applied before undo");
    edit.Undo();
    _DCheck(edit.Length() == 3 && buffer[2] == CHR('c'), "Undo reverts the edit");
    edit.Redo();
    _DCheck(edit.Length() == 4 && buffer[3] == CHR('d'), "Redo reapplies the edit");
  }

  // A single-line field rejects a newline
  {
    CHR buffer[32] = "one";
    Forms::TextEdit edit;
    Forms::TextEditMetrics metrics = {_NilP, _NilP};
    edit.Start(buffer, 32, metrics);
    edit.Clear(1);
    edit.Key(Forms::TextEditKey::TextEnd);
    edit.Input(CHR("\n"), 1);
    _DCheck(edit.Length() == 3, "single_line rejects a newline");
  }

  // Capacity is a hard bound, nil included
  {
    CHR buffer[8] = "abc";
    Forms::TextEdit edit;
    Forms::TextEditMetrics metrics = {_NilP, _NilP};
    edit.Start(buffer, 8, metrics);
    edit.Key(Forms::TextEditKey::TextEnd);
    edit.Input(CHR("defghij"), 7);
    _DCheck(edit.Length() == 7, "Input stops at capacity minus the terminator");
    _DCheck(buffer[7] == 0, "capacity bound keeps the buffer terminated");
  }

  // Click places the caret; Drag extends the selection from it
  {
    CHR buffer[32] = "click drag";
    Forms::TextEdit edit;
    Forms::TextEditMetrics metrics = {_NilP, _NilP};
    edit.Start(buffer, 32, metrics);
    edit.Click(0.0F, 0.0F);
    _DCheck(edit.state.cursor == 0, "Click at the left margin puts the caret first");
    edit.Click(0.0F, 0.0F);
    edit.Drag(24.0F, 0.0F);
    _DCheck(edit.HasSelection(), "Drag extends a selection");
    _DCheck(edit.state.select_start == 0,
            "Drag keeps the selection anchor at the click");
  }

  _DROut("  TextEdit field behaviour clean\n");
#endif
  return _NilP;
}
}  //< namespace CTKFormsTest
