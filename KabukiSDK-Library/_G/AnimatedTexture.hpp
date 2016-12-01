/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G/AnimatedTexture.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _2D::Vector_f interface.
*/

#include <string>
using namespace std;

#include "Color.h"
#include "Rect_i.h"

namespace _G {
    
class _G_API AnimatedTexture: public Rect_i
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
    
    void draw (const Cell& C, TextureObserver* O);
    
    void Print (I2P::Terminal& Slot);
    /*< Prints this object to a terminal. */

    private:
    
    int numFrames,              //< The number of frames in the animation.
        maxFrames,              //< The max number of frames in the buffer.
        currentFrame,           //< The current frame number.
        iterationDirection;     //< The iteration direction (forwards/backwards).

    // A number that is either a 1 or a -1 that is added to current frame in the AnimatedTexture.
    const char* animationName;
    Color_i transpancyColor;
    _Data::File saveFile;
    
    Texture* originalFrame,   //< The original Textures given to the AnimatedTexture
        frame;
};
}
