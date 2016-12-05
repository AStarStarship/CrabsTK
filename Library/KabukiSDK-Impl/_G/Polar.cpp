/** Underscore
    @file       /.../Source/KabukiSDK-Impl/_3D/PolarPoint.cs
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    http://www.apache.org/licenses/LICENSE-2.0
    @brief      This file contains the _2D::Vector_f interface.
*/

using System;

namespace _3D
{
    /**  */
    public class PolarPoint_d
    {

        /**  Default constructor.
        */
        public PolarPoint_d ()
        {
            theta1 = 0;
            theta2 = 0;
            r1 = 0;
            r2 = 0;
        }

        /** Constructor initializes with given values
        @param  thisAngle   The angle of the coordinate.
        @param  thisNumber  The magnitude of the coordinate.
        */
        public PolarPoint_d (double angle1, double angle2, double redius1, double radius2)
        {
            theta1 = angle1;
            theta2 = angle2;
            r1 = redius1;
            r2 = redius2;
        }

        public double Angle1
        {
            get { return theta1; }
            set { theta1 = value; }
        }

        public double Angle2
        {
            get { return theta2; }
            set { theta2 = value; }
        }

        public double Radius1
        {
            get { return r1; }
            set { r1 = value; }
        }

        public double Radius2
        {
            get { return r2; }
            set { r2 = value; }
        }

        public double Magnitude ()
        {
            double distance1 = System.Math.Sqrt (theta1 * theta1 + r1 * r1),
                distance2 = System.Math.Sqrt (theta2 * theta2 + r2 * r2);

            return System.Math.Sqrt (distance1 * distance1 + distance1 * distance2);
        }

    private: double theta1,
            theta2,
            r1,
            r2;
    }
}
