/** Kabuki Starship
    @file    /.../KabukiTheater-Examples/Software3DWireFrameViewer/Shading.hpp
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

#pragma once

#include <KabukiTheater-Config.hpp>

#include <_/Keyboard.hpp>

#include <_G/Kabuki3D.hpp>

namespace KabukiTheaterExamples {
    
class ShadingDemo: public _G::Kabuki3D
{
    public:
    
    void init ();
    
    void draw (Cell& C);
    
    void renderShapeSolid (Entity3D renderShape, Cell& C);
    
    void removeBackFacesAndShade (Entity3D renderShape);
    
    void buildSinCosTable ();
    
    FLT sinl (SI4 degree);
    
    FLT cosl (SI4 degree);
    
    FLT* getIdentityMatrix ();
    
    FLT* getZeroMatrix ();
    
    
    class DirectionListener: public IKeyListener
    {
        void pressKey (KeyEvent e)
        {
            switch (e.getKeyCode())
            {
                case KeyEvent.VK_UP:
                    testModel.translateShape(0,1,0);
                    break;
                case KeyEvent.VK_DOWN:
                    testModel.translateShape(0,-1,0);
                    break;
                case KeyEvent.VK_LEFT:
                    testModel.translateShape(-1,0,0);
                    break;
                case KeyEvent.VK_RIGHT:
                    testModel.translateShape(1,0,0);
                    break;
                case KeyEvent.VK_PAGE_UP:
                    testModel.translateShape(0,0,1);
                    break;
                case KeyEvent.VK_PAGE_DOWN:
                    testModel.translateShape(0,0,-1);
                    break;
                case KeyEvent.VK_NUMPAD2:
                    testModel.rotate (0,-1,0);
                    break;
                case KeyEvent.VK_NUMPAD8:
                    testModel.rotate (0,1,0);
                    break;
                case KeyEvent.VK_NUMPAD4:
                    testModel.rotate (-1,0,0);
                    break;
                case KeyEvent.VK_NUMPAD6:
                    testModel.rotate (1,0,0);
                    break;
                case KeyEvent.VK_NUMPAD9:
                    testModel.scale ((FLT)2);
                    break;
                case KeyEvent.VK_NUMPAD3:
                    testModel.scale ((FLT).5);
                    break;
            }
            
            repaint();
        }
        void keyTyped (KeyEvent e) {}
        void keyReleased (KeyEvent e) {}
    }
    
    class ClickListener: public IMouseListener
    {
        void mousePressed (MouseEvent event)
        {
            ++step;
            if (trip)
                step = 0;
            if (step> = testModel.polygon.length)
                step = 0;
            repaint();
        }
        
        void mouseClicked (MouseEvent event) {}
        void mouseReleased (MouseEvent event ) {}
        void mouseEntered (MouseEvent event ) {}
        void mouseExited (MouseEvent event ) {}
    }
    
    private:
    
    enum {
        DefaultWidth = 640,
        DefaultHeight = 480,
        HalfDefaultWidth = DefaultWidth / 2,
        HalfDefaultHeight = DefaultHeight / 2,
    };
        
    SI4 clipMinX = 0,
        clipMinY = 0,
        clipMaxX = DefaultWidth,
        clipMaxY = DefaultHeight,
        clipNearZ = 0,
        clipFarZ = 640;
    FLT** cos,
        sin;
    FLT** identityMatrix, 
        **zeroMatrix;
    SI4 viewingDistance,
        aspectRatio;
    SI4 step = 0,
        lineStep = 0;
    bool trip = false;
    Entity3D testModel;
    _3D::Vector_f viewPoint,
        lightSource,
        lineOfSight;
    SI4 ambientLightLevel = 7;
}