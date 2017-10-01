/** Kabuki Theater
    @file    /.../Source-Impl/_G/SoftwareEngine.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

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

#include "SoftwareEngine.h"

namespace _g {

Engine::Engine (int screenWidth, int screenHeight, Color backgroundColor) :
    base (screenWidth, screenWidth, DefaultDelay),
    clipMinX (0),
    clipMinY (0),
    clipNearZ (0),
    clipFarZ (500,
    viewingDistance (250),
    step (0),
    lineStep (0),
    trip (false),
    ambientLightLevel (5)
{
    setBackground (backgroundColor);

    aspectRatio = screenHeight ()/screenWidth ();

    clipMaxX = screenWidth ();
    clipMaxY = screenHeight ();
    halfScreenWidth  = screenWidth ()/2;
    halfScreenHeight = screenHeight ()/2;

    buildSinCosTable ();
}    

void Engine::addkeyboard (Keyboard& newkeyboard)
{
    //addKeyListener (newkeyboard);
    keyboard = newkeyboard;
}

void Engine::update ()
{
    keyboard.Update ();
    setUpNextFrame ();
    repaint ();
}

void Engine::addToPolygon3DList (Polygon& toAdd)
{  
}
void Engine::resetPolygon3DList ()
{  
}
void Engine::sortPolygon3DList ()
{  
}

Vector_f& Engine::getWindVector ()
{
    return windVector;
}

void Engine::setWindVector (Vector_f newWind)
{
    windVector = newWind;
}

int Engine::getHalfScreenWidth ()       {  return halfScreenWidth;  }
int Engine::getHalfScreenHeight ()      {  return halfScreenHeight; }
int Engine::getClipNearZ ()             {  return clipNearZ;  }
int Engine::getClipFarZ ()              {  return clipFarZ;  }
int Engine::getClipMinX ()              {  return clipMinX;  }
    int Engine::getClipMaxX ()          {  return clipMaxX;  }
int Engine::getClipMinY ()              {  return clipMinY;  }
int Engine::getClipMaxY ()              {  return clipMaxY;  }
int Engine::getViewingDistance ()       {  return viewingDistance; }
float Engine::getAspectRatio ()         {  return aspectRatio;    }
float Engine::getAmbientLightLevel ()   {  return ambientLightLevel;  }

void buildSinCosTable ()
{
    cos = sin = new float[360];
    for (int i = 0; i<360; i++)
    {  cos[i] =  (float)Math.toDegrees (Math.cos (i));
        sin[i] =  (float)Math.toDegrees (Math.sin (i));
    }
}

float sinl (int degree)
{
    if (degree<0||degree> = LookupTableSize)
        return 0;
    return sin[degree];
}

float cosl (int degree)
{
    if (degree < 0 || degree> = LookupTableSize)
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

    rotateX[1][1] =  ( cos (viewAngle.x));
    rotateX[1][2] = -( sin (viewAngle.x));
    rotateX[2][1] = -(-sin (viewAngle.x));
    rotateX[2][2] =  ( cos (viewAngle.x));

    rotateY[0][0] =  ( cos (viewAngle.y));
    rotateY[0][2] = -(-sin (viewAngle.y));
    rotateY[2][0] = -( sin (viewAngle.y));
    rotateY[2][2] =  ( cos (viewAngle.y));

    rotateZ[0][0] =  ( cos (viewAngle.z));
    rotateZ[0][1] = -( sin (viewAngle.z));
    rotateZ[1][0] = -(-sin (viewAngle.z));
    rotateZ[1][1] =  ( cos (viewAngle.z));

    multiply_4x4_by_4x4 (translate, rotateX, result1);
    multiply_4x4_by_4x4 (result1  , rotateY, result2);
    multiply_4x4_by_4x4 (result2  , rotateZ, globalViewMatrix);
}  

void multiply_4x4_by_4x4 (float[][] one, float[][] two, float[][] result)
{
    result[0][0] = one[0][0] * two[0][0] + one[0][1] * two[1][0] + one[0][2] * two[2][0];
    result[0][1] = one[0][0] * two[0][1] + one[0][1] * two[1][1] + one[0][2] * two[2][1];
    result[0][2] = one[0][0] * two[0][2] + one[0][1] * two[1][2] + one[0][2] * two[2][2];
    result[0][3] = 0;

    result[1][0] = one[1][0] * two[0][0] + one[1][1] * two[1][0] + one[1][2] * two[2][0];
    result[1][1] = one[1][0] * two[0][1] + one[1][1] * two[1][1] + one[1][2] * two[2][1];
    result[1][2] = one[1][0] * two[0][2] + one[1][1] * two[1][2] + one[1][2] * two[2][2];
    result[1][3] = 0;

    result[2][0] = one[2][0] * two[0][0] + one[2][1] * two[1][0] + one[2][2] * two[2][0];
    result[2][1] = one[2][0] * two[0][1] + one[2][1] * two[1][1] + one[2][2] * two[2][1];
    result[2][2] = one[2][0] * two[0][2] + one[2][1] * two[1][2] + one[2][2] * two[2][2];
    result[2][3] = 0;

    result[3][0] = one[3][0] * two[0][0] + one[3][1] * two[1][0] + one[3][2] * two[2][0] + two[3][0];
    result[3][1] = one[3][0] * two[0][1] + one[3][1] * two[1][1] + one[3][2] * two[2][1] + two[3][1];
    result[3][2] = one[3][0] * two[0][2] + one[3][1] * two[1][2] + one[3][2] * two[2][2] + two[3][2];
    result[3][3] = 1;
}

void multiply_1x4_by_4x4 (float* one, float* two, float* result)
{
    float sum;
    for (int y = 0; y < 4; ++y)
    {
        sum = 0;
        for (int x = 0; x < 4; ++x)
            sum+ = one[x] * two[x][y];
        result[y] = sum;
    }                
}

}   //< _G
