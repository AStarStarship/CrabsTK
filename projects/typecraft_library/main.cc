/* TypeCraft - Teaching Typing through Mining and Crafting.
@link    https://github.com/kabuki-starship/kabuki-toolkit
@file    /project/typecraft/main.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include "../Library/world.h"
#include "pch.h"

char ScanKeyboard() {
  char key = 127;

  key = _getch();

  if (key == 0 || key == -32)  // get a special key
  {
    key = _getch();

    if (key == 72)  // up arrow
    {
      key = 'u';
    } else if (key == 75)  // left arrow
    {
      key = 'l';
    } else if (key == 77)  // right
    {
      key = 'r';
    } else if (key == 80)  // down
    {
      key = 'd';
    }
  }
  return key;
}

void FrameUpdateHandler() { ScanKeyboard(); }

int main() {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  int columns, rows;

  cout << "Welcome to TypeCraft!";

  while (1) {
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
  }
  return 0;
}
