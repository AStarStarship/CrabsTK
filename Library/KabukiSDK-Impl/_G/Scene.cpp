/** Kabuki Software Development Kit
    @file       /.../Source/KabukiSDK-Impl/_G/Scene.cpp
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
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