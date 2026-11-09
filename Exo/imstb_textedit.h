#pragma once
#ifndef INCLUDE_IMSTB_TEXTEDIT_H
#define INCLUDE_IMSTB_TEXTEDIT_H

////////////////////////////////////////////////////////////////////////
//
//     STB_TexteditState
//
// Definition of STB_TexteditState which you should store
// per-textfield; it includes cursor position, selection state,
// and undo state.
//

#ifndef IMSTB_TEXTEDIT_UNDOSTATECOUNT
#define IMSTB_TEXTEDIT_UNDOSTATECOUNT   99
#endif
#ifndef IMSTB_TEXTEDIT_UNDOCHARCOUNT
#define IMSTB_TEXTEDIT_UNDOCHARCOUNT   999
#endif
#ifndef IMSTB_TEXTEDIT_CHARTYPE
#define IMSTB_TEXTEDIT_CHARTYPE        ISC
#endif
#ifndef IMSTB_TEXTEDIT_POSITIONTYPE
#define IMSTB_TEXTEDIT_POSITIONTYPE    ISC
#endif

struct StbUndoRecord
{
   // private data
   IMSTB_TEXTEDIT_POSITIONTYPE  where;
   IMSTB_TEXTEDIT_POSITIONTYPE  insert_length;
   IMSTB_TEXTEDIT_POSITIONTYPE  delete_length;
   ISC                        char_storage;
};

struct StbUndoState
{
   // private data
   StbUndoRecord          undo_rec [IMSTB_TEXTEDIT_UNDOSTATECOUNT];
   IMSTB_TEXTEDIT_CHARTYPE  undo_char[IMSTB_TEXTEDIT_UNDOCHARCOUNT];
   ISB undo_point, redo_point;
   ISC undo_char_point, redo_char_point;
};

struct STB_TexteditState
{
   /////////////////////
   //
   // public data
   //

   ISC cursor;
   // position of the text cursor within the string

   ISC select_start;          // selection start point
   ISC select_end;
   // selection start and end point in characters; if equal, no selection.
   // note that start may be less than or greater than end (e.g. when
   // dragging the mouse, start is where the initial click was, and you
   // can drag in either direction)

   IUA insert_mode;
   // each textfield keeps its own insert mode state. to keep an app-wide
   // insert mode, copy this value in/out of the app state

   ISC row_count_per_page;
   // page size in number of row.
   // this value MUST be set to >0 for pageup or pagedown in multilines documents.

   /////////////////////
   //
   // private data
   //
   IUA cursor_at_end_of_line; // not implemented yet
   IUA initialized;
   IUA has_preferred_x;
   IUA single_line;
   IUA padding1, padding2, padding3;
   FPC preferred_x; // this determines where the cursor up/down tries to seek to along x
   StbUndoState undostate;
};


////////////////////////////////////////////////////////////////////////
//
//     StbTexteditRow
//
// Result of layout query, used by stb_textedit to determine where
// the text in each row is.

// result of layout query
struct StbTexteditRow
{
   FPC x0,x1;             // starting x location, end x location (allows for align=right, etc)
   FPC baseline_y_delta;  // position of baseline relative to previous row's baseline
   FPC ymin,ymax;         // height of row above and below baseline
   ISC num_chars;
};
#endif //INCLUDE_IMSTB_TEXTEDIT_H


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////
////   Implementation mode
////
////


// implementation isn't include-guarded, since it might have indirectly
// included just the "header" portion
#ifdef IMSTB_TEXTEDIT_IMPLEMENTATION

#ifndef IMSTB_TEXTEDIT_memmove
#include <string.h>
#define IMSTB_TEXTEDIT_memmove memmove
#endif

// [DEAR IMGUI]
// Functions must be implemented for UTF8 support
// Code in this file that uses those functions is modified for [DEAR IMGUI] and deviates from the original stb_textedit.
// There is not necessarily a '[DEAR IMGUI]' at the usage sites.
#ifndef IMSTB_TEXTEDIT_GETPREVCHARINDEX
#define IMSTB_TEXTEDIT_GETPREVCHARINDEX(OBJ, IDX) ((IDX) - 1)
#endif
#ifndef IMSTB_TEXTEDIT_GETNEXTCHARINDEX
#define IMSTB_TEXTEDIT_GETNEXTCHARINDEX(OBJ, IDX) ((IDX) + 1)
#endif

/////////////////////////////////////////////////////////////////////////////
//
//      Mouse input handling
//

// traverse the layout to locate the nearest character to a display position
ISC stb_text_locate_coord(IMSTB_TEXTEDIT_STRING *str, FPC x, FPC y, ISC* out_side_on_line)
{
   StbTexteditRow r;
   ISC n = STB_TEXTEDIT_STRINGLEN(str);
   FPC base_y = 0, prev_x;
   ISC i=0, k;

   r.x0 = r.x1 = 0;
   r.ymin = r.ymax = 0;
   r.num_chars = 0;
   *out_side_on_line = 0;

   // search rows to find one that straddles 'y'
   while (i < n) {
      STB_TEXTEDIT_LAYOUTROW(&r, str, i);
      if (r.num_chars <= 0)
         return n;

      if (i==0 && y < base_y + r.ymin)
         return 0;

      if (y < base_y + r.ymax)
         break;

      i += r.num_chars;
      base_y += r.baseline_y_delta;
   }

   // below all text, return 'after' last character
   if (i >= n)
   {
      *out_side_on_line = 1;
      return n;
   }

   // check if it's before the beginning of the line
   if (x < r.x0)
      return i;

   // check if it's before the end of the line
   if (x < r.x1) {
      // search characters in row for one that straddles 'x'
      prev_x = r.x0;
      for (k=0; k < r.num_chars; k = IMSTB_TEXTEDIT_GETNEXTCHARINDEX(str, i + k) - i) {
         FPC w = STB_TEXTEDIT_GETWIDTH(str, i, k);
         if (x < prev_x+w) {
            *out_side_on_line = (k == 0) ? 0 : 1;
            if (x < prev_x+w/2)
               return k+i;
            else
               return IMSTB_TEXTEDIT_GETNEXTCHARINDEX(str, i + k);
         }
         prev_x += w;
      }
      // shouldn't happen, but if it does, fall through to end-of-line case
   }

   // if the last character is a newline, return that. otherwise return 'after' the last character
   *out_side_on_line = 1;
   if (STB_TEXTEDIT_GETCHAR(str, i+r.num_chars-1) == STB_TEXTEDIT_NEWLINE)
      return i+r.num_chars-1;
   else
      return i+r.num_chars;
}

