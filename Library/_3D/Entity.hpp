/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_3D/Entity.hpp
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

#incude <KabukiSDK-Config.hpp>

namespace _3D {

class Entity
{
    public:
    
    Entity (SoftwareEngine theEngine, _3D::Point_f initPosition, string fileName);
    /*<  */
    
    Entity (SoftwareEngine theEngine, _3D::Point_f initPosition, string EntityName, _3D::Point* vertexList, 
        Polygon* polygonList);
    /*<  */
    
    int getNumVerticies ();
    /*<  */

    int getNumPolygons ();
    /*<  */
    
    int getUID ();
    /*<  */

    int getState ();
    /*<  */
    
    void setState (int value);
    /*<  */
    
    string& getName ();
    /*<  */
    
    void setName (const string& s);
    /*<  */
    
    _2D::Point_i localVertex (int thisVertex);
    /*<  */
    
    _2D::Point_i worldVertex (int thisVertex);
    /*<  */
    
    _2D::Point_i cameraVertex (int thisVertex);
    /*<  */
    
    Polygon& getPolygon (int index);
    
    /**  */
    void draw (Cell& C);
    /*<  */
    
    void drawWireFrame (Cell& C);
    /*<  */
    
    void renderEntitySolid (Cell& C);
    /*<  */
    
    _2D::Point_i Point ();
    /*<  */
    
    void setPosition (_3D::Point_f newPoint);
    /*<  */
    
    void translatePosition (float xIncrease, float yIncrease, float zIncrease);
    /*<  */
    
    void translateX (float xIncrease);
    /*<  */
    
    void translateY (float yIncrease);
    /*<  */
    
    void translateZ (float zIncrease);
    /*<  */
    
    void translateLocalToWorld ();
    /*<  */
    
    void translateWorldToCamera (float** globalView);
    /*<  */
    
    void removeBackFacesAndShade (_3D::Point_f viewpoint, _3D::Point_f lightSource);
    /*<  */
    
    void scale (float scaleFactor);
    /*<  */
    
    void rotate (int xAngle, int yAngle, int zAngle);
    /*<  */
    
    float computeRadius ();
    /*<  */
    
    void generatePolygonList ();
    /*<  */
    
    void clipObject (int mode);
    /*<  */
    
    string allCharictorsOnThisLineUntil (char thisChar);
    /*<  */
    
    bool loadPLG (string plgName);
    /*<  */
    
    void exportToPLG (string fileName);
    /*<  */
    
    bool updatePlgText ();
    /*< Updates the PLG string. */

    override string ToString ();
    
    private:

    int uid, 
        state, 
        numVerticies, 
        numPolygons;
    float radius, 
        scaleSquare;
    string name,                    //< The entity's name.
        plgText,                    //< Stores the ToString ()
        thisLine;                   //< Used by addCharictorsUntil (char) to load loading a PLG from a file.
    bool isVisible = false;
    _2D::Point_f pos;
    _2D::Point_f* localVertex,
        worldVertex,
        cameraVertex;
    Polygon_f* polygon;
    
    SoftwareEngine engine;
};
}
