/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Examples/Software3DWireFrameViewer/SoftwareEngine.hpp
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

#pragma once

#include <KabukiSDK-Config.hpp>

namespace KabukiSDKExamples {

/**  */
class SoftwareEngine : _App.Application
{
    const int DefaultScreenWidth = 640,
        DefaultScreenHeight = 480,
        DefaultDelay = 50,
        MaxPolygonsPerScene = 256,
        ModeClipZ = 34,
        ModeCllipXYZ = 35;

    /** Sine and cosine look-up arrays. */
    const float[][] ZeroMatrix = {
            { (float)0, (float)0, (float)0, (float)0},
            { (float)0, (float)0, (float)0, (float)0},
            { (float)0, (float)0, (float)0, (float)0},
            { (float)0, (float)0, (float)0, (float)0}};

    /** Standard identity matrix. */
    const float[][] IdentityMatrix = {
                 { (float)1, (float)0, (float)0, (float)0},
                 { (float)0, (float)1, (float)0, (float)0},
                 { (float)0, (float)0, (float)1, (float)0},
                 { (float)0, (float)0, (float)0, (float)1}};  

    int halfScreenWidth,
        halfScreenHeight,
        //These last variables are to speed calculations up a bit.
        clipMinX = 0,
        clipMinY = 0,
        clipMaxX,
        clipMaxY,
        //clipMax X & Y will be set to the screen dimensions
        clipNearZ = 0,
        clipFarZ = 500,
        //clipNearZ is a constant were clipFarZ adjust the viewing distance cut-off
        viewingDistance = 250,    
        //
        step = 0,
        lineStep = 0;

    bool trip = false;      //< Debugging variables

    float ambientLightLevel = 5,
                      aspectRatio;      //< Ratio of screen width to height

    float[] cos, 
        sin;
    
    // Commonly used Matrices

    float[][] globalViewMatrix = new float[4][4],
        result1 = new float[4][4],
        result2 = new float[4][4];    

    _2D::Point_i_l viewPoint = new _2D::Point_i_l (0,0,0);
    Angle viewAngle = new Angle (0,0,0);

    Vector_l windVector = new Vector_l (0,0,0);

    _IO.Keyboard.Driver keyboard;

    SoftwareEngine (int screenWidth, int screenHeight, _G::Color_i_i_i backgroundColor) :
        base (screenWidth, screenWidth, DefaultDelay)
    {
        setBackground (backgroundColor);
    
        aspectRatio = screenHeight ()/screenWidth ();
    
        clipMaxX = screenWidth ();
        clipMaxY = screenHeight ();
        halfScreenWidth  = screenWidth ()/2;
        halfScreenHeight = screenHeight ()/2;
    
        buildSinCosTable ();
    }    

    void addkeyboard (Javakeyboard newkeyboard)
    {  addKeyListener (newkeyboard);
        keyboard = newkeyboard;
    }

    void Update ()
    {  keyboard.Update ();
        setUpNextFrame ();
        repaint ();
    }

    abstract void setUpNextFrame ();    
    abstract void Draw (ImageObserver driversIO, Cell& C);

    void addToPolygon3DList (Polygon3D toAdd)
    {  
    }
    void resetPolygon3DList ()
    {  
    }
    void sortPolygon3DList ()
    {  
    }

    Vector_l windVector ()
    {  return windVector;    }

    void setWindVector (Vector_l newWind)
    {  windVector = newWind;    }

    int halfScreenWidth ()
    {  return halfScreenWidth;  }

    int halfScreenHeight ()
    {  return halfScreenHeight; }

    int clipNearZ ()
    {  return clipNearZ;  }

    int clipFarZ ()
    {  return clipFarZ;  }

    int clipMinX ()
    {  return clipMinX;  }

    int clipMaxX ()
    {  return clipMaxX;  }

    int clipMinY ()
    {  return clipMinY;  }

    int clipMaxY ()
    {  return clipMaxY;  }

    int viewingDistance ()
    {  return viewingDistance; }

    float aspectRatio ()
    {  return aspectRatio;    }

    float ambientLightLevel ()
    {  return ambientLightLevel;  }

    void buildSinCosTable ()
    {  cos = sin = new float[360];
        for (int i = 0; i<360; i++)
        {  cos[i] =  (float)Math.toDegrees (Math.cos (i));
            sin[i] =  (float)Math.toDegrees (Math.sin (i));
        }
    }

    float sin (int degree)
    {  if (degree<0||degree> = sin.Length)
            return 0;
        return sin[degree];
    }

