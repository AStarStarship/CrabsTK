/* Kabuki Starship
    @file /.../KabukiTheater-Examples/Software3DWireFrameViewer/HotAirBalloonDemo.hpp
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

namespace KabukiTheaterExamples {
    
class HotAirBalloonDemo: public hal.Grfx3D.SoftwareEngine
{
    SI4 MoveForwards     = _G::event.KeyEvent.VK_E,
        MoveBackwards    = _G::event.KeyEvent.VK_D,
        MoveLeft         = _G::event.KeyEvent.VK_S,
        MoveRight        = _G::event.KeyEvent.VK_F,
        LookLeft         = _G::event.KeyEvent.VK_W,
        LookRight        = _G::event.KeyEvent.VK_R,
        Jump             = _G::event.KeyEvent.VK_SPACE,
        Crouch           = _G::event.KeyEvent.VK_Z;
 //                /\--  Player Controls  --/\Image background;
    
    _G::Image background;
    
    HotAirBalloon[] balloon;
    
    HotAirBalloonDemo ();
    
    void setUpNextFrame ();
    
    void draw (_G::Cell& C);
    
    class KeyboardDriver: public JavaKeyboardDriver
    {
        KeyboardDriver ()
        {
            setKeyEvent (MoveForwards   , new MoveForwardsKey  ());
            setKeyEvent (MoveBackwards  , new MoveBackwardsKey ());
            setKeyEvent (MoveLeft       , new MoveLeftKey      ());
            setKeyEvent (MoveRight      , new MoveRightKey     ());
            setKeyEvent (Jump           , new JumpKey          ());
            setKeyEvent (Crouch         , new CrouchKey        ());     
        }
        class MoveForwardsKey: public EventTrigger
        {
            void triggerEvent ()
            {
                windVector().translateZ (1);
            }
        }
        class MoveBackwardsKey: public EventTrigger
        {
            void triggerEvent ()
            {
                windVector().translateZ (-1);
            }
        }
        class MoveLeftKey: public EventTrigger
        {
            void triggerEvent ()
            {
                windVector().translateX (-1);
            }
        }
        class MoveRightKey: public EventTrigger
        {
            void triggerEvent ()
            {
                windVector().translateX (1);
            }
        }
        class JumpKey: public EventTrigger
        {
            void triggerEvent ()
            {
                balloon[0].addHellium ();
            }
        }
        class CrouchKey: public EventTrigger
        {  void triggerEvent ()
            {    balloon[0].removeHellium ();
            }
        }        
    }
}