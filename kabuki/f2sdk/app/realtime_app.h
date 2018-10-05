/** Kabuki Toolkit
    @file    $kabuki-toolkit/kabuki/toolkit/app/realtime_app.h
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#pragma once
#include <pch.h>
#if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

namespace _ {

class RealTimeApp: public App
{
	public:

	int delay;
	//Thread runner;

	RealTimeApp (int width, int height, int timerDelay) 
    :   App (width, height)
	{
		delay = timerDelay;
	}

	/** Gets and sets the timer delay. */
	int getTimerDelay ()
	{
		return delay;
	}
    
    void setTimerDelay (int value)
    {
        if (value < 0) return;
        delay = value;
    }

	void run ()
	{
		while (true)
		{
			try
			{
				
			}
			catch
			{
			}

			Update ();
		}
	}

	void start ()
	{
		/*
		if (runner == null)
		{
			//runner = new Thread (this);
			runner.start ();
		}
		*/
	}

	void stop ()
	{
		/*
		if (runner != null)
		{
			runner.stop ();
			runner = null;
		}
		*/
	}

	void update ()
	{
		/*
		Vect_i d = size ();
		if ((offScreenImage == null) || (d.Width != offScreenSize.GetWidth ()) || (d.Height != offScreenSize.GetHeight()))
		{
			offScreenImage = CreateImage (d.Width, d.Height);
			offScreenSize = d;
			offScreenCell = offScreenImage.GetCell ();
		}
		offScreenGraphics.ClearRect (0, 0, d.Width, d.Height);
		paint (offScreenGraphics);
		page.DrawImage (offScreenImage, 0, 0, this);
		*/
	}

	virtual void draw (const _g::Cell& c) = 0;
};
}       //< namespace _
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1
