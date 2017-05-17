/** Underscore
    @file       /.../Underscore.Library/_G3D/Poly3D.cs
    @author     Cale McCollough
    @copyright  Copyright 2016 Blue Storm Engineering ©
    @license    http://www.apache.org/licenses/LICENSE-2.0
    @brief      This file contains the _Math::Vector_f interface.
*/

#include <_Math/Poly3D.hpp>

namespace _Math {

Poly3D_f::Poly3D (int* indexOfPoints, Point3D* pointValues, int numPoints)
:   shadingType (FlatShading),
    isTwoSided  (false),
    isClipped   (false),
    isActive    (false),
    isVisible   (true);
{
    if (numPoints < 4)
    {
        // Create unit box with coordinates all 0.0 and 1.0.
    }
    vertexIndex = indexOfPoints;
    vertexPoint = pointValues; 
    numVerticies = vertexIndex.Length;
    base.data = this;
}

Point3D Poly3D_f::getVertex (int forThisPoint)
{
    return vertexPoint[forThisPoint];
}

int VertexIndex (int forThisPoint)
{
    return vertexIndex[forThisPoint];
}

Vect3D getNormal ()
{
    return normal;
}

void Poly3D_f::computeNormal ()
{
    Point3D u (vertexPoint[0], vertexPoint[1]);
        v = new Point3D (vertexPoint[0], vertexPoint[2]);

    normal = v.crossProduct (u);

    normalMagnitude = normal.magnitude ();
}

float Poly3D_f::getNormalMagnitude ()
{
    return normalMagnitude;
}

void Poly3D_f::scaleNormalMagnitude (float scaleFactor)
{
    normalMagnitude* = scaleFactor;
}

float Poly3D_f::getZAverage ()
{
    return zAverage;
}

void Poly3D_f::computeZAverge ()
{
    if (vertexIndex.Length == 3)
        zAverage = (float)0.33333 * (vertexPoint[0].z+vertexPoint[2].z+vertexPoint[2].z);
    else // polygon is a quad
        zAverage = (float)0.25 * (vertexPoint[0].z+vertexPoint[2].z+vertexPoint[2].z+vertexPoint[3].z);
}

void Poly3D_f::print (Terminal& io)
{
    io.prints ("Poly3D_f: \r\n")
    clour.prints (io);

    for (int i = 0; i< numVerticies; i++)
    {
        static const index_t[] header = { 4, STR, STR, STR, STR };
        vertexPoint[i].ToString () + " localVertex[" + vertexIndex[i]+ "] " + "\n");
    }

    return ToString;
}

}   //< _Math
