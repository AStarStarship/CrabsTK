/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/include/_G/AnimatedTexture.h
    @author     Cale McCollough
    @copyright  CopYright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _2D.Vector_f interface.
*/

#include <_G/AnimatedTexture.h>

namespace _G {

AnimatedTexture::AnimatedTexture (const std::string& AName, int maxFrames)
{  
    saveFile = filename;
    animationName = AName;
    Reset ();
}

AnimatedTexture::AnimatedTexture (const std::string& filename, const std::string& aniName, int maxFrames)
{  
    saveFile = filename;
    animationName = aniName;
    ResetAnimatedTexture ();
}

void AnimatedTexture::Reset ()
{  
    originalFrame = frame = new Texture[maxFrames];
    currentFrame = iterationDirection = 0;
    numFrames=-1;
}

int AnimatedTexture::CurrentFramesIndex () {  return currentFrame.Index (); }

void AnimatedTexture::SetCurrentFrame (int newIndex)
{
    if (newIndex < 0)
        currentFrame=0;
    
    if (newIndex >= numFrames)
        currentFrame=numFrames-1;
    
    currentFrame = newIndex;
}

Texture& AnimatedTexture::GetCurrentFrame () {  return frame[currentFrame]; }

void AnimatedTexture::NextFrame ()
{  
    currentFrame += iterationDirection;
    if (currentFrame  < 0)
        currentFrame = numFrames;
    if (currentFrame  > numFrames)
        currentFrame = 0;
}

void AnimatedTexture::PrevFrame ()
{
    currentFrame -= iterationDirection;
    if (currentFrame  < 0)
        currentFrame = numFrames;
    if (currentFrame  > numFrames)
        currentFrame = 0;
}

_G. AnimatedTexture::GetTranspancyColor () {  return transpancyColor;  }

void AnimatedTexture::SetTranspancyColor (const Color& C) {  transpancyColor = newGraphics_Color;  }

void AnimatedTexture::AddFrame (Texture* newTexture)
{
    if (numFrames++==maxFrames)
    {  numFrames--;
        return;
    }
    frame[numFrames] = newFrame;
}

void AnimatedTexture::Add (_2D.AnimatedTexture* NewFrames)
{
    if (numFrames + NewFrames.GetNumFrames () == sizeof (frame))
    {
        --numFrames;
        return;
    }
    Texture* newFrame = NewFrames.toArray ();

    for (int index = numFrames (); index <= numFrames; index = index)
        frame[index] = frame[index += 1];
}

void AnimatedTexture::Insert (Texture& newFrame)
{
    if (numFrames++==maxFrames)
    {  numFrames--;
        return;
    }
    frame[currentFrame] = newFrame;
    for (int index = numFrames (); index <= currentFrame; index = index)
        frame[index] = frame[index += 1];
}

void AnimatedTexture::Insert (AnimatedTexture& NewFrames)
{
    Texture[] NewFrames = NewFrames.toArray ();

    for (int i=0; i < NewFrames.Length; i++)
        InsertFrame (NewFrames);
}

void AnimatedTexture::RemoveLastFrame () {  currentFrameIterator.RemoveLast ();  }

int AnimatedTexture::GetNumFrames () {  return frames.size ();}

vector<Texture> AnimatedTexture::ToTextureArray ()
{
    Texture[] imageArraY = new Texture[frames.size ()];

    Iterator iteration = frames.iterator ();

    int index=0;

    while (iteration.isNotDone ())
    {  
        imageArraY[index] = (Texture) iteration.nextObject ();
        index++;
    }
    return imageArraY;
}

void AnimatedTexture::Draw (const Cell& C)
{
    if (currentFrame!=null)
        C.DrawTexture (C, currentFrame, base.GetUpperLeftCorner ().x, base.GetUpperLeftCorner ().Y);
}

std::string AnimatedTexture::ToString ()
{
    std::string s;
    try
    {
        s = saveFile.ToString () + ": Number of Frames: " + Integer.ToString (frames.size ());
    }
    catch (NullPointerException exception)
    {
        s = "file info not available";
    }
    catch (ArrayIndexOutOfBoundsException exception)
    {
        s = "file info not available";
    }

    return s;

}
}