    float cos (int degree)
    {  if (degree<0||degree> = cos.Length)
            return 0;
        return cos[degree];
    }

    void createWorldToCameraMatrix ()
    {  
        float[][] translate = identityMatrix,
                    rotateX = identityMatrix,
                    rotateY = identityMatrix,
                    rotateZ = identityMatrix;
    
        translate[3][0] = -viewPoint.x;
        translate[3][1] = -viewPoint.y;
        translate[3][2] = -viewPoint.z;
    
        rotateX[1][1] =  (cos (viewAngle.x));
        rotateX[1][2] = - (sin (viewAngle.x));
        rotateX[2][1] = - (-sin (viewAngle.x));
        rotateX[2][2] =  (cos (viewAngle.x));
    
        rotateY[0][0] =  (cos (viewAngle.y));
        rotateY[0][2] = - (-sin (viewAngle.y));
        rotateY[2][0] = - (sin (viewAngle.y));
        rotateY[2][2] =  (cos (viewAngle.y));
    
        rotateZ[0][0] =  (cos (viewAngle.z));
        rotateZ[0][1] = - (sin (viewAngle.z));
        rotateZ[1][0] = - (-sin (viewAngle.z));
        rotateZ[1][1] =  (cos (viewAngle.z));
    
        multiply_4x4_by_4x4 (translate, rotateX, result1);
        multiply_4x4_by_4x4 (result1  , rotateY, result2);
        multiply_4x4_by_4x4 (result2  , rotateZ, globalViewMatrix);
    }  

    void multiply_4x4_by_4x4 (float[][] one, float[][] two, float[][] result)
    {  result[0][0] = one[0][0]*two[0][0] + one[0][1]*two[1][0] + one[0][2]*two[2][0];
        result[0][1] = one[0][0]*two[0][1] + one[0][1]*two[1][1] + one[0][2]*two[2][1];
        result[0][2] = one[0][0]*two[0][2] + one[0][1]*two[1][2] + one[0][2]*two[2][2];
        result[0][3] = 0;
    
        result[1][0] = one[1][0]*two[0][0] + one[1][1]*two[1][0] + one[1][2]*two[2][0];
        result[1][1] = one[1][0]*two[0][1] + one[1][1]*two[1][1] + one[1][2]*two[2][1];
        result[1][2] = one[1][0]*two[0][2] + one[1][1]*two[1][2] + one[1][2]*two[2][2];
        result[1][3] = 0;
    
        result[2][0] = one[2][0]*two[0][0] + one[2][1]*two[1][0] + one[2][2]*two[2][0];
        result[2][1] = one[2][0]*two[0][1] + one[2][1]*two[1][1] + one[2][2]*two[2][1];
        result[2][2] = one[2][0]*two[0][2] + one[2][1]*two[1][2] + one[2][2]*two[2][2];
        result[2][3] = 0;
    
        result[3][0] = one[3][0]*two[0][0] + one[3][1]*two[1][0] + one[3][2]*two[2][0] + two[3][0];
        result[3][1] = one[3][0]*two[0][1] + one[3][1]*two[1][1] + one[3][2]*two[2][1] + two[3][1];
        result[3][2] = one[3][0]*two[0][2] + one[3][1]*two[1][2] + one[3][2]*two[2][2] + two[3][2];
        result[3][3] = 1;
    }

    void multiply_1x4_by_4x4 (float[] one, float[][] two, float[] result)
    {  float sum;
        for (int y = 0; y<4; y++)
        {  sum = 0;
            for (int x = 0; x<4; x++)
                sum+ = one[x]*two[x][y];
            result[y] = sum;
        }                
    }

