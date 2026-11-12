// Copyright AStarship <https://astarship.net>.
#pragma once
#include "TextEdit.h"
#include <ASCIICrabs/Array.hxx>
namespace CTK {
namespace Forms {

TextEdit::TextEdit() : text_(_NilP), capacity_(0) {
  Metrics.Width = _NilP;
  Metrics.Row = _NilP;
  Clear(0);
}

void TextEdit::Start(CHR* text, ISC capacity, const TextEditMetrics& metrics) {
  text_ = text;
  capacity_ = capacity;
  Metrics = metrics;
  Clear(0);
}

ISC TextEdit::Length() const {
  if (text_ == _NilP) return 0;
  ISC count = 0;
  while (text_[count]) ++count;
  return count;
}

CHC TextEdit::CharAt_(ISC at) const {
  if (at < 0 || at >= Length()) return 0;
  if constexpr (sizeof(CHR) == 4) {
    return CHC(text_[at]);
  } else {
    CHC c = 0;
    if (::_::SScan(text_ + at, c) == _NilP) return CHC(text_[at]);
    return c;
  }
}

ISC TextEdit::Next_(ISC at) const {
  ISC count = Length();
  if (at >= count) return count;
  if constexpr (sizeof(CHR) == 4) {
    return at + 1;
  } else {
    CHC c = 0;
    const CHR* next = ::_::SScan(text_ + at, c);
    if (next == _NilP) return at + 1;
    return ISC(next - text_);
  }
}

ISC TextEdit::Prev_(ISC at) const {
  ISC prev = at - 1;
  if (prev <= 0) return 0;
  if constexpr (sizeof(CHR) == 1) {
    while (prev > 0 && (CHA(text_[prev]) & 0xC0) == 0x80) --prev;
  } else if constexpr (sizeof(CHR) == 2) {
    while (prev > 0 && (IUB(text_[prev]) & 0xFC00) == 0xDC00) --prev;
  }
  return prev;
}

FPC TextEdit::Width_(ISC at) const {
  if (Metrics.Width) return Metrics.Width(text_, at);
  return CharAt_(at) < 0x80 ? 8.0f : 16.0f;
}

void TextEdit::Row_(ISC start, TextEditRow& row) const {
  if (Metrics.Row) {
    Metrics.Row(text_, start, row);
    return;
  }
  ISC count = Length(), i = start;
  while (i < count && !IsNewline_(i)) i = Next_(i);
  if (i < count) i = Next_(i);
  row.x0 = 0;
  row.x1 = FPC(i - start) * 8.0f;
  row.baseline_y_delta = 20.0f;
  row.ymin = -10.0f;
  row.ymax = 10.0f;
  row.character_count = i - start;
}

BOL TextEdit::IsNewline_(ISC at) const {
  CHC c = CharAt_(at);
  return c == '\n' || c == '\r';
}

BOL TextEdit::IsSpace_(ISC at) const {
  CHC c = CharAt_(at);
  return c == ' ' || c == '\t';
}

ISC TextEdit::InsertChars_(ISC at, const CHR* insert, ISC count) {
  ISC length = Length();
  if (at < 0) at = 0;
  if (at > length) at = length;
  if (count <= 0 || capacity_ <= 0) return 0;
  ISC room = capacity_ - 1 - length;
  if (count > room) count = room;
  if (count <= 0) return 0;
  ISW bytes = ISW(count * sizeof(CHR));
  CopyUp_(text_ + at + count, text_ + at,
          ISW((length - at + 1) * sizeof(CHR)));
  ::_::ArrayCopy(text_ + at, bytes, insert, bytes);
  return count;
}

void TextEdit::DeleteChars_(ISC at, ISC count) {
  ISC length = Length();
  if (at < 0) at = 0;
  if (count <= 0 || at >= length) return;
  if (at + count > length) count = length - at;
  ISW bytes = ISW((length - (at + count) + 1) * sizeof(CHR));
  ::_::ArrayCopy(text_ + at, bytes, text_ + at + count, bytes);
}

void TextEdit::CopyUp_(void* dest, const void* src, ISW bytes) {
  CHA* write = ::_::TRef<CHA>(dest);
  const CHA* read = ::_::TRef<const CHA>(src);
  for (ISW i = bytes - 1; i >= 0; --i) write[i] = read[i];
}

void TextEdit::Clear(BOL is_single_line) {
  state.undo.undo_point = 0;
  state.undo.undo_char_point = 0;
  state.undo.redo_point = TextEditUndoCount;
  state.undo.redo_char_point = TextEditUndoChars;
  state.select_start = state.select_end = 0;
  state.cursor = 0;
  state.has_preferred_x = 0;
  state.preferred_x = 0;
  state.initialized = 1;
  state.single_line = is_single_line;
  state.insert_mode = 0;
  state.row_count_per_page = 0;
}

ISC TextEdit::LocateCoord_(FPC x, FPC y, ISC* side_on_line) const {
  TextEditRow row;
  ISC count = Length();
  FPC base_y = 0, prev_x;
  ISC i = 0, k;

  row.x0 = row.x1 = 0;
  row.ymin = row.ymax = 0;
  row.character_count = 0;
  *side_on_line = 0;

  while (i < count) {
    Row_(i, row);
    if (row.character_count <= 0) return count;
    if (i == 0 && y < base_y + row.ymin) return 0;
    if (y < base_y + row.ymax) break;
    i += row.character_count;
    base_y += row.baseline_y_delta;
  }

  if (i >= count) {
    *side_on_line = 1;
    return count;
  }

  if (x < row.x0) return i;

  if (x < row.x1) {
    prev_x = row.x0;
    for (k = 0; k < row.character_count; k = Next_(i + k) - i) {
      FPC w = Width_(i + k);
      if (x < prev_x + w) {
        *side_on_line = (k == 0) ? 0 : 1;
        if (x < prev_x + w / 2) return k + i;
        return Next_(i + k);
      }
      prev_x += w;
    }
  }

  *side_on_line = 1;
  if (IsNewline_(i + row.character_count - 1)) return i + row.character_count - 1;
  return i + row.character_count;
}

ISC TextEdit::Click(FPC x, FPC y) {
  ISC side_on_line;

  if (state.single_line) {
    TextEditRow row;
    Row_(0, row);
    y = row.ymin;
  }

  state.cursor = LocateCoord_(x, y, &side_on_line);
  state.select_start = state.cursor;
  state.select_end = state.cursor;
  state.has_preferred_x = 0;
  return state.cursor;
}

ISC TextEdit::Drag(FPC x, FPC y) {
  ISC side_on_line;

  if (state.single_line) {
    TextEditRow row;
    Row_(0, row);
    y = row.ymin;
  }

  if (state.select_start == state.select_end)
    state.select_start = state.cursor;

  state.cursor = state.select_end = LocateCoord_(x, y, &side_on_line);
  return state.cursor;
}

void TextEdit::FindCharPos_(ISC at, TextEditFind& find) const {
  TextEditRow row;
  ISC count = Length();
  ISC prev_start = 0;
  ISC i = 0, first;

  if (at == count && state.single_line) {
    Row_(0, row);
    find.y = 0;
    find.first_char = 0;
    find.length = count;
    find.height = row.ymax - row.ymin;
    find.x = row.x1;
    return;
  }

  find.y = 0;
  for (;;) {
    Row_(i, row);
    if (at < i + row.character_count) break;
    /* The last row ends at the text end and carries no newline: stop here. */
    if (i + row.character_count == count && count > 0 && !IsNewline_(count - 1))
      break;
    prev_start = i;
    i += row.character_count;
    find.y += row.baseline_y_delta;
    if (i == count) {
      row.character_count = 0;
      break;
    }
  }

  find.first_char = first = i;
  find.length = row.character_count;
  find.height = row.ymax - row.ymin;
  find.prev_first = prev_start;

  find.x = row.x0;
  for (i = 0; first + i < at; i = Next_(first + i) - first)
    find.x += Width_(first + i);
}

void TextEdit::Clamp_() {
  ISC count = Length();
  if (HasSelection()) {
    if (state.select_start > count) state.select_start = count;
    if (state.select_end > count) state.select_end = count;
    if (state.select_start == state.select_end)
      state.cursor = state.select_start;
  }
  if (state.cursor > count) state.cursor = count;
}

void TextEdit::Delete_(ISC where, ISC count) {
  MakeUndoDelete_(where, count);
  DeleteChars_(where, count);
  state.has_preferred_x = 0;
}

void TextEdit::DeleteSelection_() {
  Clamp_();
  if (!HasSelection()) return;
  if (state.select_start < state.select_end) {
    Delete_(state.select_start, state.select_end - state.select_start);
    state.select_end = state.cursor = state.select_start;
  } else {
    Delete_(state.select_end, state.select_start - state.select_end);
    state.select_start = state.cursor = state.select_end;
  }
  state.has_preferred_x = 0;
}

void TextEdit::SortSelection_() {
  if (state.select_end < state.select_start) {
    ISC swap = state.select_end;
    state.select_end = state.select_start;
    state.select_start = swap;
  }
}

void TextEdit::MoveToFirst_() {
  if (!HasSelection()) return;
  SortSelection_();
  state.cursor = state.select_start;
  state.select_end = state.select_start;
  state.has_preferred_x = 0;
}

void TextEdit::MoveToLast_() {
  if (!HasSelection()) return;
  SortSelection_();
  Clamp_();
  state.cursor = state.select_end;
  state.select_start = state.select_end;
  state.has_preferred_x = 0;
}

ISC TextEdit::MoveLineStart_(ISC at) const {
  if (state.single_line) return 0;
  while (at > 0) {
    ISC prev = Prev_(at);
    if (IsNewline_(prev)) break;
    at = prev;
  }
  return at;
}

ISC TextEdit::MoveLineEnd_(ISC at) const {
  ISC count = Length();
  if (state.single_line) return count;
  while (at < count && !IsNewline_(at)) at = Next_(at);
  return at;
}

BOL TextEdit::IsWordBoundary_(ISC at) const {
  if (at <= 0) return 1;
  return IsSpace_(Prev_(at)) && !IsSpace_(at);
}

ISC TextEdit::MoveWordLeft_(ISC at) const {
  at = Prev_(at);
  while (at >= 0 && !IsWordBoundary_(at)) at = Prev_(at);
  return at < 0 ? 0 : at;
}

ISC TextEdit::MoveWordRight_(ISC at) const {
  ISC count = Length();
  at = Next_(at);
  while (at < count && !IsWordBoundary_(at)) at = Next_(at);
  return at > count ? count : at;
}

void TextEdit::PrepSelection_() {
  if (!HasSelection())
    state.select_start = state.select_end = state.cursor;
  else
    state.cursor = state.select_end;
}

ISC TextEdit::Input(const CHR* insert, ISC count) {
  if (count <= 0) return 0;
  if (insert[0] == CHR('\n') && state.single_line) return 0;

  if (state.insert_mode && !HasSelection() && state.cursor < Length()) {
    MakeUndoReplace_(state.cursor, 1, 1);
    DeleteChars_(state.cursor, 1);
    count = InsertChars_(state.cursor, insert, count);
    if (count) {
      state.cursor += count;
      state.has_preferred_x = 0;
    }
  } else {
    DeleteSelection_();
    count = InsertChars_(state.cursor, insert, count);
    if (count) {
      MakeUndoInsert_(state.cursor, count);
      state.cursor += count;
      state.has_preferred_x = 0;
    }
  }
  return count;
}

ISC TextEdit::Cut() {
  if (!HasSelection()) return 0;
  DeleteSelection_();
  state.has_preferred_x = 0;
  return 1;
}

ISC TextEdit::PasteInternal_(const CHR* insert, ISC count) {
  Clamp_();
  DeleteSelection_();
  count = InsertChars_(state.cursor, insert, count);
  if (count) {
    MakeUndoInsert_(state.cursor, count);
    state.cursor += count;
    state.has_preferred_x = 0;
    return 1;
  }
  return 0;
}

ISC TextEdit::Paste(const CHR* insert, ISC count) {
  return PasteInternal_(insert, count);
}

void TextEdit::FlushRedo_(TextEditUndoState& undo) {
  undo.redo_point = TextEditUndoCount;
  undo.redo_char_point = TextEditUndoChars;
}

void TextEdit::DiscardUndo_(TextEditUndoState& undo) {
  if (undo.undo_point <= 0) return;
  if (undo.records[0].char_storage >= 0) {
    ISC n = undo.records[0].insert_length, i;
    undo.undo_char_point -= n;
    ::_::ArrayCopy(undo.character, ISW(undo.undo_char_point * sizeof(CHR)),
                   undo.character + n, ISW(undo.undo_char_point * sizeof(CHR)));
    for (i = 0; i < undo.undo_point; ++i)
      if (undo.records[i].char_storage >= 0) undo.records[i].char_storage -= n;
  }
  --undo.undo_point;
  ::_::ArrayCopy(undo.records,
                 ISW(undo.undo_point * sizeof(TextEditUndoRecord)),
                 undo.records + 1,
                 ISW(undo.undo_point * sizeof(TextEditUndoRecord)));
}

void TextEdit::DiscardRedo_(TextEditUndoState& undo) {
  ISC last = TextEditUndoCount - 1;
  if (undo.redo_point > last) return;
  if (undo.records[last].char_storage >= 0) {
    ISC n = undo.records[last].insert_length, i;
    undo.redo_char_point += n;
    CopyUp_(undo.character + undo.redo_char_point,
            undo.character + undo.redo_char_point - n,
            ISW((TextEditUndoChars - undo.redo_char_point) * sizeof(CHR)));
    for (i = undo.redo_point; i < last; ++i)
      if (undo.records[i].char_storage >= 0) undo.records[i].char_storage += n;
  }
  CopyUp_(undo.records + undo.redo_point + 1, undo.records + undo.redo_point,
          ISW((TextEditUndoCount - undo.redo_point - 1) *
              sizeof(TextEditUndoRecord)));
  ++undo.redo_point;
}

TextEditUndoRecord* TextEdit::CreateRecord_(TextEditUndoState& undo,
                                            ISC count) {
  FlushRedo_(undo);
  if (undo.undo_point == TextEditUndoCount) DiscardUndo_(undo);
  if (count > TextEditUndoChars) {
    undo.undo_point = 0;
    undo.undo_char_point = 0;
    return _NilP;
  }
  while (undo.undo_char_point + count > TextEditUndoChars) DiscardUndo_(undo);
  return &undo.records[undo.undo_point++];
}

CHR* TextEdit::CreateUndo_(TextEditUndoState& undo, ISC where,
                           ISC insert_count, ISC delete_count) {
  TextEditUndoRecord* record = CreateRecord_(undo, insert_count);
  if (record == _NilP) return _NilP;
  record->where = where;
  record->insert_length = insert_count;
  record->delete_length = delete_count;
  if (insert_count == 0) {
    record->char_storage = -1;
    return _NilP;
  }
  record->char_storage = undo.undo_char_point;
  undo.undo_char_point += insert_count;
  return &undo.character[record->char_storage];
}

void TextEdit::MakeUndoInsert_(ISC where, ISC count) {
  CreateUndo_(state.undo, where, 0, count);
}

void TextEdit::MakeUndoDelete_(ISC where, ISC count) {
  ISC i;
  CHR* saved = CreateUndo_(state.undo, where, count, 0);
  if (saved == _NilP) return;
  for (i = 0; i < count; ++i) saved[i] = text_[where + i];
}

void TextEdit::MakeUndoReplace_(ISC where, ISC old_count, ISC new_count) {
  ISC i;
  CHR* saved = CreateUndo_(state.undo, where, old_count, new_count);
  if (saved == _NilP) return;
  for (i = 0; i < old_count; ++i) saved[i] = text_[where + i];
}

void TextEdit::Undo() {
  TextEditUndoState& undo = state.undo;
  TextEditUndoRecord record, * redo;

  if (undo.undo_point == 0) return;

  record = undo.records[undo.undo_point - 1];
  redo = &undo.records[undo.redo_point - 1];
  redo->char_storage = -1;
  redo->insert_length = record.delete_length;
  redo->delete_length = record.insert_length;
  redo->where = record.where;

  if (record.delete_length) {
    if (undo.undo_char_point + record.delete_length >= TextEditUndoChars) {
      redo->insert_length = 0;
    } else {
      ISC i;
      while (undo.undo_char_point + record.delete_length >
             undo.redo_char_point) {
        if (undo.redo_point == TextEditUndoCount) return;
        DiscardRedo_(undo);
      }
      redo = &undo.records[undo.redo_point - 1];
      redo->char_storage = undo.redo_char_point - record.delete_length;
      undo.redo_char_point -= record.delete_length;
      for (i = 0; i < record.delete_length; ++i)
        undo.character[redo->char_storage + i] = text_[record.where + i];
    }
    DeleteChars_(record.where, record.delete_length);
  }

  if (record.insert_length) {
    record.insert_length = InsertChars_(
        record.where, &undo.character[record.char_storage], record.insert_length);
    undo.undo_char_point -= record.insert_length;
  }

  state.cursor = record.where + record.insert_length;
  --undo.undo_point;
  --undo.redo_point;
}

void TextEdit::Redo() {
  TextEditUndoState& undo = state.undo;
  TextEditUndoRecord* record, redo;

  if (undo.redo_point == TextEditUndoCount) return;

  record = &undo.records[undo.undo_point];
  redo = undo.records[undo.redo_point];

  record->delete_length = redo.insert_length;
  record->insert_length = redo.delete_length;
  record->where = redo.where;
  record->char_storage = -1;

  if (redo.delete_length) {
    if (undo.undo_char_point + record->insert_length > undo.redo_char_point) {
      record->insert_length = 0;
      record->delete_length = 0;
    } else {
      ISC i;
      record->char_storage = undo.undo_char_point;
      undo.undo_char_point += record->insert_length;
      for (i = 0; i < record->insert_length; ++i)
        undo.character[record->char_storage + i] = text_[record->where + i];
    }
    DeleteChars_(redo.where, redo.delete_length);
  }

  if (redo.insert_length) {
    redo.insert_length = InsertChars_(
        redo.where, &undo.character[redo.char_storage], redo.insert_length);
    undo.redo_char_point += redo.insert_length;
  }

  state.cursor = redo.where + redo.insert_length;
  ++undo.undo_point;
  ++undo.redo_point;
}

ISC TextEdit::Key(TextEditKey key) {
  ISC raw = ISC(key);
  BOL shift = (raw & ISC(TextEditKey::Shift)) != 0;
  TextEditKey base = TextEditKey(raw & ~ISC(TextEditKey::Shift));
retry:
  switch (base) {
    case TextEditKey::Undo:
      Undo();
      state.has_preferred_x = 0;
      return 1;

    case TextEditKey::Redo:
      Redo();
      state.has_preferred_x = 0;
      return 1;

    case TextEditKey::Left:
      if (shift) {
        Clamp_();
        PrepSelection_();
        if (state.select_end > 0) state.select_end = Prev_(state.select_end);
        state.cursor = state.select_end;
      } else if (HasSelection()) {
        MoveToFirst_();
      } else if (state.cursor > 0) {
        state.cursor = Prev_(state.cursor);
      }
      state.has_preferred_x = 0;
      return 1;

    case TextEditKey::Right:
      if (shift) {
        PrepSelection_();
        state.select_end = Next_(state.select_end);
        Clamp_();
        state.cursor = state.select_end;
      } else if (HasSelection()) {
        MoveToLast_();
      } else {
        state.cursor = Next_(state.cursor);
        Clamp_();
      }
      state.has_preferred_x = 0;
      return 1;

    case TextEditKey::WordLeft:
      if (shift) {
        if (!HasSelection()) PrepSelection_();
        state.cursor = MoveWordLeft_(state.cursor);
        state.select_end = state.cursor;
        Clamp_();
      } else if (HasSelection()) {
        MoveToFirst_();
      } else {
        state.cursor = MoveWordLeft_(state.cursor);
        Clamp_();
      }
      return 1;

    case TextEditKey::WordRight:
      if (shift) {
        if (!HasSelection()) PrepSelection_();
        state.cursor = MoveWordRight_(state.cursor);
        state.select_end = state.cursor;
        Clamp_();
      } else if (HasSelection()) {
        MoveToLast_();
      } else {
        state.cursor = MoveWordRight_(state.cursor);
        Clamp_();
      }
      return 1;

    case TextEditKey::Down:
    case TextEditKey::PageDown: {
      TextEditFind find;
      TextEditRow row;
      ISC i, j;
      BOL is_page = base == TextEditKey::PageDown;
      ISC row_count = is_page ? state.row_count_per_page : 1;

      if (!is_page && state.single_line) {
        base = TextEditKey::Right;
        goto retry;
      }

      if (shift)
        PrepSelection_();
      else if (HasSelection())
        MoveToLast_();

      Clamp_();
      FindCharPos_(state.cursor, find);

      for (j = 0; j < row_count; ++j) {
        FPC x, goal_x = state.has_preferred_x ? state.preferred_x : find.x;
        ISC start = find.first_char + find.length;

        if (find.length == 0) break;

        state.cursor = start;
        Row_(state.cursor, row);
        x = row.x0;
        for (i = 0; i < row.character_count;) {
          FPC dx = Width_(start + i);
          ISC next = Next_(state.cursor);
          if (IsNewline_(state.cursor)) break;
          x += dx;
          if (x > goal_x) break;
          i += next - state.cursor;
          state.cursor = next;
        }
        Clamp_();

        state.has_preferred_x = 1;
        state.preferred_x = goal_x;

        if (shift) state.select_end = state.cursor;

        find.first_char = find.first_char + find.length;
        find.length = row.character_count;
      }
      return 1;
    }

    case TextEditKey::Up:
    case TextEditKey::PageUp: {
      TextEditFind find;
      TextEditRow row;
      ISC i, j, prev_scan;
      BOL is_page = base == TextEditKey::PageUp;
      ISC row_count = is_page ? state.row_count_per_page : 1;

      if (!is_page && state.single_line) {
        base = TextEditKey::Left;
        goto retry;
      }

      if (shift)
        PrepSelection_();
      else if (HasSelection())
        MoveToFirst_();

      Clamp_();
      FindCharPos_(state.cursor, find);

      for (j = 0; j < row_count; ++j) {
        FPC x, goal_x = state.has_preferred_x ? state.preferred_x : find.x;

        if (find.prev_first == find.first_char) break;

        state.cursor = find.prev_first;
        Row_(state.cursor, row);
        x = row.x0;
        for (i = 0; i < row.character_count;) {
          FPC dx = Width_(find.prev_first + i);
          ISC next = Next_(state.cursor);
          if (IsNewline_(state.cursor)) break;
          x += dx;
          if (x > goal_x) break;
          i += next - state.cursor;
          state.cursor = next;
        }
        Clamp_();

        state.has_preferred_x = 1;
        state.preferred_x = goal_x;

        if (shift) state.select_end = state.cursor;

        prev_scan = find.prev_first > 0 ? find.prev_first - 1 : 0;
        while (prev_scan > 0) {
          ISC prev = Prev_(prev_scan);
          if (IsNewline_(prev)) break;
          prev_scan = prev;
        }
        find.first_char = find.prev_first;
        find.prev_first = MoveLineStart_(prev_scan);
      }
      return 1;
    }

    case TextEditKey::Delete:
      if (HasSelection()) {
        DeleteSelection_();
      } else if (state.cursor < Length()) {
        Delete_(state.cursor, Next_(state.cursor) - state.cursor);
      }
      state.has_preferred_x = 0;
      return 1;

    case TextEditKey::Backspace:
      if (HasSelection()) {
        DeleteSelection_();
      } else {
        Clamp_();
        if (state.cursor > 0) {
          ISC prev = Prev_(state.cursor);
          Delete_(prev, state.cursor - prev);
          state.cursor = prev;
        }
      }
      state.has_preferred_x = 0;
      return 1;

    case TextEditKey::TextStart:
      if (shift) {
        PrepSelection_();
        state.cursor = state.select_end = 0;
      } else {
        state.cursor = state.select_start = state.select_end = 0;
      }
      state.has_preferred_x = 0;
      return 1;

    case TextEditKey::TextEnd:
      if (shift) {
        PrepSelection_();
        state.cursor = state.select_end = Length();
      } else {
        state.cursor = Length();
        state.select_start = state.select_end = 0;
      }
      state.has_preferred_x = 0;
      return 1;

    case TextEditKey::LineStart:
      Clamp_();
      if (shift) {
        PrepSelection_();
        state.cursor = MoveLineStart_(state.cursor);
        state.select_end = state.cursor;
      } else {
        MoveToFirst_();
        state.cursor = MoveLineStart_(state.cursor);
      }
      state.has_preferred_x = 0;
      return 1;

    case TextEditKey::LineEnd:
      Clamp_();
      if (shift) {
        PrepSelection_();
        state.cursor = MoveLineEnd_(state.cursor);
        state.select_end = state.cursor;
      } else {
        MoveToLast_();
        state.cursor = MoveLineEnd_(state.cursor);
      }
      state.has_preferred_x = 0;
      return 1;

    default:
      return 0;
  }
}

}       //< namespace Forms
}       //< namespace CTK