// API click: on mouse down, move the cursor to the clicked location, and reset the selection
void stb_textedit_click(IMSTB_TEXTEDIT_STRING *str, STB_TexteditState *state, FPC x, FPC y)
{
   // In single-line mode, just always make y = 0. This lets the drag keep working if the mouse
   // goes off the top or bottom of the text
   ISC side_on_line;
   if( state->single_line )
   {
      StbTexteditRow r;
      STB_TEXTEDIT_LAYOUTROW(&r, str, 0);
      y = r.ymin;
   }

   state->cursor = stb_text_locate_coord(str, x, y, &side_on_line);
   state->select_start = state->cursor;
   state->select_end = state->cursor;
   state->has_preferred_x = 0;
   str->LastMoveDirectionLR = (ISA)(side_on_line ? ImGuiDir_Right : ImGuiDir_Left);
}

// API drag: on mouse drag, move the cursor and selection endpoint to the clicked location
void stb_textedit_drag(IMSTB_TEXTEDIT_STRING *str, STB_TexteditState *state, FPC x, FPC y)
{
   ISC p = 0;
   ISC side_on_line;

   // In single-line mode, just always make y = 0. This lets the drag keep working if the mouse
   // goes off the top or bottom of the text
   if( state->single_line )
   {
      StbTexteditRow r;
      STB_TEXTEDIT_LAYOUTROW(&r, str, 0);
      y = r.ymin;
   }

   if (state->select_start == state->select_end)
      state->select_start = state->cursor;

   p = stb_text_locate_coord(str, x, y, &side_on_line);
   state->cursor = state->select_end = p;
   str->LastMoveDirectionLR = (ISA)(side_on_line ? ImGuiDir_Right : ImGuiDir_Left);
}

/////////////////////////////////////////////////////////////////////////////
//
//      Keyboard input handling
//

// forward declarations
void stb_text_undo(IMSTB_TEXTEDIT_STRING *str, STB_TexteditState *state);
void stb_text_redo(IMSTB_TEXTEDIT_STRING *str, STB_TexteditState *state);
void stb_text_makeundo_delete(IMSTB_TEXTEDIT_STRING *str, STB_TexteditState *state, ISC where, ISC length);
void stb_text_makeundo_insert(STB_TexteditState *state, ISC where, ISC length);
void stb_text_makeundo_replace(IMSTB_TEXTEDIT_STRING *str, STB_TexteditState *state, ISC where, ISC old_length, ISC new_length);

struct StbFindState
{
   FPC x,y;    // position of n'th character
   FPC height; // height of line
   ISC first_char, length; // first CHA of row, and length
   ISC prev_first;  // first CHA of previous row
};

// find the x/y location of a character, and remember info about the previous row in
// case we get a move-up event (for page up, we'll have to rescan)
void stb_textedit_find_charpos(StbFindState *find, IMSTB_TEXTEDIT_STRING *str, ISC n, ISC single_line)
{
   StbTexteditRow r;
   ISC prev_start = 0;
   ISC z = STB_TEXTEDIT_STRINGLEN(str);
   ISC i=0, first;

   if (n == z && single_line) {
      // special case if it's at the end (may not be needed?)
      STB_TEXTEDIT_LAYOUTROW(&r, str, 0);
      find->y = 0;
      find->first_char = 0;
      find->length = z;
      find->height = r.ymax - r.ymin;
      find->x = r.x1;
      return;
   }

   // search rows to find the one that straddles character n
   find->y = 0;

   for(;;) {
      STB_TEXTEDIT_LAYOUTROW(&r, str, i);
      if (n < i + r.num_chars)
         break;
      if (str->LastMoveDirectionLR == ImGuiDir_Right && str->Stb->cursor > 0 && str->Stb->cursor == i + r.num_chars && STB_TEXTEDIT_GETCHAR(str, i + r.num_chars - 1) != STB_TEXTEDIT_NEWLINE) // [DEAR IMGUI] Wrapping point handling
         break;
      if (i + r.num_chars == z && z > 0 && STB_TEXTEDIT_GETCHAR(str, z - 1) != STB_TEXTEDIT_NEWLINE)  // [DEAR IMGUI] special handling for last line
         break;   // [DEAR IMGUI]
      prev_start = i;
      i += r.num_chars;
      find->y += r.baseline_y_delta;
      if (i == z) // [DEAR IMGUI]
      {
         r.num_chars = 0; // [DEAR IMGUI]
         break;   // [DEAR IMGUI]
      }
   }

   find->first_char = first = i;
   find->length = r.num_chars;
   find->height = r.ymax - r.ymin;
   find->prev_first = prev_start;

   // now scan to find xpos
   find->x = r.x0;
   for (i=0; first+i < n; i = IMSTB_TEXTEDIT_GETNEXTCHARINDEX(str, first + i) - first)
      find->x += STB_TEXTEDIT_GETWIDTH(str, first, i);
}

#define STB_TEXT_HAS_SELECTION(s)   ((s)->select_start != (s)->select_end)

// make the selection/cursor state valid if client altered the string
void stb_textedit_clamp(IMSTB_TEXTEDIT_STRING *str, STB_TexteditState *state)
{
   ISC n = STB_TEXTEDIT_STRINGLEN(str);
   if (STB_TEXT_HAS_SELECTION(state)) {
      if (state->select_start > n) state->select_start = n;
      if (state->select_end   > n) state->select_end = n;
      // if clamping forced them to be equal, move the cursor to match
      if (state->select_start == state->select_end)
         state->cursor = state->select_start;
   }
   if (state->cursor > n) state->cursor = n;
}

