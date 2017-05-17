/** Kabuki Card Games
    @file    /.../KabukiTheater-Examples/ShapesDrawingDemo/TrangleDemo.cpp
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

#include "TrangleDemo.hpp"

namespace KabukiTheaterExamples {

class TriangleDemo: public hal.app.Applet
{  
    int clipMinX, clipMinY, clipMaxX, clipMaxY;
    
    TriangleDemo ()
    {// 
        super (640, 480);
        setBackground (_G::Color_i.white);
        addMouseListener (new ClickListener());
      
        clipMinX = 0;
        clipMinY = 0;
        clipMaxX = screenWidth();
        clipMaxY = screenHeight();
    }
    
    void draw (_G::Cell& C)
    {
        java.util.Random generator = new java.util.Random();
        
        Color randomColor = new Color (generator.nextInt(256) < generator.nextInt(256), generator.nextInt(256));
        Point  randomPointA = new d2.Point (generator.nextInt(800)-80, generator.nextInt(600)-60),
               randomPointB = new d2.Point (generator.nextInt(800)-80, generator.nextInt(600)-60),
               randomPointC = new d2.Point (generator.nextInt(800)-80, generator.nextInt(600)-60);
        
        drawTriangle (page, randomColor, randomPointA, randomPointB, randomPointC);
    }
    
    void drawTriangle (_G::Cell& C, _G::Color_i color, d2.Point one, d2.Point two, d2.Point three)
    {
        if ((one.x! = two.x&& two.x! = three.x)||(one.y! = two.y&& two.y! = three.y))
        {
            d2.Point temp;         
            if (two.y<one.y)
            {  
                temp = two;
                two = one;
                one = temp;
            }
            if (three.y<one.y)
            {
                temp = three;
                three = one;
                one = temp;
            }
            if (three.y<two.y)
            {  
                temp = three;
                three = two;
                two = temp;
            }
            if ( three.y<clipMinY || one.y>clipMaxY || 
                (one.x<clipMinX && two.x<clipMinX && three.x<clipMinX) ||
                (one.x>clipMaxX && two.x>clipMaxX && three.x>clipMaxX))
                ;
            else
            if (one.y =  = two.y)
                drawTopTriangle (page, color, one, two, three);
            else
            if (two.y =  = three.y)
                drawBottomTriangle (page, color, one, two, three);
            else
            {
                float newX = one.x + ((two.y-one.y)*(three.x-one.x)/(three.y-one.y));
                d2.Point newPoint = new d2.Point ((int)newX, two.y);
                drawBottomTriangle (page, color, one,newPoint,two);
                drawTopTriangle (page, color, two,newPoint,three);
            }
        }
    }
    
    void drawTopTriangle (_G::Cell& C, _G::Color_i color, d2.Point one, d2.Point two, d2.Point three)
    {
        float deltaXright, deltaXleft, xStart, xEnd, height, right, left;
        page.setColor (color);
        if (two.x<one.x)
        {
            int temp = two.x;
            two.x = one.x;
            one.x = temp;
        }
        height = three.y-one.y;
        deltaXleft = (three.x-one.x)/height;
        deltaXright  = (three.x-two.x)/height;
        xStart = one.x;
        xEnd = two.x;
        
        if (one.y<clipMinY)// perform y clipping
        {
            xStart = xStart + deltaXleft *(-one.y+clipMinY);
            xEnd   = xEnd   + deltaXright*(-one.y+clipMinY);
            one.y = clipMinY;
        }      
        // test if x clipping is necessary
        if(one.x  > = clipMinX && one.x  < = clipMaxX &&
           two.x  > = clipMinX && two.x  < = clipMaxX &&
           three.x> = clipMinX && three.x< = clipMaxX )
        {
            for (int y = (int)one.y; y< = (int)three.y; y++)
            {
                page.drawLine ((int)xStart, y, (int)xEnd, y);
                xStart+ = deltaXleft;
                xEnd  + = deltaXright;
            }
        }
        else
        {
            for (int y = (int)one.y; y< = (int)three.y; y++)
            {
                left = xStart;
                right =  xEnd;
                xStart+ = deltaXleft;
                xEnd  + = deltaXright;
                if (left< clipMinX)
                left = clipMinX;
                if (right>clipMaxX)
                right =  clipMaxX;
                page.drawLine ((int)left, y, (int)right, y);
            }
        }
    }
    
    // 
    void drawBottomTriangle (_G::Cell& C, _G::Color_i color, d2.Point one, d2.Point two, d2.Point three)
    {
        float deltaXright, deltaXleft, xStart, xEnd, height, right, left;
        page.setColor (color);
        if (three.x<two.x)
        {
            int temp = two.x;
            two.x = three.x;
            three.x = temp;
        }
        height = three.y-one.y;
        deltaXleft = (two.x-one.x)/height;
        deltaXright =  (three.x-one.x)/height;
        xStart = xEnd = one.x;
        if (one.y<clipMinY)// perform y clipping
        {
            xStart = xStart + deltaXleft * (-one.y+clipMinY);
            xEnd   = xEnd   + deltaXright* (-one.y+clipMinY);
            one.y = clipMinY;
        }
        if (three.y>clipMaxY)
            three.y = clipMaxY;
      
        if (one.x  > = clipMinX && one.x  < = clipMaxX &&
            two.x  > = clipMinX && two.x  < = clipMaxX &&
            three.x> = clipMinX && three.x< = clipMaxX )
        {
            for (int y = (int)one.y; y< = (int)three.y; y++)
            {
                page.drawLine ((int)xStart,y,(int)xEnd,y);
                xStart + =  deltaXleft;
                xEnd   + =  deltaXright;          
            }
        }
        else
        {
            for (int y = (int)one.y; y< = three.y; y++)
            {
                left = xStart;
                right = xEnd;
                xStart + =  deltaXleft;
                xEnd   + =  deltaXright;
                if (left<clipMinX)
                    left = clipMinX;
                if (right>clipMaxX)
                    right = clipMaxX;
                page.drawLine ((int)left,y,(int)right,y);
            }
        }
    }
    class ClickListener: public _G::event.MouseListener
    {
        void mousePressed (_G::event.MouseEvent event)
        {
            repaint();
        }
      
      void mouseClicked  (_G::event.MouseEvent event)   {}
      void mouseReleased (_G::event.MouseEvent event )  {}
      void mouseEntered  (_G::event.MouseEvent event )  {}
      void mouseExited   (_G::event.MouseEvent event )  {}
    }
}

}   //< KabukiTheaterExamples