    bool drawLine (Cell& C, _G::Color_i_i_i colour, float xOne, float yOne, float xTwo, float yTwo)// returns false if the line is not drawn
    {  bool clipAlways = false, pointONE = false, pointTWO = false;
        bool rightEdge = false,leftEdge = false,topEdge = false,bottomEdge = false;
        float deltaX, deltaY, intersectX = 0, intersectY = 0;
    
        if ((xOne> = 0&&xOne< = screenWidth ())&& (yOne> = 0&&yOne< = screenHeight ()))
            pointONE = true;
        if ((xTwo> = 0&&xTwo< = screenWidth ())&& (yTwo> = 0&&yTwo< = screenHeight ()))
            pointTWO = true;
    
        if (pointONE&&pointTWO)
        {  page.setColor (colour);
            page.DrawLine ((int)xOne, (int)yOne, (int)xTwo, (int)yTwo);
            return true;
        }
    
        if (!pointONE&&!pointTWO)
         // this guy test to see if the line is in the cliping plane.
            if (((xOne<0     ) && (xTwo<0     )) || // left
                  ((xOne>screenWidth ()) && (xTwo>screenWidth ())) || // right
                  ((yOne<0     ) && (yTwo<0     )) || // top
                  ((yOne>screenHeight ()) && (yTwo>screenHeight ())) ) // bottom
            {  return false;                                 }
    
        clipAlways = true;
     // Clip point two;
        if (pointONE|| (!pointONE&&!pointTWO))
        {  deltaX = xTwo-xOne;
            deltaY = yTwo-yOne;
            if (xTwo<0)
            {  leftEdge = true;
                if (deltaX! = 0)
                    intersectY = (deltaY/deltaX) * (0-xOne) + yOne;
                else
                    intersectY = -1;
            }
            else
            if (xTwo>screenWidth ())
            {  rightEdge = true;
                if (deltaX! = 0)
                    intersectY = (deltaY/deltaX) * ((float)screenWidth ()-xOne) + yOne;
                else
                    intersectY = -1;
            }
        
            if (yTwo<0)
            {  topEdge = true;
                if (deltaY! = 0)
                    intersectX = (deltaX/deltaY) * (0 - yOne) + xOne;
                else
                    intersectX = -1;
            }
            else
            if (yTwo>screenHeight ())
            {  bottomEdge = true;
                if (deltaY! = 0)
                    intersectX = (deltaX/deltaY) * ((float)screenHeight () - yOne) + xOne;
                else
                    intersectX = -1;
            }
        
            if (rightEdge && (intersectY> = 0&&intersectY< =  (float)screenWidth ()))
            {  xTwo = (float)screenWidth ();
                yTwo = intersectY;
            }
            else
            if (leftEdge && (intersectY> = 0&&intersectY< =  (float)screenWidth ()))
            {  xTwo = 0;
                yTwo = intersectY;
            }
        
            if (bottomEdge && (intersectX> = 0&&intersectX< =  (float)screenWidth ()))
            {  xTwo = intersectX;
                yTwo = (float)screenHeight ();
            }
            else
            if (topEdge && (intersectX> = 0&&intersectX< =  (float)screenWidth ()))
            {  xTwo = intersectX;
                yTwo = 0;
            }
        }
        rightEdge = leftEdge = topEdge = bottomEdge = false;// reset flags
     // Clip point one
        if (pointTWO|| (!pointONE&&!pointTWO))
        {  deltaX = xOne-xTwo;
            deltaY = yOne-yTwo;
            if (xOne>screenWidth ())
            {  rightEdge = true;
                if (deltaX! = 0)
                    intersectY = (deltaY/deltaX) * ((float)screenWidth ()-xTwo) + yTwo;
                else
                    intersectY = -1;
            }
            else
            if (xOne<0)
            {  leftEdge = true;
                if (deltaX! = 0)
                    intersectY = (deltaY/deltaX) * (0-xTwo) + yTwo;
                else
                    intersectY = -1;
            }
            if (yOne>screenHeight ())
            {  bottomEdge = true;
                if (deltaY! = 0)
                    intersectX = (deltaX/deltaY) * ((float)screenHeight () - yTwo) + xTwo;
                else
                    intersectX = -1;
            }
            else
            if (yOne<0)
            {  topEdge = true;
                if (deltaY! = 0)
                    intersectX = (deltaX/deltaY) * (0 - yTwo) + xTwo;
                else
                    intersectX = -1;
            }
            if (rightEdge && (intersectY> = 0&&intersectY< =  (float)screenWidth ()))
            {  xOne = (float)screenWidth ();
                yOne = intersectY;
            }
            else
            if (leftEdge && (intersectY> = 0&&intersectY< =  (float)screenWidth ()))
            {  xOne = 0;
                yOne = intersectY;
            }
            if (bottomEdge && (intersectX> = 0&&intersectX< =  (float)screenWidth ()))
            {  xOne = intersectX;
                yOne = (float)screenHeight ();
            }
            else
            if (topEdge && (intersectX> = 0&&intersectX< =  (float)screenWidth ()))
            {  xOne = intersectX;
                yOne = 0;
            }
        }
        page.setColor (colour);
        page.DrawLine ((int)xOne, (int)yOne, (int)xTwo, (int)yTwo);
        return true;
    }

