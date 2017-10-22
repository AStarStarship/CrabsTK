/** KabukiTheater
    @file    /.../Source/KabukiTheater/_G/Entity3D.cpp
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

#include <_G/Entity3D.h>

namespace _G {

Entity::Entity (SoftwareEngine theEngine, Point3D<Type> initPosition, std::string filename)
{
    engine = theEngine;
    Point2D_f = initPosition;
    if (loadPLG (filename))
    {
        isVisible  = true;
        for (int i = 0; i < numVerticies; i++)
        {
            cameraVertex[i] = worldVertex[i] = new Point3D<Type> ();
            cameraVertex[i].x = localVertex[i].x + pos.x;
            cameraVertex[i].y = localVertex[i].y + pos.y;
            cameraVertex[i].z = localVertex[i].z + pos.z;
        }
    }
}  

Entity::Entity (SoftwareEngine theEngine, Point3D<Type> initPosition, std::string EntityName, Point3D<Type>* verticies, 
    Polygon* polygons)
{
    engine = theEngine;
    pos = initPosition;
    name = EntityName;
    localVertex = verticies;
    polygon = polygons;

    if (UpdatePlgText ())
        isVisible = true;

    numVerticies = localVertex.Length;        
    worldVertex  = new Point3D<Type>[numVerticies];
    cameraVertex = new Point3D<Type>[numVerticies];
    for (int i = 0; i<numVerticies; i++)
    {
        cameraVertex[i] = worldVertex[i] = new Point3D<Type> ();
        cameraVertex[i].x = localVertex[i].x + pos ().x;
        cameraVertex[i].y = localVertex[i].y + pos ().y;
        cameraVertex[i].z = localVertex[i].z + pos ().z;
    }
}

int Entity::getNumVerticies ()    { return numVerticies; }
int Entity::getNumPolygons ()     { return numPolygons; }
int Entity::getUID ()             { return uid; }
int Entity::getState ()           { return state; }

void setState (int value)
{
    state = value;
}

std::string& getName ()

Point2D_f localVertex (int thisVertex)
{  return localVertex[thisVertex];
}

Point2D_f worldVertex (int thisVertex)
{
    return worldVertex[thisVertex];
}

Point2D_f cameraVertex (int thisVertex)
{
    return cameraVertex[thisVertex];
}

Polygon& Entity::getGolygon (int index)
{
    if (index < 0 || index > numPolygons) return Polygon ();
    
    return polygons[index];
}

void Entity::draw (Canas& c)
{
    drawWireFrame (c);
}

void Entity::drawWireFrame (Canas& c)
{
    Point3D<Type>[] convertedpos = new Point3D<Type>[numVerticies];
    for (int i = 0; i< numVerticies; i++)
        convertedpos[i] = new Point3D<Type> (engine.halfScreenWidth () + cameraVertex[i].x * 
            engine.viewingDistance ()/cameraVertex[i].z, engine.halfScreenHeight () - engine.aspectRatio () *
            cameraVertex[i].y * engine.viewingDistance () / cameraVertex[i].z,0);
    for (int currPoly = 0; currPoly<numPolygons-1; currPoly++)
    {
        if (polygon[currPoly].isActive ()||polygon[currPoly].isClipped ())
        {
            for (int currVertex = 0; currVertex<polygon[currPoly].numVerticies ()-1; currVertex++)
                engine.drawLine (c, polygon[currPoly].getColor (),
                    convertedpos[polygon[currPoly].vertexIndex (currVertex )].x,
                    convertedpos[polygon[currPoly].vertexIndex (currVertex )].y,
                    convertedpos[polygon[currPoly].vertexIndex (currVertex+1)].x,
                    convertedpos[polygon[currPoly].vertexIndex (currVertex+1)].y);
            engine.drawLine (c, polygon[currPoly].getColor (),
                                 convertedpos[polygon[currPoly].vertexIndex (0)].x,
                                 convertedpos[polygon[currPoly].vertexIndex (0)].y,
                                 convertedpos[polygon[currPoly].vertexIndex (polygon[currPoly].numVerticies ()-1)].x,
                                 convertedpos[polygon[currPoly].vertexIndex (polygon[currPoly].numVerticies ()-1)].y
                               );
        }
    }
}

void Entity::renderEntitySolid (Canas& c)
{
    bool polygonIsQuad = false;
    int vertexA, vertexB, vertexC, vertexD;
    Type z1, z2, z3, z4;
    Point2D_f one, two, three, four;
    Point2D_f convertedpos[numVerticies];
    for (int i = 0; i<numVerticies; ++i)
    {
        convertedpos[i] = new Point2D_f ((engine.halfScreenWidth () + cameraVertex[i].x*engine.viewingDistance () /cameraVertex[i].z), 
        (engine.halfScreenHeight () - engine.aspectRatio ()*cameraVertex[i].y*engine.viewingDistance ()/cameraVertex[i].z));
    }
    for (int currPoly = 0; currPoly<numPolygons; ++currPoly)
    {
        if (polygon[currPoly].isVisible ()||!polygon[currPoly].isClipped ())
        {
            vertexA = polygon[currPoly].vertexIndex (0);
            vertexB = polygon[currPoly].vertexIndex (1);
            vertexC = polygon[currPoly].vertexIndex (2);
            z1 = cameraVertex[vertexA].z;
            z2 = cameraVertex[vertexB].z;
            z3 = cameraVertex[vertexC].z;
            if (polygon[currPoly].numVerticies () =  = 4)
            {
                vertexD = polygon[currPoly].vertexIndex (3);
                z4 = cameraVertex[vertexD].z;
                polygonIsQuad = true;
            }
            else
            {
                z4 = z3;
                vertexD = vertexC;
            }
            if ( (   z1>engine.clipNearZ () && z2>engine.clipNearZ ()
                     && z3>engine.clipNearZ () && z4>engine.clipNearZ ()
                   )
                 ||
                    (   z1<engine.clipFarZ ()  && z2<engine.clipFarZ ()
                     && z3<engine.clipFarZ ()  && z4<engine.clipFarZ ()
                   )
               )
            {
                engine.DrawTriangle (page, polygon[currPoly].getShade (), convertedpos[vertexA],convertedpos[vertexB],convertedpos[vertexC]);
                if (polygonIsQuad)
                    engine.DrawTriangle (page, polygon[currPoly].getShade (), convertedpos[vertexA],convertedpos[vertexC],convertedpos[vertexD]);
            }
        }
    }
}

Point2D_f Entity::get2DPosition ()
{  
    return pos;
}

void Entity::setPosition (Point3D<Type> newPosition)
{
    pos = newPosition;
}    

void Entity::translatePosition (Type xIncrease, Type yIncrease, Type zIncrease)
{
    pos.x += xIncrease;
    pos.y += yIncrease;
    pos.z += zIncrease;
    for (int i = 0; i < numVerticies; i++)
    {
        cameraVertex[i].x += xIncrease;
        cameraVertex[i].y += yIncrease;
        cameraVertex[i].z += zIncrease;
    }
}

void Entity::translateX (Type xIncrease)
{
    pos.x += xIncrease;
    for (int i = 0; i<numVerticies; i++)
        cameraVertex[i].x += xIncrease;
}

void Entity::translateY (Type yIncrease)
{
    pos.y += yIncrease;
    for (int i = 0; i<numVerticies; i++)
        cameraVertex[i].y += yIncrease;
}

void Entity::translateZ (Type zIncrease)
{
    pos.z += zIncrease;
    for (int i = 0; i<numVerticies; i++)
        cameraVertex[i].z += zIncrease;
}

void Entity::translateLocalToWorld ()
{
    for (int i = 0; i < numVerticies; i++)
    {
        worldVertex[i].x = localVertex[i].x + pos.x;
        worldVertex[i].y = localVertex[i].y + pos.y;
        worldVertex[i].z = localVertex[i].z + pos.z;         
    }
    for (int i = 0; i<polygon.Length; i++)
    {  polygon[i].makeVisible ();
        polygon[i].setAsClipped ();
    }
}

void Entity::translateWorldToCamera (Type[][] globalView)
{
    for (int i = 0; i<numVerticies; i++)
    {
        cameraVertex[i].x = worldVertex[i].x * globalView[0][0]+
            worldVertex[i].y * globalView[1][0]+
            worldVertex[i].z * globalView[2][0]+
            globalView[3][0];
        cameraVertex[i].y = worldVertex[i].x * globalView[0][1]+
            worldVertex[i].y * globalView[1][1]+
            worldVertex[i].z * globalView[2][1]+
            globalView[3][1];
        cameraVertex[i].z = worldVertex[i].x * globalView[0][2]+
            worldVertex[i].y * globalView[1][2]+
            worldVertex[i].z * globalView[2][2]+
            globalView[3][2];
    }
}

void Entity::removeBackFacesAndShade (Point3D<Type> voidPoint, pos lightSource)
{
    int vertexA, vertexB, vertexC;
    Type dp,intensity;
    pos v,u,normal,lineOfSight;
    // for each polygon determine if its pointing towards or away
    std::string debug = "";

    for (int currPoly = 0; currPoly<polygon.Length; currPoly++)
    {
        vertexA = polygon[currPoly].vertexIndex (0);
        vertexB = polygon[currPoly].vertexIndex (1);
        vertexC = polygon[currPoly].vertexIndex (2);
        u = new pos (worldVertex[vertexA],worldVertex[vertexB]);
        v = new pos (worldVertex[vertexA],worldVertex[vertexC]);
        normal = v.crossProduct (u);// MUST BE VxU
    
        lineOfSight = new pos (view_3D.pos,worldVertex[vertexA]);
        dp = normal.dotProduct (lineOfSight);
        if (dp>0)
        {
            debug = debug.concat ("Polygon[" + Integer.ToString (currPoly) + "].isVisible     ");
            polygon[currPoly].makeInvisible ();
                        
            if (polygon[currPoly].shadingType () = =  Polygon.flatShading)
            {
                dp = normal.dotProduct (lightSource);
                if (dp>0)
                {
                    intensity = engine.ambientLightLevel () + (15*dp/polygon[currPoly].normalMagnitude ());
                    if (intensity>15)
                        intensity = 15;
                    polygon[currPoly].setShade (polygon[currPoly].colour ());
                    //intensity (for a 256 colour pallette)
                
                }  
                else
                    polygon[currPoly].setShade (polygon[currPoly].colour ());
                 // at ambient light
            }
            else
                polygon[currPoly].setShade (polygon[currPoly].colour ());
        }
        else
        {  polygon[currPoly].makeInvisible (); 
            debug = debug.concat ("Polygon[" + Integer.ToString (currPoly) + "].isNotVisible ");
        }
    }
    printf (debug);
}

void Entity::scale (Type scaleFactor)
{
    for (int currVertex = 0; currVertex<localVertex.Length; currVertex++)
    {
        localVertex[currVertex].x * =  scaleFactor;
        localVertex[currVertex].y * =  scaleFactor;
        localVertex[currVertex].z * =  scaleFactor;
    }
    for (int currPoly = 0; currPoly<polygon.Length; currPoly++)
        polygon[currPoly].scaleNormalMagnitude (scaleFactor);

    radius* = scaleFactor;
    scaleSquare =  scaleFactor*scaleFactor;
}

void Entity::rotate (int xAngle, int yAngle, int zAngle)
{
    int product = 0; 
    
    if (xAngle! = 0)
        product += 4;
    if (yAngle! = 0)
        product += 2;
    if (zAngle! = 0)
        product += 1;
    if (product! = 0)
    {
        Type[][] rotateX, rotateY, rotateZ, rotate;
        Type tempX,tempY,tempZ;
    
        rotate = engine.identityMatrix;
        switch (product)
        {
            case 1: // rotateZ
            {
                rotate[0][0] = (engine.cosl (zAngle));
                rotate[0][1] = (engine.sinl (zAngle));
                rotate[1][0] = (-engine.sinl (zAngle));
                rotate[1][1] = (engine.cosl (zAngle));
            
                for (int i = 0; i<numVerticies; i++)
                {
                    tempX = localVertex[i].x * rotate[0][0]+
                        localVertex[i].y * rotate[1][0];//+
                        //localVertex[i].z * rotate[2][0];
                    tempY = localVertex[i].x * rotate[0][1]+
                        localVertex[i].y * rotate[1][1];//+
                        //localVertex[i].z * rotate[2][1];
                    tempZ = localVertex[i].z;
                
                    localVertex[i].x = tempX;
                    localVertex[i].y = tempY;
                    localVertex[i].z = tempZ;
                }
            }  break;
            case 2: // rotateY
            {
                rotate[0][0] = (engine.cosl (yAngle));
                rotate[0][2] = (-engine.sinl (yAngle));
                rotate[2][0] = (engine.sinl (yAngle));
                rotate[2][2] = (engine.cosl (yAngle));
            
                for (int i = 0; i<numVerticies; i++)
                {
                    tempX = localVertex[i].x * rotate[0][0]+
                        //localVertex[i].y * rotate[1][0]+
                        localVertex[i].z * rotate[2][0];
                    tempY = localVertex[i].y;
                    tempZ = localVertex[i].x * rotate[0][1]+
                        //localVertex[i].y * rotate[1][1]+
                        localVertex[i].z * rotate[2][1];
                    localVertex[i].x = tempX;
                    localVertex[i].y = tempY;
                    localVertex[i].z = tempZ;
                }
            }  break;
            case 3: // rotate Y & Z
            {
                rotate[0][0] =  engine.cosl (yAngle)*engine.cosl (zAngle);
                rotate[0][1] =  engine.cosl (yAngle)*engine.sinl (zAngle);
                rotate[0][2] = -engine.sinl (yAngle);
            
                rotate[1][0] = -engine.sinl (zAngle);
                rotate[1][1] =  engine.cosl (zAngle);
            
                rotate[2][0] =  engine.sinl (yAngle)*engine.cosl (zAngle);
                rotate[2][1] =  engine.sinl (yAngle)*engine.sinl (zAngle);
                rotate[2][2] =  engine.cosl (yAngle);
            
                for (int i = 0; i<numVerticies; i++)
                {
                    tempX = localVertex[i].x * rotate[0][0]+
                        localVertex[i].y * rotate[1][0]+
                        localVertex[i].z * rotate[2][0];
                    tempY = localVertex[i].x * rotate[0][1]+
                        localVertex[i].y * rotate[1][1]+
                        localVertex[i].z * rotate[2][1];
                    tempZ = localVertex[i].x * rotate[0][2]+
                        //localVertex[i].y * rotate[1][2];//+
                        localVertex[i].z * rotate[2][2];
                    localVertex[i].x = tempX;
                    localVertex[i].y = tempY;
                    localVertex[i].z = tempZ;
                }
            }  break;
            case 4: // rotateX
            {
                rotate[1][1] = (engine.cosl (xAngle));
                rotate[1][2] = (engine.sinl (xAngle));
                rotate[2][1] = (-engine.sinl (xAngle));
                rotate[2][2] = (engine.cosl (xAngle));
            
                for (int i = 0; i<numVerticies; i++)
                {
                    tempX = localVertex[i].x;
                    tempY = //localVertex[i].x * rotate[0][1]+
                        localVertex[i].y * rotate[1][1]+
                        localVertex[i].z * rotate[2][1];
                    tempZ = //localVertex[i].x * rotate[0][2]+
                        localVertex[i].y * rotate[1][2]+
                        localVertex[i].z * rotate[2][2];
                    localVertex[i].x = tempX;
                    localVertex[i].y = tempY;
                    localVertex[i].z = tempZ;
                }
            }  break;
            case 5: // rotate X & Z
            {
                rotate[0][0] =  engine.cosl (zAngle);
                rotate[0][1] =  engine.sinl (zAngle);
            
                rotate[1][0] = -engine.cosl (xAngle) * engine.sinl (zAngle);
                rotate[1][1] =  engine.cosl (xAngle) * engine.cosl (zAngle);
                rotate[1][2] =  engine.sinl (xAngle);
            
                rotate[2][0] =  engine.sinl (xAngle) * engine.sinl (zAngle);
                rotate[2][1] = -engine.sinl (xAngle) * engine.cosl (zAngle);
                rotate[2][2] =  engine.cosl (xAngle);
            
            
                for (int i = 0; i<numVerticies; i++)
                {
                    tempX = localVertex[i].x * rotate[0][0]+
                        localVertex[i].y * rotate[1][0]+
                        localVertex[i].z * rotate[2][0];
                    tempY = localVertex[i].x * rotate[0][1]+
                        localVertex[i].y * rotate[1][1]+
                        localVertex[i].z * rotate[2][1];
                    tempZ = //localVertex[i].x * rotate[0][2] +
                        localVertex[i].y * rotate[1][2]+
                        localVertex[i].z * rotate[2][2];
                    localVertex[i].x = tempX;
                    localVertex[i].y = tempY;
                    localVertex[i].z = tempZ;
                }
            }  break;
            case 6:  // rotate X & Y
            {
                rotate[0][0] =  engine.cosl (yAngle);
                rotate[0][1] = -engine.sinl (yAngle);
            
                rotate[1][0] =  engine.sinl (xAngle) * engine.sinl (yAngle);
                rotate[1][1] =  engine.cosl (xAngle);
                rotate[1][2] =  engine.sinl (xAngle) * engine.cosl (yAngle);
            
                rotate[2][0] =  engine.cosl (xAngle) * engine.sinl (yAngle);
                rotate[2][1] = -engine.sinl (xAngle);
                rotate[2][2] =  engine.cosl (xAngle) * engine.cosl (yAngle);
            
                for (int i = 0; i<numVerticies; i++)
                {
                    tempX = localVertex[i].x * rotate[0][0]+
                        localVertex[i].y * rotate[1][0]+
                        localVertex[i].z * rotate[2][0];
                    tempY = //localVertex[i].x * rotate[0][1] +
                        localVertex[i].y * rotate[1][1]+
                        localVertex[i].z * rotate[2][1];
                    tempZ = localVertex[i].x * rotate[0][2]+
                        localVertex[i].y * rotate[1][2]+
                        localVertex[i].z * rotate[2][2];
                    localVertex[i].x = tempX;
                    localVertex[i].y = tempY;
                    localVertex[i].z = tempZ;
                }
                break;
            }
            case 7: // rotate X, Y & Z
            {
                rotateX  = rotateY = rotateZ = engine.identityMatrix;
                rotateX[1][1] = (engine.cosl (xAngle));
                rotateX[1][2] = (engine.sinl (xAngle));
                rotateX[2][1] = (-engine.sinl (xAngle));
                rotateX[2][2] = (engine.cosl (xAngle));
            
                rotateY[0][0] = (engine.cosl (yAngle));
                rotateY[0][2] = (-engine.sinl (yAngle));
                rotateY[2][0] = (engine.sinl (yAngle));
                rotateY[2][2] = (engine.cosl (yAngle));
                engine.multiply_4x4_by_4x4 (rotateX, rotateY, rotate);
            
                rotateZ[0][0] = (engine.cosl (zAngle));
                rotateZ[0][1] = (engine.sinl (zAngle));
                rotateZ[1][0] = (-engine.sinl (zAngle));
                rotateZ[1][1] = (engine.cosl (zAngle));
                engine.multiply_4x4_by_4x4 (rotate, rotateZ, rotate);
            
                for (int i = 0; i<numVerticies; i++)
                {
                    tempX = localVertex[i].x * rotate[0][0] +
                        localVertex[i].y * rotate[1][0]+
                        localVertex[i].z * rotate[2][0];
                    tempY = localVertex[i].x * rotate[0][1] +
                        localVertex[i].y * rotate[1][1]+
                        localVertex[i].z * rotate[2][1];
                    tempZ = localVertex[i].x * rotate[0][2] +
                        localVertex[i].y * rotate[1][2]+
                        localVertex[i].z * rotate[2][2];
                    localVertex[i].x = tempX;
                    localVertex[i].y = tempY;
                    localVertex[i].z = tempZ;
                }
            }
            break;
            
        }
    }
}    

Type Entity::computeRadius ()
{
    Type newRadius,x,y,z;
    radius = 0;
    for (int i = 0; i<localVertex.Length; i++)
    {
        x = localVertex[i].x;
        y = localVertex[i].y;
        z = localVertex[i].z;
        newRadius = (Type)Math.sqrt ((double) (x*x + y*y + z*z));
        if (newRadius > radius)
            radius = newRadius;
    }
    return radius;
}

void generatePolygonList ()
{
    for (int currPolygon = 0; currPolygon<polygon.Length; currPolygon++)
        if (polygon[currPolygon].isVisible () && !polygon[currPolygon].isClipped ())
            engine.addToPolygonList (polygon[currPolygon]);//  add to polygon list
} 

void Entity::clipObject (int mode)
{  
    Point3D<Type> one, 
        two, 
        three, 
        four;
    Type x1, y1, z1, 
        x2, y2, z2,
        x3, y3, z3,
        x4, y4, z4,
    
        x1Compair, y1Comair,
        x2Compair, y2Comair,
        x3Compair, y3Comair,
        x4Compair, y4Comair;

    if (mode = =  engine.ModeClipZ)
    {
        for (int currPolygon = 0; currPolygon<polygon.Length; currPolygon++)
        {
            z1 = cameraVertex[polygon[currPolygon].vertexIndex (0)].z;
            z2 = cameraVertex[polygon[currPolygon].vertexIndex (1)].z;
            z3 = cameraVertex[polygon[currPolygon].vertexIndex (2)].z;
            if (polygon[currPolygon].numVerticies () =  = 4)// Polygon is a Quad
                z4 = cameraVertex[polygon[currPolygon].vertexIndex (3)].z;
            else
                z4 = z3;
            if ( (z1<engine.clipNearZ () && z2<engine.clipNearZ () && z3<engine.clipNearZ () && z4<engine.clipNearZ ())
                 || (z1>engine.clipFarZ ()  && z2>engine.clipFarZ ()  && z3>engine.clipFarZ ()  && z4>engine.clipFarZ () )
               )
                polygon[currPolygon].setAsClipped ();              
        }
    }
    else // it is ModeCllipXYZ    full 3d viewing volume clipping
    {
        Type x1Comapir, y1Compair,
            x2Comapir, y2Compair,
            x3Comapir, y3Compair,
            x4Comapir, y4Compair;
    
        for (int currPolygon = 0; currPolygon<polygon.Length; currPolygon++)
        {
            x1 = cameraVertex[polygon[currPolygon].vertexIndex (0)].x;
            y1 = cameraVertex[polygon[currPolygon].vertexIndex (0)].y;
            z1 = cameraVertex[polygon[currPolygon].vertexIndex (0)].z;
        
            x2 = cameraVertex[polygon[currPolygon].vertexIndex (1)].x;
            y2 = cameraVertex[polygon[currPolygon].vertexIndex (1)].y;
            z2 = cameraVertex[polygon[currPolygon].vertexIndex (1)].z;
        
            x3 = cameraVertex[polygon[currPolygon].vertexIndex (2)].x;
            y3 = cameraVertex[polygon[currPolygon].vertexIndex (2)].y;
            z3 = cameraVertex[polygon[currPolygon].vertexIndex (2)].z;
        
            if (polygon[currPolygon].numVerticies () =  = 4)// polygon is a quad.
            {
                x4 = cameraVertex[polygon[currPolygon].vertexIndex (3)].x;
                y4 = cameraVertex[polygon[currPolygon].vertexIndex (3)].y;
                z4 = cameraVertex[polygon[currPolygon].vertexIndex (3)].z;
            
                if (! ( (z1>engine.clipNearZ () || z2>engine.clipNearZ () || z3>engine.clipNearZ () || z4>engine.clipNearZ ())
                        && (z1<engine.clipFarZ ()  || z2<engine.clipFarZ ()  || z3<engine.clipFarZ ()  || z4<engine.clipFarZ () )
                   )) // flag as clipped
                {  polygon[currPolygon].setAsClipped ();  }
            
                x1Compair = ((engine.halfScreenWidth ()*z1)/engine.viewingDistance ());
                x2Compair = ((engine.halfScreenWidth ()*z2)/engine.viewingDistance ());
                x3Compair = ((engine.halfScreenWidth ()*z3)/engine.viewingDistance ());
                x4Compair = ((engine.halfScreenWidth ()*z4)/engine.viewingDistance ());
                if (! ( (x1>-x1Compair || x2>-x1Compair || x3>-x3Compair || x4>-x4Compair)
                        && (x1< x1Compair || x2< x2Compair || x3< x3Compair || x4< x4Compair)
                   )) // flag as clipped
                {  polygon[currPolygon].setAsClipped ();  }
            
                y1Compair = ((engine.halfScreenWidth ()*z1)/engine.viewingDistance ());
                y2Compair = ((engine.halfScreenWidth ()*z2)/engine.viewingDistance ());
                y3Compair = ((engine.halfScreenWidth ()*z3)/engine.viewingDistance ());
                y4Compair = ((engine.halfScreenWidth ()*z4)/engine.viewingDistance ());
                if (! ( (y1>-y1Compair || y2>-y1Compair || y3>-y3Compair || y4>-y4Compair)
                        && (y1< y1Compair || y2< y2Compair || y3< y3Compair || y4< y4Compair)
                   ))
                {  polygon[currPolygon].setAsClipped ();  }
            }
            else // polygon must be a triangle
            {  
                if (! ( (z1>engine.clipNearZ () || z2>engine.clipNearZ () || z3>engine.clipNearZ ())
                        && (z1<engine.clipFarZ ()  || z2<engine.clipFarZ ()  || z3<engine.clipFarZ () )
                   )) // flag as clipped
                {
                    polygon[currPolygon].setAsClipped ();
                }
            
                x1Compair = ((engine.halfScreenWidth ()*z1)/engine.viewingDistance ());
                x2Compair = ((engine.halfScreenWidth ()*z2)/engine.viewingDistance ());
                x3Compair = ((engine.halfScreenWidth ()*z3)/engine.viewingDistance ());
                if (! ( (x1>-x1Compair || x2>-x1Compair || x3>-x3Compair)
                        && (x1< x1Compair || x2< x2Compair || x3< x3Compair)
                   ))
                {
                    polygon[currPolygon].setAsClipped ();
                }
            
                y1Compair = ((engine.halfScreenWidth ()*z1)/engine.viewingDistance ());
                y2Compair = ((engine.halfScreenWidth ()*z2)/engine.viewingDistance ());
                y3Compair = ((engine.halfScreenWidth ()*z3)/engine.viewingDistance ());
                if (! ( (y1>-y1Compair || y2>-y1Compair || y3>-y3Compair)
                      && (y1< y1Compair || y2< y2Compair || y3< y3Compair)
                   ))
                {
                    polygon[currPolygon].setAsClipped ();
                }
            }
        }
    }
}

std::string Entity::allCharictorsOnThisLineUntil (char thisChar)
{
    if (thisLine =  = null)
        return thisLine = "";
    std::string returnString = "";
    char charictor;

    int i = 0;
    try
    {
        while ((charictor = thisLine.charAt (i)) = =  ' ') i++;
        // removes white space
        while (true)
        {
            charictor = thisLine.charAt (i++);
            if (charictor = =  thisChar)
            {
                thisLine = thisLine.substring (i, thisLine.Length);
                return returnString;
            }
            returnString = returnString + charictor;
        }            
    }
    catch (TextIndexOutOfBoundsException e)
    {
        return thisLine;
    }  
}

bool Entity::loadPLG (std::string plgName)
{
    printf ("Loading " + plgName);

    try
    {
        BufferedReader inFile = new BufferedReader (new FileReader (plgName));
            
        if (!inFile.readLine ().equals ("plg header"))
        {
            printf ("Error loading " + plgName + ": plg header tag missing");
            return false;
        }      
        thisLine = inFile.readLine ();
        name = allCharictorsOnThisLineUntil (' ');            
        try
        {
            numVerticies = Integer.parseInt (allCharictorsOnThisLineUntil (' '));
        }
        catch (NumberFormatException e)
        {
            printf ("Error loading " + plgName + ": numVerticies not specified right");
            return false;
        }
        try
        {
            numPolygons = Integer.parseInt (allCharictorsOnThisLineUntil (' '));
        }
        catch (NumberFormatException e)
        {
            printf ("Error loading " + plgName + ": numPolygons not specified right");
            return false;
        }
        
        if (!inFile.readLine ().equals ("verticies"))
        {
            printf ("Error loading " + plgName + ":  verticies tag missing");
            return false;
        }
        plgText = (plgName + "\n" + "plg header\n" + name + " " + Integer.ToString (numVerticies) + 
            " " + Integer.ToString (numPolygons) + "\n" + "verticies\n");
    
        localVertex  = new Point3D<Type>[numVerticies];
        worldVertex  = new Point3D<Type>[numVerticies];
        cameraVertex = new Point3D<Type>[numVerticies];
    
        Type xValue, yValue, zValue;
    
        for (int h = 0; h<numVerticies; h++)
        {
            thisLine = inFile.readLine ();
            plgText = plgText + thisLine;
            try  
            {  xValue =  Float.parseFloat (allCharictorsOnThisLineUntil (','));
            }
            catch (NumberFormatException e)
            {
                printf ("Error loading " + plgName + ": vertex[" + h + "].x Value not a number");
                return false;
            }              
            try
            {  yValue = Float.parseFloat (allCharictorsOnThisLineUntil (','));
            }
            catch (NumberFormatException e)
            {
                printf ("Error loading " + plgName + ": vertex[" + h + "].y Value not a number");
                return false;
            }                  
            try
            {
                zValue =  Float.parseFloat (allCharictorsOnThisLineUntil (' '));
            }
            catch (NumberFormatException e)
            {
                printf ("Error loading " + plgName + ": vertex[" + h + "].y Value not a number");
                return false;
            }    
            localVertex[h] = new Point3D<Type> (xValue, yValue, zValue);
            plgText = plgText.concat (Float.ToString (xValue) + "," + Float.ToString (yValue) + "," + Float.ToString (zValue) + ".\n");
        }
    
        if (!inFile.readLine ().equals ("polygons"))
        {
            printf ("Error loading " + plgName + ": polygons tag missing\n");
            return false;
        }            
    
        polygon = new Polygon[numPolygons];
        Point3D<Type>* vertexpos;
        int* vertexIndex;
        int numpossInPoly,
             red,green,blue;
        for (int j = 0; j<numPolygons; j++)
        {
            thisLine = inFile.readLine ();
            plgText = plgText +thisLine;
            std::string rgb = allCharictorsOnThisLineUntil (' ');
            try
            {
                // The first 8 charictors of the polygon lines is the hexidecimal colour of the poly
                red    = Hex.ValueOf (rgb.charAt (0))*Hex.ValueOf (rgb.charAt (1));
                green = Hex.ValueOf (rgb.charAt (2))*Hex.ValueOf (rgb.charAt (3));
                blue  = Hex.ValueOf (rgb.charAt (4))*Hex.ValueOf (rgb.charAt (5));
            }
            catch (TextIndexOutOfBoundsException e)
            {
                printf ("Error loading " + plgName + ": polygon[" + j + "].colour () is not specified right");
                return false;
            }
            try
            {
                numpossInPoly = Integer.parseInt (allCharictorsOnThisLineUntil (' '));
            }
            catch (NumberFormatException e)
            {
                printf ("Error loading " + plgName + ": polygon[" + j + "].numVerticies () is not a valid number");
                return false;
            }    
        
            vertexpos = new Point3D<Type>[numpossInPoly];
            vertexIndex = new int     [numpossInPoly];
        
            for (int l = 0; l<numpossInPoly; l++)
            {
                try
                {
                    vertexIndex[l] = Integer.parseInt (allCharictorsOnThisLineUntil (' '));
                }
                catch (NumberFormatException e)
                {
                    printf ("Error loading " + plgName + ": polygon[" + j + "].vertextIndex[" + l + "] is not a valid nuber");
                    return false;
                }
                vertexpos[l] = localVertex[vertexIndex[l]];
            }
            polygon[j] = new Polygon (vertexIndex, vertexpos);
            polygon[j].setColor (new Color (red,green,blue));
            polygon[j].setAsUnclipped ();
            polygon[j].setAsOneSided ();
            try
            {
                polygon[j].computeNormal ();                        
            }
            catch (ArrayIndexOutOfBoundsException e)
            {
                printf ("Error loading " + plgName + ": polygon[" + j + "] cant access localVertex");
                return false;
            }
            polygon[j].activate ();                                         
        }
        printf ("Done reading " + plgName);
        return true;
    }
    catch (FileNotFoundException e)
    {
        plgText = plgText.concat ("Error loading " + plgName + ": File not found\n");
    }
    catch (IOException e)
    {
        plgText = plgText.concat ("Error loading " + plgName + ": File is currupted\n");
    }

    return false;
}

void Entity::exportToPLG (std::string filename)
{
    try
    {
        FileWriter fileWriter = new FileWriter (filename);
        BufferedWriter bufferedWriter = new BufferedWriter (fileWriter);
        PrintWriter output = new PrintWriter (bufferedWriter);
    
        UpdatePlgText ();
    
        output.println (plgText);
        output.close ();
    }
    catch (IOException e) {}
}

bool Entity::updatePlgText ()
{
    try
    {
        std::string Value;
        plgText =     "plg header"
                      + "\n" + name + " " + Integer.ToString (numVerticies) + " " + Integer.ToString (numPolygons)
                      + "\nverticies";
        
        std::string vertexText[3][numVerticies];
        
        int xLength = 0, yLength = 0, zLength = 0;
        for (int i = 0; i<numVerticies; i++)
        {
            vertexText[0][i] = Float.ToString (localVertex[i].x);
            vertexText[1][i] = Float.ToString (localVertex[i].y);
            vertexText[2][i] = Float.ToString (localVertex[i].z);
            if (vertexText[0][i].Length > xLength)
                xLength = vertexText[0][i].Length;
            if (vertexText[1][i].Length > yLength)
                yLength = vertexText[1][i].Length;
            if (vertexText[2][i].Length > zLength)
                zLength = vertexText[2][i].Length;
        }
        // This loop finds the longest std::string length for each variable so i can make the std::string equal length
        for (int i = 0; i<numVerticies; i++)
        {
            while (xLength ! =  vertexText[0][i].Length)
                    vertexText[0][i] = " " + vertexText[0][i];
        
            while (yLength ! =  vertexText[1][i].Length)
                    vertexText[1][i] = " " + vertexText[1][i];
        
            while (zLength ! =  vertexText[2][i].Length)
                    vertexText[2][i] = " " + vertexText[2][i];
            
        }
        // This loop adds the white space a the beginning
    
        for (int i = 0; i < numVerticies; i++)
            plgText = plgText + "\n" + vertexText[0][i] + "," + vertexText[1][i] + "," + vertexText[2][i] + " vertex[" + i + "]";
        
        plgText = plgText + "\npolygons";
    
        for (int i = 0; i < polygon.Length; i++)
        {
            plgText = plgText + "\n";
        
            int red = polygon[i].colour ().getRed ();
            int green = polygon[i].colour ().getGreen ();
            int blue = polygon[i].colour ().getBlue ();
            //int alpha = polygon[i].colour ().getAlpha ();
            plgText = plgText + Hex.hexValue (red) + Hex.hexValue (green) + Hex.hexValue (blue) + " " + Integer.ToString (polygon[i].numVerticies ()) + " ";
            for (int h = 0; h<polygon[i].numVerticies (); h++)
                plgText = plgText + Integer.ToString (polygon[i].vertexIndex (h)) + " "; 
            plgText = plgText + "polygon[" + i + "]";              
        }
        return true;
    }
    catch (NullposerException e) {}
    printf ("Error: Shits fucked up with the " + name + " model");
    return false;
}

void Entity::print (Expression& io)
{
    updatePlgText ();
    return plgText;
}

}   //< _G