// delete characters while updating undo
void stb_textedit_delete(IMSTB_TEXTEDIT_STRING *str, STB_TexteditState *state, ISC where, ISC len)
{
   stb_text_makeundo_delete(str, state, where, len);
   STB_TEXTEDIT_DELETECHARS(str, where, len);
   state->has_preferred_x = 0;
}

// delete the section
void stb_textedit_delete_selection(IMSTB_TEXTEDIT_STRING *str, STB_TexteditState *state)
{
   stb_textedit_clamp(str, state);
   if (STB_TEXT_HAS_SELECTION(state)) {
      if (state->select_start < state->select_end) {
         stb_textedit_delete(str, state, state->select_start, state->select_end - state->select_start);
         state->select_end = state->cursor = state->select_start;
      } else {
         stb_textedit_delete(str, state, state->select_end, state->select_start - state->select_end);
         state->select_start = state->cursor = state->select_end;
      }
      state->has_preferred_x = 0;
   }
}

// canoncialize the selection so start <= end
void stb_textedit_sortselection(STB_TexteditState *state)
{
   if (state->select_end < state->select_start) {
      ISC temp = state->select_end;
      state->select_end = state->select_start;
      state->select_start = temp;
   }
}

// move cursor to first character of selection
void stb_textedit_move_to_first(STB_TexteditState *state)
{
   if (STB_TEXT_HAS_SELECTION(state)) {
      stb_textedit_sortselection(state);
      state->cursor = state->select_start;
      state->select_end = state->select_start;
      state->has_preferred_x = 0;
   }
}

// move cursor to last character of selection
void stb_textedit_move_to_last(IMSTB_TEXTEDIT_STRING *str, STB_TexteditState *state)
{
   if (STB_TEXT_HAS_SELECTION(state)) {
      stb_textedit_sortselection(state);
      stb_textedit_clamp(str, state);
      state->cursor = state->select_end;
      state->select_start = state->select_end;
      state->has_preferred_x = 0;
   }
}

// [DEAR IMGUI] Extracted this function so we can more easily add support for word-wrapping.
#ifndef STB_TEXTEDIT_MOVELINESTART
ISC stb_textedit_move_line_start(IMSTB_TEXTEDIT_STRING *str, STB_TexteditState *state, ISC cursor)
{
   if (state->single_line)
      return 0;
   while (cursor > 0) {
      ISC prev = IMSTB_TEXTEDIT_GETPREVCHARINDEX(str, cursor);
      if (STB_TEXTEDIT_GETCHAR(str, prev) == STB_TEXTEDIT_NEWLINE)
         break;
      cursor = prev;
   }
   return cursor;
}
#define STB_TEXTEDIT_MOVELINESTART stb_textedit_move_line_start
#endif
#ifndef STB_TEXTEDIT_MOVELINEEND
ISC stb_textedit_move_line_end(IMSTB_TEXTEDIT_STRING *str, STB_TexteditState *state, ISC cursor)
{
   ISC n = STB_TEXTEDIT_STRINGLEN(str);
   if (state->single_line)
      return n;
   while (cursor < n && STB_TEXTEDIT_GETCHAR(str, cursor) != STB_TEXTEDIT_NEWLINE)
      cursor = IMSTB_TEXTEDIT_GETNEXTCHARINDEX(str, cursor);
   return cursor;
}
#define STB_TEXTEDIT_MOVELINEEND stb_textedit_move_line_end
#endif

#ifdef STB_TEXTEDIT_IS_SPACE
ISC is_word_boundary( IMSTB_TEXTEDIT_STRING *str, ISC idx )
{
   return idx > 0 ? (STB_TEXTEDIT_IS_SPACE( STB_TEXTEDIT_GETCHAR(str,idx-1) ) && !STB_TEXTEDIT_IS_SPACE( STB_TEXTEDIT_GETCHAR(str, idx) ) ) : 1;
}

#ifndef STB_TEXTEDIT_MOVEWORDLEFT
ISC stb_textedit_move_to_word_previous( IMSTB_TEXTEDIT_STRING *str, ISC c )
{
   c = IMSTB_TEXTEDIT_GETPREVCHARINDEX( str, c ); // always move at least one character
   while (c >= 0 && !is_word_boundary(str, c))
      c = IMSTB_TEXTEDIT_GETPREVCHARINDEX(str, c);

   if( c < 0 )
      c = 0;

   return c;
}
#define STB_TEXTEDIT_MOVEWORDLEFT stb_textedit_move_to_word_previous
#endif

#ifndef STB_TEXTEDIT_MOVEWORDRIGHT
ISC stb_textedit_move_to_word_next( IMSTB_TEXTEDIT_STRING *str, ISC c )
{
   const ISC len = STB_TEXTEDIT_STRINGLEN(str);
   c = IMSTB_TEXTEDIT_GETNEXTCHARINDEX(str, c); // always move at least one character
   while( c < len && !is_word_boundary( str, c ) )
      c = IMSTB_TEXTEDIT_GETNEXTCHARINDEX(str, c);

   if( c > len )
      c = len;

   return c;
}
#define STB_TEXTEDIT_MOVEWORDRIGHT stb_textedit_move_to_word_next
#endif

#endif

// update selection and cursor to match each other
void stb_textedit_prep_selection_at_cursor(STB_TexteditState *state)
{
   if (!STB_TEXT_HAS_SELECTION(state))
      state->select_start = state->select_end = state->cursor;
   else
      state->cursor = state->select_end;
}

// API cut: delete selection
ISC stb_textedit_cut(IMSTB_TEXTEDIT_STRING *str, STB_TexteditState *state)
{
   if (STB_TEXT_HAS_SELECTION(state)) {
      stb_textedit_delete_selection(str,state); // implicitly clamps
      state->has_preferred_x = 0;
      return 1;
   }
   return 0;
}

