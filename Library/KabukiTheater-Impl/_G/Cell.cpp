/** Kabuki Theater
    @file       /.../Source/KabukiTheater-Impl/_G/Cell.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _G::Cell class.
*/

#include "_G/Cell.hpp"
namespace _G {}

Cell::Cell (int LeftEdge, int TopEdge, int Width, int height) :
	Entity_f ()
{ 
}

void Cell::Draw (const string& S, int x, int y)
{

}

void Cell::Draw (const string& S, int x, int y, float thisAngle)
{

}

Color Cell::getBGColor () { return backgroundColor; }
void Cell::setBGColor (Color C) { backgroundColor = C }

Color Cell::getFGColor () { return forgroundColor; }
void Cell::setFGColor (Color C) { forgroundColor = C }


void Cell::setColor (Color newColor)
{

}

_G::Font& Cell::getFont () { return font; }
void 

void Cell::DrawImage (Image thisImage, int leftEdge, int topEdge)
{

}

void Cell::DrawImage (Image thisImage, int leftEdge, int topEdge, int newWidth, int newHeight)
{

}

void Cell::DrawLine (_G::Cell canvas, double startX, double startY, double endX, double endY)
{

}

void Cell::DrawLine (_G::Cell canvas, _G::Color thisColor, int startX, int startY, int endX, int endY)
{

}

void Cell::DrawRectangle (_G::Cell canvas, _G::Color color, int leftEdge, int topEdge, int width, int height)
{

}

void Cell::DrawRectangle (_G::Cell canvas, _G::Color color, int leftEdge, int topEdge, int width, int height, int thickness)
{

}

void Cell::DrawOval (int x, int y,int width, int height)
{

}

void Cell::DrawRoundedRectangle (int x, int y, int width, int height, int arcWidth, int arcHeight)
{

}

void Cell::ClearArea (int x, int y, int width, int height)
{

}

void Cell::ClipArea (int x, int y, int width, int height)
{

}

void Cell::CopyArea (int x, int y, int width, int height, int dx, int dy)
{

}

void Cell::DrawArc (int x, int y, int width, int height, int startAngle, int arcAngle)
{

}

void Cell::DrawText (string thisText, int offset, int length, int x, int y)
{

}

void Cell::DrawPolygon (int[] xPoints, int[] yPoints, int nPoints)
{

}

void Cell::DrawPolyline (int[] xPoints, int[] yPoints, int nPoints)
{

}

void Cell::DrawRect (int x, int y, int width, int height)
{

}

void Cell::Fill (_2D::Polygon_f p)
{

}

void Cell::Draw (_2D::Polygon_f p)
{

}

void Cell::Fill3DRect (int x, int y, int width, int height, bool raised)
{

}

void Cell::FillArc (int x, int y, int width, int height, int startAngle, int arcAngle)
{

}

void Cell::FillOval (int x, int y, int width, int height)
{

}

void Cell::FillPolygon (int []xPoints, int []yPoints, int nPoints)
{

}

void Cell::FillRectangle (int x, int y, int width, int height)
{

}

void Cell::FillRound3dRectangle (int x, int y, int width, int height, int arcWidth, int arcHeight)
{

}

void Cell::setClip (int x, int y, int width, int height)
{

}

void Cell::setClip (_2D::Entity thisShape)
{

}

void Cell::setXORMode (Color newColor)
{

}

void Cell::Translate (int x, int y)
{

}

