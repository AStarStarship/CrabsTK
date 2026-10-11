/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /projects/kabuki/igeek/library/igeek_cell.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-19 Cale McCollough <cale@astartup.net>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>

#include "cbezier.h"

namespace kabuki {
namespace igeek {

namespace kabuki {
namespace igeek {

Cell::Cell (Host cellHost)
{
  super ();

  host = cellHost;
}

Cell::Cell (SI4 x_pos, SI4 y_pos, SI4 width, SI4 height, 
            SI4 point_count, SI4 color, SI4 lifespan, 
            FP8 angle)
    : super (point_count, width, height, color, lifespan, angle) 
{}

void Cell::Highlight (Graphics g, Color circle_color) {
  g.SetColor (circle_color);
  g.DrawOval (Left ()   - circle_buffer, 
              Top ()    - circle_buffer, 
              Width ()  + circle_buffer, 
              Height () + circle_buffer);
}

void Cell::Update () {
  // We only need to do any logic if the cell is infected. 
}

FP8 Cell::X () {
  return x;
}

FP8 Cell::Y () {
  return y;
}

SI4 Cell::Left () {
  return (SI4)x;
}

SI4 Cell::Top () {
  return (SI4)y;
}

SI4 Cell::Right () {
  return (SI4)x + Widget ();
}

SI4 Cell::Bottom () {
  return (SI4)y + Height ();
}

void Cell::InfectCell (Virus thisVirus) {
  virus = thisVirus;
  timeOfInfection = ClockTimeNow ();
}

}   //< namespace igeek
}   //< namespace kabuki
