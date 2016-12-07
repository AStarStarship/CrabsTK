/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Examples/Software3DWireFrameViewer/SoftwareEngine.cpp
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

#include "SwRenderer.hpp"

namespace _G {

bool drawLine (Cell& C, _G::Color color, float xOne, float yOne, float xTwo, float yTwo)// returns false if the line is not drawn
{
    bool clipAlways = false, 
        pointONE    = false, 
        pointTWO    = false,
        rightEdge   = false,
        leftEdge    = false,
        topEdge     = false,
        bottomEdge  = false;
        
    float deltaX, 
        deltaY, 
        intersectX = 0.0, 
        intersectY = 0.0;

    if ((xOne>= 0.0 && xOne<= screenWidth ())&& (yOne>= 0.0 && yOne<= screenHeight ()))
        pointONE = true;
    if ((xTwo>= 0.0 && xTwo<= screenWidth ())&& (yTwo>= 0.0 && yTwo<= screenHeight ()))
        pointTWO = true;

    if (pointONE&&pointTWO)
    {  page.setColor (color);
        page.drawLine ((int)xOne, (int)yOne, (int)xTwo, (int)yTwo);
        return true;
    }

    if (!pointONE && !pointTWO)
     // this guy test to see if the line is in the cliping plane.
        if (((xOne<0     ) && (xTwo<0     )) || // left
              ((xOne>screenWidth ()) && (xTwo>screenWidth ())) || // right
              ((yOne<0     ) && (yTwo<0     )) || // top
              ((yOne>screenHeight ()) && (yTwo>screenHeight ())) ) // bottom
        {
            return false;
        }

    clipAlways = true;
 // Clip point two;
    if (pointONE|| (!pointONE&&!pointTWO))
    {
        deltaX = xTwo-xOne;
        deltaY = yTwo-yOne;
        if (xTwo<0)
        {
            leftEdge = true;
            if (deltaX! = 0)
                intersectY = (deltaY/deltaX) * (0-xOne) + yOne;
            else
                intersectY = -1;
        }
        else
        if (xTwo>screenWidth ())
        {
            rightEdge = true;
            if (deltaX! = 0)
                intersectY = (deltaY/deltaX) * ((float)screenWidth ()-xOne) + yOne;
            else
                intersectY = -1;
        }
    
        if (yTwo<0)
        {
            topEdge = true;
            if (deltaY! = 0)
                intersectX = (deltaX/deltaY) * (0 - yOne) + xOne;
            else
                intersectX = -1;
        }
        else
        if (yTwo>screenHeight ())
        {
            bottomEdge = true;
            if (deltaY! = 0)
                intersectX = (deltaX/deltaY) * ((float)screenHeight () - yOne) + xOne;
            else
                intersectX = -1;
        }
    
        if (rightEdge && (intersectY>= 0&&intersectY<=  (float)screenWidth ()))
        {
            xTwo = (float)screenWidth ();
            yTwo = intersectY;
        }
        else
        if (leftEdge && (intersectY>= 0&&intersectY<=  (float)screenWidth ()))
        {
            xTwo = 0;
            yTwo = intersectY;
        }
    
        if (bottomEdge && (intersectX>= 0&&intersectX<=  (float)screenWidth ()))
        {
            xTwo = intersectX;
            yTwo = (float)screenHeight ();
        }
        else
        if (topEdge && (intersectX>= 0&&intersectX<=  (float)screenWidth ()))
        {
            xTwo = intersectX;
            yTwo = 0;
        }
    }
    rightEdge = leftEdge = topEdge = bottomEdge = false;// reset flags
 // Clip point one
    if (pointTWO|| (!pointONE&&!pointTWO))
    {  deltaX = xOne-xTwo;
        deltaY = yOne-yTwo;
        if (xOne>screenWidth ())
        {
            rightEdge = true;
            if (deltaX! = 0)
                intersectY = (deltaY/deltaX) * ((float)screenWidth ()-xTwo) + yTwo;
            else
                intersectY = -1;
        }
        else
        if (xOne<0)
        {
            leftEdge = true;
            if (deltaX! = 0)
                intersectY = (deltaY/deltaX) * (0-xTwo) + yTwo;
            else
                intersectY = -1;
        }
        if (yOne>screenHeight ())
        {
            bottomEdge = true;
            if (deltaY! = 0)
                intersectX = (deltaX/deltaY) * ((float)screenHeight () - yTwo) + xTwo;
            else
                intersectX = -1;
        }
        else
        if (yOne<0)
        {
            topEdge = true;
            if (deltaY! = 0)
                intersectX = (deltaX/deltaY) * (0 - yTwo) + xTwo;
            else
                intersectX = -1;
        }
        if (rightEdge && (intersectY>= 0&&intersectY<=  (float)screenWidth ()))
        {
            xOne = (float)screenWidth ();
            yOne = intersectY;
        }
        else
        if (leftEdge && (intersectY>= 0&&intersectY<=  (float)screenWidth ()))
        {
            xOne = 0;
            yOne = intersectY;
        }
        if (bottomEdge && (intersectX>= 0&&intersectX<=  (float)screenWidth ()))
        {
            xOne = intersectX;
            yOne = (float)screenHeight ();
        }
        else
        if (topEdge && (intersectX>= 0&&intersectX<=  (float)screenWidth ()))
        {
            xOne = intersectX;
            yOne = 0;
        }
    }
    page.setColor (color);
    page.drawLine ((int)xOne, (int)yOne, (int)xTwo, (int)yTwo);
    return true;
}

void drawTriangle (Cell& C, _G::Color color, Point2D one, Point2D two, Point2D three)
{
    if ((one.x! = two.x&& two.x! = three.x)|| (one.y! = two.y&& two.y! = three.y))
    {
        Point2D temp;            
        if (two.y < one.y)
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
        if (three.y < clipMinY || one.y > clipMaxY ||
            (one.x < clipMinX && two.x < clipMinX && three.x < clipMinX) ||
            (one.x > clipMaxX && two.x > clipMaxX && three.x > clipMaxX))
        {
            ;
        }
        else if (one.y = =  two.y)
        {
            drawTopTriangle (page, color, one, two, three);
        }
        else if (two.y = =  three.y)
        {
            drawBottomTriangle (page, color, one, two, three);
        }
        else
        {
            float newX = one.x + ((two.y - one.y) * (three.x - one.x) / (three.y - one.y));
            Point2D newPoint = new Point2D ((int)newX, two.y);
            drawBottomTriangle (page, color, one, newPoint, two);
            drawTopTriangle (page, color, two, newPoint, three);
        }
    }
}

void drawTopTriangle (Cell& C, _G::Color color, Point2D one, Point2D two, Point2D three)
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

