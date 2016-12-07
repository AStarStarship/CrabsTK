/** Kabuki Card Games
    @file    /.../KabukiSDK-Examples/ShapesDrawingDemo/ClipLinesTest.cpp
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

#include "ClipLinesTest.hpp"

namespace KabukiSDKExamples {

class ClipLinesDemo: public _App::App, public IKeyListener
{
    final int WIDTH = 640, HEIGHT = 480, halfWIDTH = 320, halfHEIGHT = 240;
    Point satellite, center;
    
    void init ()
    {
        satellite = new Point (0,0);
        center = new Point (halfWIDTH, halfHEIGHT);
        this.addKeyListener (new DirectionListener());
        setBackground (Color.white);
        setSize (WIDTH, HEIGHT);
    }    
    
    void draw (Cell& C)
    {
        page.fillRect (0,0,WIDTH,HEIGHT);
        drawLine (center, satellite, Color.white, page);          
    }

    virtual void pressKey (KeyEvent event) override
    {
        switch (event.getVKCode())
        {  
            case KeyEvent.VK_UP:
                satellite.y -= 10;
                break;
            case KeyEvent.VK_DOWN:
                satellite.y += 10;
                break;
            case KeyEvent.VK_LEFT:
                satellite.x -= 10;
                break;
            case KeyEvent.VK_RIGHT:
                satellite.x += 10;
                break;
            case KeyEvent.VK_SPACE:
                Point2D temp = center;
                center = satellite;
                satellite = temp;
        }
        
        repaint();
    }
    virtual void typeKey (KeyEvent event) override {}
    virtual void releaseKey (KeyEvent event) override {}
}

}   //< KabukiSDKExamples
