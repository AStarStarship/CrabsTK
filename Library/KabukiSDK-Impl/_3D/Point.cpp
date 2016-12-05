/** Underscore
    @file       /.../Underscore.Library/_G3D/Point_f.cs
    @author     Cale McCollough
    @copyright  Copyright 2016 Blue Storm Engineering ©
    @license    http://www.apache.org/licenses/LICENSE-2.0
    @brief      This file contains the _2D::Vector_f interface.
*/

namespace _G3D
{
    /**  */
    struct Point_f
    {
        /**  */
        Point_f (float initX, float initY, float initZ)
        {
            x = initX;
            y = initY;
            z = initZ;
        }
    
        /**  */
        void TranslateX (float deltaX)
        {
            x + =  deltaX;
        }
    
        /**  */
        void TranslateY (float deltaY)
        {
            y + =  deltaY;
        }
    
        /**  */
        void TranslateZ (float deltaZ)
        {
            z + =  deltaZ;
        }
    
        /**  */
        void Print ()
        {
            return new textArray<char> (Integer.ToString ((int)x), ", ", Integer.ToString ((int)y), ", ", Integer.ToString ((int)z));
        }
    
        /**  */
        override string ToString ()
        {
            return new textArray<char> (Integer.ToString ((int)x), ", ", Integer.ToString ((int)y), ", ", Integer.ToString ((int)z));
        }
    }
}