/*
Cell::DrawLine (_G::Cell canvas, float startX, float startY, float endX, float endY)
{
	// This routing clips the line to the drawSpace boundary returns false if the line is not drawn

	bool startPointInScreenBounds = false,
			endPointInScreenBounds = false, 
			leftEdgeClip = false,
			rightEdgeClip = false,
			topEdgeClip = false,
			bottomEdgeClip = false;

	float deltaX,       //< The intersecting 
		  deltaY,
		  intersectX,
		  intersectY;

	if (startX < 0 || startX > getScreenWidth () || startY < 0 || startY > getScreenHeight ())
		startPointInScreenBounds = false;
	
	if (endX < 0 || endX > getScreenWidth () || endY < 0 || endY > getScreenHeight ())
		endPointInScreenBounds = false;

	if (startPointInScreenBounds && endPointInScreenBounds)
	{ 
		context.setColor (color);
		context.DrawLine ((int)startX, (int)startY, (int)endX, (int)endY);
		return;
	}

	if (! (startPointInScreenBounds || endPointInScreenBounds))
	{
		/// This guy test to see if the line is in the clipping plane.
		if (   ((startX < 0             ) && (endX < 0             ))// left
			 || ((startX > ScreenWidth  ()) && (endX > ScreenWidth  ()))// right
			 || ((startY < 0             ) && (endY < 0             ))// top
			 || ((startY > ScreenHeight ()) && (endY > ScreenHeight ()))// bottom
		   )
		{
			return;
		}
	}

	if (startPointInScreenBounds || ! (startPointInScreenBounds || endPointInScreenBounds))
	{
		Clip point two;
		deltaX = endX-startX;
		deltaY = endY-startY;
		if (endX<0)
		{
			leftEdgeClip = true;
			if (deltaX! = 0)
				intersectY = (deltaY / deltaX)  (0-startX) + startY;
			else
				intersectY = -1;
		}
		else
		if (endX>ScreenWidth ())
		{  rightEdgeClip = true;
			if (deltaX! = 0)
				intersectY = (deltaY / deltaX)  ((float)ScreenWidth ()-startX) + startY;
			else
				intersectY = -1;
		}
	
		if (endY<0)
		{  topEdgeClip = true;
			if (deltaY! = 0)
				intersectX = (deltaX/deltaY)  (0 - startY) + startX;
			else
				intersectX = -1;
		}
		else
		if (endY>ScreenHeight ())
		{  bottomEdgeClip = true;
			if (deltaY! = 0)
				intersectX = (deltaX / deltaY)  ((float)ScreenHeight () - startY) + startX;
			else
				intersectX = -1;
		}
	
		if (rightEdgeClip && (intersectY > =  0 && intersectY < =  (float)ScreenWidth ()))
		{  endX = (float)ScreenWidth ();
			endY = intersectY;
		}
		else
		if (leftEdgeClip && (intersectY > =  0&& intersectY < =  (float)ScreenWidth ()))
		{  endX = 0;
			endY = intersectY;
		}
	
		if (bottomEdgeClip && (intersectX > =  0 &&intersectX < =  (float)ScreenWidth ()))
		{  endX = intersectX;
			endY = (float)ScreenHeight ();
		}
		else
		if (topEdgeClip && (intersectX > =  0 && intersectX < =  (float)ScreenWidth ()))
		{  endX = intersectX;
			endY = 0;
		}
	}

	//Reset clip flags
	rightEdgeClip = leftEdgeClip = topEdgeClip = bottomEdgeClip = false;

	if (endPointInScreenBounds || (!startPointInScreenBounds && !endPointInScreenBounds))
	{ 
		// Clip point one
		deltaX = startX-endX;
		deltaY = startY-endY;
	
		if (startX>ScreenWidth ())
		{
			rightEdgeClip = true;
			if (deltaX! = 0)
				intersectY = (deltaY/deltaX)  ((float)ScreenWidth ()-endX) + endY;
			else
				intersectY = -1;
		}
		else if (startX<0)
		{
			leftEdgeClip = true;
			if (deltaX! = 0)
				intersectY = (deltaY/deltaX)  (0-endX) + endY;
			else
				intersectY = -1;
		}
	
		if (startY>ScreenHeight ())
		{
			bottomEdgeClip = true;
			if (deltaY! = 0)
				intersectX = (deltaX/deltaY)  ((float)ScreenHeight () - endY) + endX;
			else
				intersectX = -1;
		}
		else if (startY<0)
		{
			topEdgeClip = true;
			if (deltaY! = 0)
				intersectX = (deltaX/deltaY)  (0 - endY) + endX;
			else
				intersectX = -1;
		}
	
		if (rightEdgeClip && (intersectY > =  0&&intersectY < =  (float)ScreenWidth ()))
		{
			startX = (float)ScreenWidth ();
			startY = intersectY;
		}
		else if (leftEdgeClip && (intersectY > =  0 && intersectY < =  (float)ScreenWidth ()))
		{
			startX = 0;
			startY = intersectY;
		}
	
		if (bottomEdgeClip && (intersectX > =  0 && intersectX < =  (float)ScreenWidth ()))
		{
			startX = intersectX;
			startY = (float)ScreenHeight ();
		}
		else if (topEdgeClip && (intersectX > =  0 && intersectX < =  (float)ScreenWidth ()))
		{
			startX = intersectX;
			startY = 0;
		}
	}
	DrawLinePreClipped (drawSpace, color, (int)startX, (int)startY, (int)endX, (int)endY);
}

void Cell::DrawLine (_G::Cell canvas, _G::Color color, int leftEdge, int topEdge, int width, int height)
{
	drawLine (drawSpace, color, (float)leftEdge, (float)topEdge, (float)width, (float)height);
}

void Cell::DrawRectangle (_G::Cell canvas, _G::Color color, int leftEdge, int topEdge, int width, int height)
{
	for (int y = 0; y < height; y++)
		drawLine (drawSpace, color, leftEdge, topEdge, width, height);
}

void Cell::DrawRainbowLine (_G::Cell canvas, int startX, int startY, int endX, int endY)
{
	int lineLength = Math.sqrt ((startX + endX) / (startY + endY));

	for (int i = 0; i < lineLength; i++)
	{
		context.setColor (Color.Red);
		context.DrawLine (i, 0, i, 320);
	}
}

void Cell::DrawPixel (_G::* color, int pixelX, int pixelY)
{
	Image.Draw_pixel (_2D::Rectangle<_2D::Point_i>.Left_edge_int () + pixelX,
		_2D::Rectangle<_2D::Point_i>.Top_edge_int ()  + pixelY, color);
}

void Cell::DrawImage (Image* image, int leftEdge, int topEdge)
{  
	Image.Draw (this, image,
					 _2D::Rectangle<_2D::Point_i>.Left_edge_int () + leftEdge,
					_2D::Rectangle<_2D::Point_i>.Top_edge_int () + topEdge);
}

void Cell::DrawLine (_G::* color, int startX, int startY, int   endX, int   endY)
{
	Line::Draw (this, color, _2D::Rectangle<_2D::Point_i>.Left_edge_int () + startX,
		_2D::Rectangle<_2D::Point_i>.Top_edge_int ()  + startY,
		_2D::Rectangle<_2D::Point_i>.Left_edge_int () + endX ,
		_2D::Rectangle<_2D::Point_i>.Top_edge_int ()  + endY);
}

void Cell::DrawVector (_G::* color, int startX, int startY, int vectorX, int vectorY)
{
	int screenX, screenY;
		screenX = _2D::Rectangle<_2D::Point_i>.Left_edge_int () + startX;
		screenY = _2D::Rectangle<_2D::Point_i>.Top_edge_int ()  + startY;

	Line::Draw (this, color, screenX, screenY, screenX + vectorX, screenY + vectorY);
}

void Cell::DrawRectangle (_G::* color, int leftEdge, int topEdge, uint width, uint height)
{
	Rectangle.Draw (this, color, _2D::Rectangle<_2D::Point_i>.Left_edge_int () + leftEdge,
	_2D::Rectangle<_2D::Point_i>.Top_edge_int ()  + topEdge, width, height);
}

void Cell::FillRectangle (_G::* color, int leftEdge, int topEdge, uint width, uint height)
{
	Rectangle.Fill (this , color, _2D::Rectangle<_2D::Point_i>.Left_edge_int () + leftEdge,
		_2D::Rectangle<_2D::Point_i>.Top_edge_int () + topEdge, width, height);
}

void Cell::DrawRoundedRectangle (_G::* color, int leftEdge, int topEdge, uint width, uint height)
{
	Rectangle.Draw_rounded (this, color, _2D::Rectangle<_2D::Point_i>.Left_edge_int () + leftEdge,
		_2D::Rectangle<_2D::Point_i>.Top_edge_int ()  + topEdge, width, height);
}

void Cell::FillRoundedRectangle (_G::* color, int leftEdge, int topEdge, uint width, uint height)
{
	Rectangle.Fill_rounded (this, color, _2D::Rectangle<_2D::Point_i>.Left_edge_int () + leftEdge,
		_2D::Rectangle<_2D::Point_i>.Top_edge_int ()  + topEdge, width, height);
}
*/