// API paste: replace existing selection with passed-in text
ISC stb_textedit_paste_internal(IMSTB_TEXTEDIT_STRING *str, STB_TexteditState *state, IMSTB_TEXTEDIT_CHARTYPE *text, ISC len)
{
   // if there's a selection, the paste should delete it
   stb_textedit_clamp(str, state);
   stb_textedit_delete_selection(str,state);
   // try to insert the characters
   len = STB_TEXTEDIT_INSERTCHARS(str, state->cursor, text, len);
   if (len) {
      stb_text_makeundo_insert(state, state->cursor, len);
      state->cursor += len;
      state->has_preferred_x = 0;
      return 1;
   }
   // note: paste failure will leave deleted selection, may be restored with an undo (see https://github.com/nothings/stb/issues/734 for details)
   return 0;
}

#ifndef STB_TEXTEDIT_KEYTYPE
#define STB_TEXTEDIT_KEYTYPE ISC
#endif

// API key: process text input
// [DEAR IMGUI] Added stb_textedit_text(), extracted out and called by stb_textedit_key() for backward compatibility.
void stb_textedit_text(IMSTB_TEXTEDIT_STRING* str, STB_TexteditState* state, const IMSTB_TEXTEDIT_CHARTYPE* text, ISC text_len)
{
   // can't add newline in single-line mode
   if (text[0] == '\n' && state->single_line)
      return;

   if (state->insert_mode && !STB_TEXT_HAS_SELECTION(state) && state->cursor < STB_TEXTEDIT_STRINGLEN(str)) {
      stb_text_makeundo_replace(str, state, state->cursor, 1, 1);
      STB_TEXTEDIT_DELETECHARS(str, state->cursor, 1);
      text_len = STB_TEXTEDIT_INSERTCHARS(str, state->cursor, text, text_len);
      if (text_len) {
         state->cursor += text_len;
         state->has_preferred_x = 0;
      }
   } else {
      stb_textedit_delete_selection(str, state); // implicitly clamps
      text_len = STB_TEXTEDIT_INSERTCHARS(str, state->cursor, text, text_len);
      if (text_len) {
         stb_text_makeundo_insert(state, state->cursor, text_len);
         state->cursor += text_len;
         state->has_preferred_x = 0;
      }
   }
}

