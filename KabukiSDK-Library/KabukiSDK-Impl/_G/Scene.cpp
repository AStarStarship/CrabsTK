/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G/Scene.cpp
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
*/

using namespace _G;

Scene::Scene ()
{
    head = nullptr;

    Reset ();
}

Scene::Scene (const char* pipeDesc)
:   head (nullptr)
{
    Reset ();
}
void Scene::Delete ()
{
    head.Prev = this;
    //head.Next = this);//Dont need to do this

    Layer.Prev = head;
    Layer.Next = head;
}

void Scene::Draw (const Cell& C)
{
    if (head == nullptr)
        return;

    head->Draw (C);
    head->Next->Draw (C);
}

void DeconstructScene (Layer* currentPipe)
{
    if (currentPipe == nullptr)
        return

    DeconstructScene (currentPip.Next);
}
