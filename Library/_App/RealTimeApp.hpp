/** Kabuki Toolkit
    @file       /.../Source/Kabuki_SDK-Impl/_App/RealTimeApp.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    http://www.apache.org/licenses/LICENSE-2.0
    @brief      This file contains the _ class.
*/

namespace _App {

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
	{ return delay; }
    
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
				//runner.sleep (delay);
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

	virtual void draw (const _2D::Cell& C) = 0;
};
}
 