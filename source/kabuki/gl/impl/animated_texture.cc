/** Kabuki Starship
    @file       /.../Source/KabukiTheater-Impl/include/_G/AnimatedTexture.h
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

#include <_G/AnimatedTexture.h>

namespace _G {

AnimatedTexture::AnimatedTexture (const string& AName, int maxFrames)
{  
    saveFile = filename;
    animationName = AName;
    Reset ();
}

AnimatedTexture::AnimatedTexture (const string& filename, const string& aniName, int maxFrames)
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

void AnimatedTexture::setCurrentFrame (int newIndex)
{
    if (newIndex < 0)
        currentFrame=0;
    
    if (newIndex >= numFrames)
        currentFrame=numFrames-1;
    
    currentFrame = newIndex;
}

Texture& AnimatedTexture::getCurrentFrame () {  return frame[currentFrame]; }

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

_G. AnimatedTexture::getTranspancyColor () {  return transpancyColor;  }

void AnimatedTexture::setTranspancyColor (const Color& C) {  transpancyColor = newGraphics_Color;  }

void AnimatedTexture::AddFrame (Texture* newTexture)
{
    if (numFrames++==maxFrames)
    {  numFrames--;
        return;
    }
    frame[numFrames] = newFrame;
}

void AnimatedTexture::Add (_2D::AnimatedTexture* NewFrames)
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

    for (int i=0; i < NewFrames.length (); i++)
        InsertFrame (NewFrames);
}

void AnimatedTexture::RemoveLastFrame () {  currentFrameIterator.RemoveLast ();  }

int AnimatedTexture::getNumFrames () {  return frames.size ();}

vector<Texture> AnimatedTexture::toTextureArray ()
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

void AnimatedTexture::draw (Cell& c)
{
    if (currentFrame!=null)
        C.DrawTexture (C, currentFrame, base.GetUpperLeftCorner ().x, base.GetUpperLeftCorner ().Y);
}

string AnimatedTexture::toString ()
{
    string s;
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
