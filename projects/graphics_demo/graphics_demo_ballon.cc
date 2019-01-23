/** Kabuki Card Games
    @file    /.../KabukiTheater-Examples/ShapesDrawingDemo/BaloonTest.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

                    http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#include "BaloonTest.hpp"

namespace KabukiTheaterExamples {

class Ballon
{
   SI4 x, y, z, value, place, ballonNumber;
   Color color;

   Ballon (SI4 xValue, SI4 yValue, SI4 zValue, Color shade)
   {
     x = xValue;
     y = yValue;
     z = zValue;
     color = shade;
   }

   void setX (SI4 newX)
   {
      x = newX;
   }


   void setY (SI4 newY)
   {
      y = newY;
   }

   void setZ (SI4 newZ)
   {
      z = newZ;
   }

   SI4 getZ ()
   {
      return z;
   }

   void draw (Cell& C)
   {
      page.setColor (color);

      if (z < 0)
      {
         x = ballonNumber; 
         y = 400;
         z = 30;
      }
      page.fillOval (x, y, ((z*2)/3), z);
      page.fillRect (x+(((z*2)/3)/2)-2, y+z, (z/15)*2, (z/15));
      
      page.setColor (Color.black);
      page.drawLine (x+(((z*2)/3)/2), y+z+(z/15), x+(((z*2)/3)/2), y+z+z);
   }

   void newPos ()
   {
      Random generator = new Random();
      value = generator.nextInt(600);

      if (value < 100 || value > 450)
      {
         z--;
         y--;
      }
      if (value > 50 && value < 400)
      {
         x++;
         y--;
      }
      if (value > 400)
      {
         x--;
         y--;
      }
   }
   
} // End

}   //< KabukiTheaterExamples
