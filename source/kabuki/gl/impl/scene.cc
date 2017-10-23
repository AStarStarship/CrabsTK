/** kabuki::gl
    @file    /.../Source/KabukiTheater-Impl/_G/Scene.cpp
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

namespace _G {

Scene::Scene ()
{
    head = nullptr;

    reset ();
}

Scene::Scene (const std::string& pipeDesc)
:   head (nullptr)
{
    reset ();
}

void Scene::destroy ()
{
    head.prev = this;
    //head.Next = this);//Dont need to do this

    Layer.prev = head;
    Layer.next = head;
}

void Scene::draw (Canvas& c)
{
    if (head == nullptr)
        return;

    head->draw (c);
    head->next->draw (c);
}

void Scene::deconstructScene (Layer* currentPipe)
{
    if (currentPipe == nullptr)
        return

    DeconstructScene (currentPip.Next);
}

}   //< _G