    if (one.y<clipMinY)// perform y cliping
    {
        xStart = xStart + deltaXleft * (-one.y+clipMinY);
        xEnd    = xEnd    + deltaXright* (-one.y+clipMinY);
        one.y = clipMinY;
    }        
    // test if x cliping is nessisary
    if (one.x  >= clipMinX && one.x  <= clipMaxX &&
         two.x  >= clipMinX && two.x  <= clipMaxX &&
         three.x>= clipMinX && three.x<= clipMaxX)
    {
        for (int y = (int)one.y; y <= (int)three.y; y++)
        {
            page.DrawLine ((int)xStart, y, (int)xEnd, y);
            xStart+= deltaXleft;
            xEnd  += deltaXright;
        }
    }
    else
    {
        for (int y =  (int)one.y; y<=  (int)three.y; y++)
        {
            left = xStart;
            right =  xEnd;
            xStart+= deltaXleft;
            xEnd  += deltaXright;
            if (left< clipMinX)
                left = clipMinX;
            if (right>clipMaxX)
                right =  clipMaxX;
            page.drawLine ((int)left, y, (int)right, y);
        }
    
    }
}

void DrawBottomTriangle (_G::Cell page, _G::Color color, Point2D one, Point2D two, Point2D three)
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
    
    if (one.y < clipMinY)// perform y cliping
    {
        xStart = xStart + deltaXleft * (-one.y+clipMinY);
        xEnd    = xEnd    + deltaXright* (-one.y+clipMinY);
        one.y = clipMinY;
    }
    if (three.y>clipMaxY)
        three.y = clipMaxY;

    if (one.x  >= clipMinX && one.x  <= clipMaxX &&
         two.x  >= clipMinX && two.x  <= clipMaxX &&
         three.x>= clipMinX && three.x<= clipMaxX)
    {
        for (int y =  (int)one.y; y<=  (int)three.y; y++)
        {  page.drawLine ((int)xStart,y, (int)xEnd,y);
            xStart +=  deltaXleft;
            xEnd    +=  deltaXright;             
        }
    }
    else
    {
        for (int y = (int)one.y; y <= (int)three.y; y++)
        {
            left = xStart;
            right = xEnd;
            xStart +=  deltaXleft;
            xEnd    +=  deltaXright;
            if (left<clipMinX)
                left = clipMinX;
            if (right>clipMaxX)
                right = clipMaxX;
            page.drawLine ((int)left,y, (int)right,y);
        }
    }
}

}   //< _G
