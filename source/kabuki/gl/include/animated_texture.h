/** Kabuki Starship
    @file    /.../Source/KabukiTheater-Impl/_G/AnimatedTexture.h
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

#include "Color.h"
#include "Rect2D.h"

namespace _g {

_g

template<typename Type>
class AnimatedTexture: public Rect2D<Type>
/** An animated texture */
{
    public:
    
    //frame stores the originalFrame Textures after theve been altered.
    /*<  */
    
    AnimatedTexture (_Data::File fileName, int maxFrames);
    /*<  */
    
    AnimatedTexture (_Data::File fileName, string aniName, int maxFrames);
    /*<  */
    
    void reset ();
    /*<  */
    
    int getCurrentIndex ();
    /*<  */
    
    void setCurrentFrame (int newIndex);
    /*<  */
    
    Texture* getCurrentFrame ();
    /*<  */
    
    void getNextFrame ();
     /*<  */
        
    void getPrevFrame ();
    /*<  */
    
    Color* getTranspancyColor ();
    /*<  */
    
    void setTranspancyColor (Color* C);
    /*<  */
    
    void addFrame (Texture* newTexture);
    /*<  */
    
    void add (AnimatedTexture* newFrames);
    /*<  */
    
    void insert (Texture* newFrame);
    /*<  */
    
    void insert (_2D::AnimatedTexture newFrames);
    /*<  */
    
    void removeLastFrame ();
    /*<  */
    
    int getNumFrames ();
    /*<  */
    
    Texture* toTextureArray ();
    /*<  */
    
    void draw (Canvas& C, TextureObserver* O);
    
    void print ();
    /*< Prints this object to a terminal. */

    private:
    
    int numFrames,              //< The number of frames in the animation.
        maxFrames,              //< The max number of frames in the buffer.
        currentFrame,           //< The current frame number.
        iterationDirection;     //< The iteration direction (forwards/backwards).

    // A number that is either a 1 or a -1 that is added to current frame in the AnimatedTexture.
    const char* animationName;
    Color transpancyColor;
    _Data::File saveFile;
    
    Texture* originalFrame,   //< The original Textures given to the AnimatedTexture
        frame;
};

}   //< _G
