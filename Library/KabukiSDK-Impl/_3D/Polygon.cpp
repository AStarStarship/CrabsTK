/** Underscore
    @file       /.../Underscore.Library/_G3D/Polygon.cs
    @author     Cale McCollough
    @copyright  Copyright 2016 Blue Storm Engineering ©
    @license    http://www.apache.org/licenses/LICENSE-2.0
    @brief      This file contains the _2D::Vector_f interface.
*/

#include <_3D/Polygon.hpp>

namespace _3D {

Polygon (int* indexOfPoints, _3D.Point* pointValues, int numPoints)
:   shadingType (FlatShading),
    isTwoSided  (false),
    isClipped   (false),
    isActive    (false),
    isVisible   (true);
{
    if (numPoints < 4)
    {
        // Create dummy polygon.
    }
    vertexIndex = indexOfPoints;
    vertexPoint = pointValues; 
    numVerticies = vertexIndex.Length;
    base.data = this;
}

_3D.Point Vertex (int forThisPoint)
{
    return vertexPoint[forThisPoint];
}

int VertexIndex (int forThisPoint)
{
    return vertexIndex[forThisPoint];
}

_3D::Point getNormal
{
    get { return normal; }
}

void ComputeNormal ()
{
    _3D.Point vectorU = new _3D.Point (vertexPoint[0], vertexPoint[1]);
    _3D.Point vectorV = new _3D.Point (vertexPoint[0], vertexPoint[2]);

    normal = vectorV.crossProduct (vectorU);

    normalMagnitude = normal.magnitude ();
}

float NormalMagnitude ()
{
    return normalMagnitude;
}

void ScaleNormalMagnitude (float scaleFactor)
{
    normalMagnitude* = scaleFactor;
}

float getZAverage ()
{
    return zAverage;
}

void ComputeZAverge ()
{
    if (vertexIndex.Length == 3)
        zAverage = (float)0.33333 * (vertexPoint[0].z+vertexPoint[2].z+vertexPoint[2].z);
    else // polygon is a quad
        zAverage = (float)0.25 * (vertexPoint[0].z+vertexPoint[2].z+vertexPoint[2].z+vertexPoint[3].z);
}

void Polygon_f::print (Terminal& io)
{
    io.prints ("Polygon_f: \r\n")
    clour.prints (io);

    for (int i = 0; i< numVerticies; i++)
    {
        static const index_t[] header = { 4, STR, STR, STR, STR };
        vertexPoint[i].ToString () + " localVertex[" + vertexIndex[i]+ "] " + "\n");
    }

    return ToString;
}

}   //< _2D