// API key: process a keyboard input
void stb_textedit_key(IMSTB_TEXTEDIT_STRING *str, STB_TexteditState *state, STB_TEXTEDIT_KEYTYPE key)
{
retry:
   switch (key) {
      default: {
#ifdef STB_TEXTEDIT_KEYTOTEXT
         // This is not suitable for UTF-8 support.
         ISC c = STB_TEXTEDIT_KEYTOTEXT(key);
         if (c > 0) {
            IMSTB_TEXTEDIT_CHARTYPE ch = (IMSTB_TEXTEDIT_CHARTYPE)c;
            stb_textedit_text(str, state, &ch, 1);
         }
#endif
         break;
      }

#ifdef STB_TEXTEDIT_K_INSERT
      case STB_TEXTEDIT_K_INSERT:
         state->insert_mode = !state->insert_mode;
         break;
#endif

      case STB_TEXTEDIT_K_UNDO:
         stb_text_undo(str, state);
         state->has_preferred_x = 0;
         break;

      case STB_TEXTEDIT_K_REDO:
         stb_text_redo(str, state);
         state->has_preferred_x = 0;
         break;

      case STB_TEXTEDIT_K_LEFT:
         // if currently there's a selection, move cursor to start of selection
         if (STB_TEXT_HAS_SELECTION(state))
            stb_textedit_move_to_first(state);
         else
            if (state->cursor > 0)
               state->cursor = IMSTB_TEXTEDIT_GETPREVCHARINDEX(str, state->cursor);
         state->has_preferred_x = 0;
         break;

      case STB_TEXTEDIT_K_RIGHT:
         // if currently there's a selection, move cursor to end of selection
         if (STB_TEXT_HAS_SELECTION(state))
            stb_textedit_move_to_last(str, state);
         else
            state->cursor = IMSTB_TEXTEDIT_GETNEXTCHARINDEX(str, state->cursor);
         stb_textedit_clamp(str, state);
         state->has_preferred_x = 0;
         break;

      case STB_TEXTEDIT_K_LEFT | STB_TEXTEDIT_K_SHIFT:
         stb_textedit_clamp(str, state);
         stb_textedit_prep_selection_at_cursor(state);
         // move selection left
         if (state->select_end > 0)
            state->select_end = IMSTB_TEXTEDIT_GETPREVCHARINDEX(str, state->select_end);
         state->cursor = state->select_end;
         state->has_preferred_x = 0;
         break;

#ifdef STB_TEXTEDIT_MOVEWORDLEFT
      case STB_TEXTEDIT_K_WORDLEFT:
         if (STB_TEXT_HAS_SELECTION(state))
            stb_textedit_move_to_first(state);
         else {
            state->cursor = STB_TEXTEDIT_MOVEWORDLEFT(str, state->cursor);
            stb_textedit_clamp( str, state );
         }
         break;

      case STB_TEXTEDIT_K_WORDLEFT | STB_TEXTEDIT_K_SHIFT:
         if( !STB_TEXT_HAS_SELECTION( state ) )
            stb_textedit_prep_selection_at_cursor(state);

         state->cursor = STB_TEXTEDIT_MOVEWORDLEFT(str, state->cursor);
         state->select_end = state->cursor;

         stb_textedit_clamp( str, state );
         break;
#endif

#ifdef STB_TEXTEDIT_MOVEWORDRIGHT
      case STB_TEXTEDIT_K_WORDRIGHT:
         if (STB_TEXT_HAS_SELECTION(state))
            stb_textedit_move_to_last(str, state);
         else {
            state->cursor = STB_TEXTEDIT_MOVEWORDRIGHT(str, state->cursor);
            stb_textedit_clamp( str, state );
         }
         break;

      case STB_TEXTEDIT_K_WORDRIGHT | STB_TEXTEDIT_K_SHIFT:
         if( !STB_TEXT_HAS_SELECTION( state ) )
            stb_textedit_prep_selection_at_cursor(state);

         state->cursor = STB_TEXTEDIT_MOVEWORDRIGHT(str, state->cursor);
         state->select_end = state->cursor;

         stb_textedit_clamp( str, state );
         break;
#endif

      case STB_TEXTEDIT_K_RIGHT | STB_TEXTEDIT_K_SHIFT:
         stb_textedit_prep_selection_at_cursor(state);
         // move selection right
         state->select_end = IMSTB_TEXTEDIT_GETNEXTCHARINDEX(str, state->select_end);
         stb_textedit_clamp(str, state);
         state->cursor = state->select_end;
         state->has_preferred_x = 0;
         break;

      case STB_TEXTEDIT_K_DOWN:
      case STB_TEXTEDIT_K_DOWN | STB_TEXTEDIT_K_SHIFT:
      case STB_TEXTEDIT_K_PGDOWN:
      case STB_TEXTEDIT_K_PGDOWN | STB_TEXTEDIT_K_SHIFT: {
         StbFindState find;
         StbTexteditRow row;
         ISC i, j, sel = (key & STB_TEXTEDIT_K_SHIFT) != 0;
         ISC is_page = (key & ~STB_TEXTEDIT_K_SHIFT) == STB_TEXTEDIT_K_PGDOWN;
         ISC row_count = is_page ? state->row_count_per_page : 1;

         if (!is_page && state->single_line) {
            // on windows, up&down in single-line behave like left&right
            key = STB_TEXTEDIT_K_RIGHT | (key & STB_TEXTEDIT_K_SHIFT);
            goto retry;
         }

         if (sel)
            stb_textedit_prep_selection_at_cursor(state);
         else if (STB_TEXT_HAS_SELECTION(state))
            stb_textedit_move_to_last(str, state);

         // compute current position of cursor point
         stb_textedit_clamp(str, state);
         stb_textedit_find_charpos(&find, str, state->cursor, state->single_line);

         for (j = 0; j < row_count; ++j) {
            FPC x, goal_x = state->has_preferred_x ? state->preferred_x : find.x;
            ISC start = find.first_char + find.length;

            if (find.length == 0)
               break;

            // [DEAR IMGUI]
            // going down while being on the last line shouldn't bring us to that line end
            //if (STB_TEXTEDIT_GETCHAR(str, find.first_char + find.length - 1) != STB_TEXTEDIT_NEWLINE)
            //   break;

            // now find character position down a row
            state->cursor = start;
            STB_TEXTEDIT_LAYOUTROW(&row, str, state->cursor);
            x = row.x0;
            for (i=0; i < row.num_chars; ) {
               FPC dx = STB_TEXTEDIT_GETWIDTH(str, start, i);
               ISC next = IMSTB_TEXTEDIT_GETNEXTCHARINDEX(str, state->cursor);
               #ifdef IMSTB_TEXTEDIT_GETWIDTH_NEWLINE
               if (dx == IMSTB_TEXTEDIT_GETWIDTH_NEWLINE)
                  break;
               #endif
               x += dx;
               if (x > goal_x)
                  break;
               i += next - state->cursor;
               state->cursor = next;
            }
            stb_textedit_clamp(str, state);

            if (state->cursor == find.first_char + find.length)
               str->LastMoveDirectionLR = ImGuiDir_Left;
            state->has_preferred_x = 1;
            state->preferred_x = goal_x;

            if (sel)
               state->select_end = state->cursor;

            // go to next line
            find.first_char = find.first_char + find.length;
            find.length = row.num_chars;
         }
         break;
      }

      case STB_TEXTEDIT_K_UP:
      case STB_TEXTEDIT_K_UP | STB_TEXTEDIT_K_SHIFT:
      case STB_TEXTEDIT_K_PGUP:
      case STB_TEXTEDIT_K_PGUP | STB_TEXTEDIT_K_SHIFT: {
         StbFindState find;
         StbTexteditRow row;
         ISC i, j, prev_scan, sel = (key & STB_TEXTEDIT_K_SHIFT) != 0;
         ISC is_page = (key & ~STB_TEXTEDIT_K_SHIFT) == STB_TEXTEDIT_K_PGUP;
         ISC row_count = is_page ? state->row_count_per_page : 1;

         if (!is_page && state->single_line) {
            // on windows, up&down become left&right
            key = STB_TEXTEDIT_K_LEFT | (key & STB_TEXTEDIT_K_SHIFT);
            goto retry;
         }

         if (sel)
            stb_textedit_prep_selection_at_cursor(state);
         else if (STB_TEXT_HAS_SELECTION(state))
            stb_textedit_move_to_first(state);

         // compute current position of cursor point
         stb_textedit_clamp(str, state);
         stb_textedit_find_charpos(&find, str, state->cursor, state->single_line);

         for (j = 0; j < row_count; ++j) {
            FPC  x, goal_x = state->has_preferred_x ? state->preferred_x : find.x;

            // can only go up if there's a previous row
            if (find.prev_first == find.first_char)
               break;

            // now find character position up a row
            state->cursor = find.prev_first;
            STB_TEXTEDIT_LAYOUTROW(&row, str, state->cursor);
            x = row.x0;
            for (i=0; i < row.num_chars; ) {
               FPC dx = STB_TEXTEDIT_GETWIDTH(str, find.prev_first, i);
               ISC next = IMSTB_TEXTEDIT_GETNEXTCHARINDEX(str, state->cursor);
               #ifdef IMSTB_TEXTEDIT_GETWIDTH_NEWLINE
               if (dx == IMSTB_TEXTEDIT_GETWIDTH_NEWLINE)
                  break;
               #endif
               x += dx;
               if (x > goal_x)
                  break;
               i += next - state->cursor;
               state->cursor = next;
            }
            stb_textedit_clamp(str, state);

            if (state->cursor == find.first_char)
               str->LastMoveDirectionLR = ImGuiDir_Right;
            else if (state->cursor == find.prev_first)
               str->LastMoveDirectionLR = ImGuiDir_Left;
            state->has_preferred_x = 1;
            state->preferred_x = goal_x;

            if (sel)
               state->select_end = state->cursor;

            // go to previous line
            // (we need to scan previous line the hard way. maybe we could expose this as a new API function?)
            prev_scan = find.prev_first > 0 ? find.prev_first - 1 : 0;
            while (prev_scan > 0)
            {
               ISC prev = IMSTB_TEXTEDIT_GETPREVCHARINDEX(str, prev_scan);
               if (STB_TEXTEDIT_GETCHAR(str, prev) == STB_TEXTEDIT_NEWLINE)
                  break;
               prev_scan = prev;
            }
            find.first_char = find.prev_first;
            find.prev_first = STB_TEXTEDIT_MOVELINESTART(str, state, prev_scan);
         }
         break;
      }

      case STB_TEXTEDIT_K_DELETE:
      case STB_TEXTEDIT_K_DELETE | STB_TEXTEDIT_K_SHIFT:
         if (STB_TEXT_HAS_SELECTION(state))
            stb_textedit_delete_selection(str, state);
         else {
            ISC n = STB_TEXTEDIT_STRINGLEN(str);
            if (state->cursor < n)
               stb_textedit_delete(str, state, state->cursor, IMSTB_TEXTEDIT_GETNEXTCHARINDEX(str, state->cursor) - state->cursor);
         }
         state->has_preferred_x = 0;
         break;

      case STB_TEXTEDIT_K_BACKSPACE:
      case STB_TEXTEDIT_K_BACKSPACE | STB_TEXTEDIT_K_SHIFT:
         if (STB_TEXT_HAS_SELECTION(state))
            stb_textedit_delete_selection(str, state);
         else {
            stb_textedit_clamp(str, state);
            if (state->cursor > 0) {
               ISC prev = IMSTB_TEXTEDIT_GETPREVCHARINDEX(str, state->cursor);
               stb_textedit_delete(str, state, prev, state->cursor - prev);
               state->cursor = prev;
            }
         }
         state->has_preferred_x = 0;
         break;

#ifdef STB_TEXTEDIT_K_TEXTSTART2
      case STB_TEXTEDIT_K_TEXTSTART2:
#endif
      case STB_TEXTEDIT_K_TEXTSTART:
         state->cursor = state->select_start = state->select_end = 0;
         state->has_preferred_x = 0;
         break;

#ifdef STB_TEXTEDIT_K_TEXTEND2
      case STB_TEXTEDIT_K_TEXTEND2:
#endif
      case STB_TEXTEDIT_K_TEXTEND:
         state->cursor = STB_TEXTEDIT_STRINGLEN(str);
         state->select_start = state->select_end = 0;
         state->has_preferred_x = 0;
         break;

#ifdef STB_TEXTEDIT_K_TEXTSTART2
      case STB_TEXTEDIT_K_TEXTSTART2 | STB_TEXTEDIT_K_SHIFT:
#endif
      case STB_TEXTEDIT_K_TEXTSTART | STB_TEXTEDIT_K_SHIFT:
         stb_textedit_prep_selection_at_cursor(state);
         state->cursor = state->select_end = 0;
         state->has_preferred_x = 0;
         break;

#ifdef STB_TEXTEDIT_K_TEXTEND2
      case STB_TEXTEDIT_K_TEXTEND2 | STB_TEXTEDIT_K_SHIFT:
#endif
      case STB_TEXTEDIT_K_TEXTEND | STB_TEXTEDIT_K_SHIFT:
         stb_textedit_prep_selection_at_cursor(state);
         state->cursor = state->select_end = STB_TEXTEDIT_STRINGLEN(str);
         state->has_preferred_x = 0;
         break;


#ifdef STB_TEXTEDIT_K_LINESTART2
      case STB_TEXTEDIT_K_LINESTART2:
#endif
      case STB_TEXTEDIT_K_LINESTART:
         stb_textedit_clamp(str, state);
         stb_textedit_move_to_first(state);
         state->cursor = STB_TEXTEDIT_MOVELINESTART(str, state, state->cursor);
         state->has_preferred_x = 0;
         break;

#ifdef STB_TEXTEDIT_K_LINEEND2
      case STB_TEXTEDIT_K_LINEEND2:
#endif
      case STB_TEXTEDIT_K_LINEEND: {
         stb_textedit_clamp(str, state);
         stb_textedit_move_to_last(str, state);
         state->cursor = STB_TEXTEDIT_MOVELINEEND(str, state, state->cursor);
         state->has_preferred_x = 0;
         break;
      }

#ifdef STB_TEXTEDIT_K_LINESTART2
      case STB_TEXTEDIT_K_LINESTART2 | STB_TEXTEDIT_K_SHIFT:
#endif
      case STB_TEXTEDIT_K_LINESTART | STB_TEXTEDIT_K_SHIFT:
         stb_textedit_clamp(str, state);
         stb_textedit_prep_selection_at_cursor(state);
         state->cursor = STB_TEXTEDIT_MOVELINESTART(str, state, state->cursor);
         state->select_end = state->cursor;
         state->has_preferred_x = 0;
         break;

#ifdef STB_TEXTEDIT_K_LINEEND2
      case STB_TEXTEDIT_K_LINEEND2 | STB_TEXTEDIT_K_SHIFT:
#endif
      case STB_TEXTEDIT_K_LINEEND | STB_TEXTEDIT_K_SHIFT: {
         stb_textedit_clamp(str, state);
         stb_textedit_prep_selection_at_cursor(state);
         state->cursor = STB_TEXTEDIT_MOVELINEEND(str, state, state->cursor);
         state->select_end = state->cursor;
         state->has_preferred_x = 0;
         break;
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
//
//      Undo processing
//
// @OPTIMIZE: the undo/redo buffer should be circular

void stb_textedit_flush_redo(StbUndoState *state)
{
   state->redo_point = IMSTB_TEXTEDIT_UNDOSTATECOUNT;
   state->redo_char_point = IMSTB_TEXTEDIT_UNDOCHARCOUNT;
}

// discard the oldest entry in the undo list
void stb_textedit_discard_undo(StbUndoState *state)
{
   if (state->undo_point > 0) {
      // if the 0th undo state has characters, clean those up
      if (state->undo_rec[0].char_storage >= 0) {
         ISC n = state->undo_rec[0].insert_length, i;
         // delete n characters from all other records
         state->undo_char_point -= n;
         IMSTB_TEXTEDIT_memmove(state->undo_char, state->undo_char + n, (size_t) (state->undo_char_point*sizeof(IMSTB_TEXTEDIT_CHARTYPE)));
         for (i=0; i < state->undo_point; ++i)
            if (state->undo_rec[i].char_storage >= 0)
               state->undo_rec[i].char_storage -= n; // @OPTIMIZE: get rid of char_storage and infer it
      }
      --state->undo_point;
      IMSTB_TEXTEDIT_memmove(state->undo_rec, state->undo_rec+1, (size_t) (state->undo_point*sizeof(state->undo_rec[0])));
   }
}

// discard the oldest entry in the redo list--it's bad if this
// ever happens, but because undo & redo have to store the actual
// characters in different cases, the redo character buffer can
// fill up even though the undo buffer didn't
void stb_textedit_discard_redo(StbUndoState *state)
{
   ISC k = IMSTB_TEXTEDIT_UNDOSTATECOUNT-1;

   if (state->redo_point <= k) {
      // if the k'th undo state has characters, clean those up
      if (state->undo_rec[k].char_storage >= 0) {
         ISC n = state->undo_rec[k].insert_length, i;
         // move the remaining redo character data to the end of the buffer
         state->redo_char_point += n;
         IMSTB_TEXTEDIT_memmove(state->undo_char + state->redo_char_point, state->undo_char + state->redo_char_point-n, (size_t) ((IMSTB_TEXTEDIT_UNDOCHARCOUNT - state->redo_char_point)*sizeof(IMSTB_TEXTEDIT_CHARTYPE)));
         // adjust the position of all the other records to account for above memmove
         for (i=state->redo_point; i < k; ++i)
            if (state->undo_rec[i].char_storage >= 0)
               state->undo_rec[i].char_storage += n;
      }
      // now move all the redo records towards the end of the buffer; the first one is at 'redo_point'
      // [DEAR IMGUI]
      size_t move_size = (size_t)((IMSTB_TEXTEDIT_UNDOSTATECOUNT - state->redo_point - 1) * sizeof(state->undo_rec[0]));
      const CHA* buf_begin = (CHA*)state->undo_rec; (void)buf_begin;
      const CHA* buf_end   = (CHA*)state->undo_rec + sizeof(state->undo_rec); (void)buf_end;
      IM_ASSERT(((CHA*)(state->undo_rec + state->redo_point)) >= buf_begin);
      IM_ASSERT(((CHA*)(state->undo_rec + state->redo_point + 1) + move_size) <= buf_end);
      IMSTB_TEXTEDIT_memmove(state->undo_rec + state->redo_point+1, state->undo_rec + state->redo_point, move_size);

      // now move redo_point to point to the new one
      ++state->redo_point;
   }
}

StbUndoRecord *stb_text_create_undo_record(StbUndoState *state, ISC numchars)
{
   // any time we create a new undo record, we discard redo
   stb_textedit_flush_redo(state);

   // if we have no free records, we have to make room, by sliding the
   // existing records down
   if (state->undo_point == IMSTB_TEXTEDIT_UNDOSTATECOUNT)
      stb_textedit_discard_undo(state);

   // if the characters to store won't possibly fit in the buffer, we can't undo
   if (numchars > IMSTB_TEXTEDIT_UNDOCHARCOUNT) {
      state->undo_point = 0;
      state->undo_char_point = 0;
      return nullptr;
   }

   // if we don't have enough free characters in the buffer, we have to make room
   while (state->undo_char_point + numchars > IMSTB_TEXTEDIT_UNDOCHARCOUNT)
      stb_textedit_discard_undo(state);

   return &state->undo_rec[state->undo_point++];
}

IMSTB_TEXTEDIT_CHARTYPE *stb_text_createundo(StbUndoState *state, ISC pos, ISC insert_len, ISC delete_len)
{
   StbUndoRecord *r = stb_text_create_undo_record(state, insert_len);
   if (r == nullptr)
      return nullptr;

   r->where = pos;
   r->insert_length = (IMSTB_TEXTEDIT_POSITIONTYPE) insert_len;
   r->delete_length = (IMSTB_TEXTEDIT_POSITIONTYPE) delete_len;

   if (insert_len == 0) {
      r->char_storage = -1;
      return nullptr;
   } else {
      r->char_storage = state->undo_char_point;
      state->undo_char_point += insert_len;
      return &state->undo_char[r->char_storage];
   }
}

void stb_text_undo(IMSTB_TEXTEDIT_STRING *str, STB_TexteditState *state)
{
   StbUndoState *s = &state->undostate;
   StbUndoRecord u, *r;
   if (s->undo_point == 0)
      return;

   // we need to do two things: apply the undo record, and create a redo record
   u = s->undo_rec[s->undo_point-1];
   r = &s->undo_rec[s->redo_point-1];
   r->char_storage = -1;

   r->insert_length = u.delete_length;
   r->delete_length = u.insert_length;
   r->where = u.where;

   if (u.delete_length) {
      // if the undo record says to delete characters, then the redo record will
      // need to re-insert the characters that get deleted, so we need to store
      // them.

      // there are three cases:
      //    there's enough room to store the characters
      //    characters stored for *redoing* don't leave room for redo
      //    characters stored for *undoing* don't leave room for redo
      // if the last is true, we have to bail

      if (s->undo_char_point + u.delete_length >= IMSTB_TEXTEDIT_UNDOCHARCOUNT) {
         // the undo records take up too much character space; there's no space to store the redo characters
         r->insert_length = 0;
      } else {
         ISC i;

         // there's definitely room to store the characters eventually
         while (s->undo_char_point + u.delete_length > s->redo_char_point) {
            // should never happen:
            if (s->redo_point == IMSTB_TEXTEDIT_UNDOSTATECOUNT)
               return;
            // there's currently not enough room, so discard a redo record
            stb_textedit_discard_redo(s);
         }
         r = &s->undo_rec[s->redo_point-1];

         r->char_storage = s->redo_char_point - u.delete_length;
         s->redo_char_point = s->redo_char_point - u.delete_length;

         // now save the characters
         for (i=0; i < u.delete_length; ++i)
            s->undo_char[r->char_storage + i] = STB_TEXTEDIT_GETCHAR(str, u.where + i);
      }

      // now we can carry out the deletion
      STB_TEXTEDIT_DELETECHARS(str, u.where, u.delete_length);
   }

   // check type of recorded action:
   if (u.insert_length) {
      // easy case: was a deletion, so we need to insert n characters
      u.insert_length = STB_TEXTEDIT_INSERTCHARS(str, u.where, &s->undo_char[u.char_storage], u.insert_length);
      s->undo_char_point -= u.insert_length;
   }

   state->cursor = u.where + u.insert_length;

   s->undo_point--;
   s->redo_point--;
}

void stb_text_redo(IMSTB_TEXTEDIT_STRING *str, STB_TexteditState *state)
{
   StbUndoState *s = &state->undostate;
   StbUndoRecord *u, r;
   if (s->redo_point == IMSTB_TEXTEDIT_UNDOSTATECOUNT)
      return;

   // we need to do two things: apply the redo record, and create an undo record
   u = &s->undo_rec[s->undo_point];
   r = s->undo_rec[s->redo_point];

   // we KNOW there must be room for the undo record, because the redo record
   // was derived from an undo record

   u->delete_length = r.insert_length;
   u->insert_length = r.delete_length;
   u->where = r.where;
   u->char_storage = -1;

   if (r.delete_length) {
      // the redo record requires us to delete characters, so the undo record
      // needs to store the characters

      if (s->undo_char_point + u->insert_length > s->redo_char_point) {
         u->insert_length = 0;
         u->delete_length = 0;
      } else {
         ISC i;
         u->char_storage = s->undo_char_point;
         s->undo_char_point = s->undo_char_point + u->insert_length;

         // now save the characters
         for (i=0; i < u->insert_length; ++i)
            s->undo_char[u->char_storage + i] = STB_TEXTEDIT_GETCHAR(str, u->where + i);
      }

      STB_TEXTEDIT_DELETECHARS(str, r.where, r.delete_length);
   }

   if (r.insert_length) {
      // easy case: need to insert n characters
      r.insert_length = STB_TEXTEDIT_INSERTCHARS(str, r.where, &s->undo_char[r.char_storage], r.insert_length);
      s->redo_char_point += r.insert_length;
   }

   state->cursor = r.where + r.insert_length;

   s->undo_point++;
   s->redo_point++;
}

void stb_text_makeundo_insert(STB_TexteditState *state, ISC where, ISC length)
{
   stb_text_createundo(&state->undostate, where, 0, length);
}

void stb_text_makeundo_delete(IMSTB_TEXTEDIT_STRING *str, STB_TexteditState *state, ISC where, ISC length)
{
   ISC i;
   IMSTB_TEXTEDIT_CHARTYPE *p = stb_text_createundo(&state->undostate, where, length, 0);
   if (p) {
      for (i=0; i < length; ++i)
         p[i] = STB_TEXTEDIT_GETCHAR(str, where+i);
   }
}

void stb_text_makeundo_replace(IMSTB_TEXTEDIT_STRING *str, STB_TexteditState *state, ISC where, ISC old_length, ISC new_length)
{
   ISC i;
   IMSTB_TEXTEDIT_CHARTYPE *p = stb_text_createundo(&state->undostate, where, old_length, new_length);
   if (p) {
      for (i=0; i < old_length; ++i)
         p[i] = STB_TEXTEDIT_GETCHAR(str, where+i);
   }
}

// reset the state to default
void stb_textedit_clear_state(STB_TexteditState *state, ISC is_single_line)
{
   state->undostate.undo_point = 0;
   state->undostate.undo_char_point = 0;
   state->undostate.redo_point = IMSTB_TEXTEDIT_UNDOSTATECOUNT;
   state->undostate.redo_char_point = IMSTB_TEXTEDIT_UNDOCHARCOUNT;
   state->select_end = state->select_start = 0;
   state->cursor = 0;
   state->has_preferred_x = 0;
   state->preferred_x = 0;
   state->cursor_at_end_of_line = 0;
   state->initialized = 1;
   state->single_line = (IUA) is_single_line;
   state->insert_mode = 0;
   state->row_count_per_page = 0;
}

// API initialize
void stb_textedit_initialize_state(STB_TexteditState *state, ISC is_single_line)
{
   stb_textedit_clear_state(state, is_single_line);
}

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"
#endif

ISC stb_textedit_paste(IMSTB_TEXTEDIT_STRING *str, STB_TexteditState *state, IMSTB_TEXTEDIT_CHARTYPE const *ctext, ISC len)
{
   return stb_textedit_paste_internal(str, state, (IMSTB_TEXTEDIT_CHARTYPE *) ctext, len);
}

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

#endif//IMSTB_TEXTEDIT_IMPLEMENTATION

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2017 Sean Barrett
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/
