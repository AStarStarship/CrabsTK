/** Kabuki Starship
    @file    /.../Source/_g/engine.h
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

#pragma once

#include <KabukiTheater-Config.h>

namespace _g {

/** Super simple 3D engine. */
class _KabukiTheater_ Engine
{
public:

    enum {
        DefaultScreenWidth  = 640,
        DefaultScreenHeight = 480,
        DefaultDelay        = 50,
        MaxPolygonsPerScene = 256,
        ModeClipZ           = 34,
        ModeCllipXYZ        = 35,
        LookupTableSize     = 
    };

    /** Sine and cosine look-up arrays. */
    static const float ZeroMatrix[][] = {
            { 0.0, 0.0, 0.0, 0.0},
            { 0.0, 0.0, 0.0, 0.0},
            { 0.0, 0.0, 0.0, 0.0},
            { 0.0, 0.0, 0.0, 0.0}
    };

    /** Standard identity matrix. */
    static const float IdentityMatrix[][] = {
                 { 1.0, 0.0, 0.0, 0.0},
                 { 0.0, 1.0, 0.0, 0.0},
                 { 0.0, 0.0, 1.0, 0.0},
                 { 0.0, 0.0, 0.0, 1.0}
    };  

    int halfScreenWidth,
        halfScreenHeight,
        //These last variables are to speed calculations up a bit.
        clipMinX,
        clipMinY,
        clipMaxX,
        clipMaxY,
        //clipMax X & Y will be set to the screen dimensions
        clipNearZ,
        clipFarZ,
        //clipNearZ is a constant were clipFarZ adjust the viewing distance cut-off
        viewingDistance,    
        //
        step,
        lineStep;

    bool trip = false;      //< Debugging variables

    float ambientLightLevel = 5,
                      aspectRatio;      //< Ratio of screen width to height

    float* cosl, 
        * sinl;
    
    // Commonly used Matrices

    float[][] globalViewMatrix = new float[4][4],
        result1 = new float[4][4],
        result2 = new float[4][4];    

    _2D::Point viewPoint = new _2D::Point (0,0,0);
    Angle viewAngle = new Angle (0,0,0);

    Vector_l windVector = new Vector_l (0,0,0);

    _IO.Keyboard.Driver keyboard;

    Engine (int screenWidth, int screenHeight, _G::Color backgroundColor) :
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
    {
        cosl = sinl = new float[360];
        for (int i = 0; i<360; i++)
        {
            cosl[i] =  (float)Math.toDegrees (Math.cosl (i));
            sinl[i] =  (float)Math.toDegrees (Math.sinl (i));
        }
    }

    float sinl (int degree)
    {
        if (degree<0||degree> = sinl.Length)
            return 0;
        return sinl[degree];
    }

    float cosl (int degree)
    {
        if (degree<0||degree> = cosl.Length)
            return 0;
        return cosl[degree];
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
    
        rotateX[1][1] =  (cosl (viewAngle.x));
        rotateX[1][2] = - (sinl (viewAngle.x));
        rotateX[2][1] = - (-sinl (viewAngle.x));
        rotateX[2][2] =  (cosl (viewAngle.x));
    
        rotateY[0][0] =  (cosl (viewAngle.y));
        rotateY[0][2] = - (-sinl (viewAngle.y));
        rotateY[2][0] = - (sinl (viewAngle.y));
        rotateY[2][2] =  (cosl (viewAngle.y));
    
        rotateZ[0][0] =  (cosl (viewAngle.z));
        rotateZ[0][1] = - (sinl (viewAngle.z));
        rotateZ[1][0] = - (-sinl (viewAngle.z));
        rotateZ[1][1] =  (cosl (viewAngle.z));
    
        multiply_4x4_by_4x4 (translate, rotateX, result1);
        multiply_4x4_by_4x4 (result1  , rotateY, result2);
        multiply_4x4_by_4x4 (result2  , rotateZ, globalViewMatrix);
    }  

    void multiply_4x4_by_4x4 (float[][] one, float[][] two, float[][] result)
    {
        result[0][0] = one[0][0]*two[0][0] + one[0][1]*two[1][0] + one[0][2]*two[2][0];
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
    {
        float sum;
        for (int y = 0; y<4; y++)
        {
            sum = 0;
            for (int x = 0; x<4; x++)
                sum+ = one[x]*two[x][y];
            result[y] = sum;
        }                
    }
};

}   //< KabukiTheaterExamples