    void DrawTriangle (Cell& C, _G::Color_i_i_i colour, _2D::Point_i_l one, _2D::Point_i_l two, _2D::Point_i_l three)
    {
        if ((one.x! = two.x&& two.x! = three.x)|| (one.y! = two.y&& two.y! = three.y))
        {
            _2D::Point_i_l temp;            
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
            if (three.y < clipMinY || one.y > clipMaxY ||
                (one.x < clipMinX && two.x < clipMinX && three.x < clipMinX) ||
                (one.x > clipMaxX && two.x > clipMaxX && three.x > clipMaxX))
            {
                ;
            }
            else if (one.y = =  two.y)
            {
                drawTopTriangle (page, colour, one, two, three);
            }
            else if (two.y = =  three.y)
            {
                drawBottomTriangle (page, colour, one, two, three);
            }
            else
            {
                float newX = one.x + ((two.y - one.y) * (three.x - one.x) / (three.y - one.y));
                _2D::Point_i_l newPoint = new _2D::Point_i_l ((int)newX, two.y);
                drawBottomTriangle (page, colour, one, newPoint, two);
                drawTopTriangle (page, colour, two, newPoint, three);
            }
        }
    }

    void DrawTopTriangle (Cell& C, _G::Color_i_i_i colour, _2D::Point_i_l one, _2D::Point_i_l two, _2D::Point_i_l three)
    {  float deltaXright, deltaXleft, xStart, xEnd, height, right, left;
        page.setColor (colour);
        if (two.x<one.x)
        {  int temp = two.x;
            two.x = one.x;
            one.x = temp;
        }
        height = three.y-one.y;
        deltaXleft = (three.x-one.x)/height;
        deltaXright  = (three.x-two.x)/height;
        xStart = one.x;
        xEnd = two.x;
    
        if (one.y<clipMinY)// perform y cliping
        {  xStart = xStart + deltaXleft * (-one.y+clipMinY);
            xEnd    = xEnd    + deltaXright* (-one.y+clipMinY);
            one.y = clipMinY;
        }        
        // test if x cliping is nessisary
        if (one.x  > = clipMinX && one.x  < = clipMaxX &&
             two.x  > = clipMinX && two.x  < = clipMaxX &&
             three.x> = clipMinX && three.x< = clipMaxX)
        {  for (int y =  (int)one.y; y< =  (int)three.y; y++)
            {  page.DrawLine ((int)xStart, y, (int)xEnd, y);
                xStart+ = deltaXleft;
                xEnd  + = deltaXright;
            }
        }
        else
        {  for (int y =  (int)one.y; y< =  (int)three.y; y++)
            {  left = xStart;
                right =  xEnd;
                xStart+ = deltaXleft;
                xEnd  + = deltaXright;
                if (left< clipMinX)
                    left = clipMinX;
                if (right>clipMaxX)
                    right =  clipMaxX;
                page.DrawLine ((int)left, y, (int)right, y);
            }
        
        }
    }

    void DrawBottomTriangle (_G2D.Cell page, _G::Color_i_i_i colour, _2D::Point_i_l one, _2D::Point_i_l two, _2D::Point_i_l three)
    {  float deltaXright, deltaXleft, xStart, xEnd, height, right, left;
        page.setColor (colour);
        if (three.x<two.x)
        {  int temp = two.x;
            two.x = three.x;
            three.x = temp;
        }
        height = three.y-one.y;
        deltaXleft = (two.x-one.x)/height;
        deltaXright =  (three.x-one.x)/height;
        xStart = xEnd = one.x;
        if (one.y<clipMinY)// perform y cliping
        {  xStart = xStart + deltaXleft * (-one.y+clipMinY);
            xEnd    = xEnd    + deltaXright* (-one.y+clipMinY);
            one.y = clipMinY;
        }
        if (three.y>clipMaxY)
            three.y = clipMaxY;
    
        if (one.x  > = clipMinX && one.x  < = clipMaxX &&
             two.x  > = clipMinX && two.x  < = clipMaxX &&
             three.x> = clipMinX && three.x< = clipMaxX)
        {  for (int y =  (int)one.y; y< =  (int)three.y; y++)
            {  page.DrawLine ((int)xStart,y, (int)xEnd,y);
                xStart + =  deltaXleft;
                xEnd    + =  deltaXright;             
            }
        }
        else
        {  for (int y =  (int)one.y; y< = three.y; y++)
            {  left = xStart;
                right = xEnd;
                xStart + =  deltaXleft;
                xEnd    + =  deltaXright;
                if (left<clipMinX)
                    left = clipMinX;
                if (right>clipMaxX)
                    right = clipMaxX;
                page.DrawLine ((int)left,y, (int)right,y);
            }
        }
    }
};

}   //< KabukiSDKExamples
