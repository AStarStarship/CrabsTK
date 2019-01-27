/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /projects/kabuki/igeek/library/igeek_geneticpolygon.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>

#include "cgeneticpolygon.h"

namespace kabuki {
namespace igeek {

Color GeneticPolygon::getDefaultColor (SI4 numPoints)
{
    switch (numPoints)
    {
        case 3:
            return Color.blue;
        case 4:
            return Color.red;
        case 5:
            return Color.green;
        case 6:
            return Color.orange;
        case 7:
            return Color.gray;
        case 8:
            return Color.pink;
    }
    return new Color ((SI4)Math.random ());
}

GeneticPolygon::GeneticPolygon () {
    SI4 numPoints = (SI4)(3.0 + Math.random () * maxRandomPoints);
    
    mother = father = null;
    lifespan = defaultLifespan;
    
    width  = randomizeInt   (defaultSize);
    height = randomizeInt   (defaultSize);
    age    = randomizeDouble (defaultLifespan);
    angle  = randomizeDouble (defaultAngle);
    
    color = getDefaultColor (numPoints);
    
    setupPoints (numPoints);
    
    renderBitmap ();
}

GeneticPolygon::GeneticPolygon (SI4 point_count, SI4 color) {
    mother = father = null;
    lifespan = defaultLifespan;
    
    width           = randomizeInt    (defaultSize);
    height          = randomizeInt    (defaultSize);
    age             = randomizeDouble (defaultLifespan);
    angle           = randomizeDouble (defaultAngle);
    
    color = new Color (color);
    
    setupPoints (point_count);
    
    renderBitmap ();
}

GeneticPolygon::GeneticPolygon (SI4 point_count, SI4 width, SI4 height, SI4 color, 
    SI4 lifespan, DBL angle) {
    mother = father = null;
    lifespan = lifespan;
    
    age             = (SI4) lifespan;
    
    width           = randomizeInt    (width);
    height          = randomizeInt    (height);
    angle           = randomizeDouble (angle);
    
    color = new Color (color);
    
    setupPoints (point_count);
    renderBitmap ();
}

GeneticPolygon::GeneticPolygon (GeneticPolygon thisMother, 
                                GeneticPolygon thisFather) {
    mother = thisMother;
    father = thisFather;
    
    if (mother == null || father == null)
    {
        if (mother == null)
            Print ("Error breeding GeneticPolygon: Mother was null!\n");
        if (father == null)
            Print ("Error breeding GeneticPolygon: Father was null!\n");
        
        grandparents = null;
        
        width  = randomizeInt    (GeneticPolygon.defaultSize);
        height = randomizeInt    (GeneticPolygon.defaultSize);
        angle  = randomizeDouble (GeneticPolygon.defaultAngle);
        
        color = getDefaultColor (defaultNumPoints);
        
        setupPoints (GeneticPolygon.defaultNumPoints);
    }
    else
    {
        grandparents = new GeneticPolygon ((thisMother.getNumPoints () + thisFather.getNumPoints ())/2, 
                                           (thisMother.Width () + thisFather.Width ())/2,
                                           (thisMother.Height () + thisFather.Height ())/2,
                                           (thisMother.getColor ().getRGB () + thisFather.getColor ().getRGB ())/2,
                                           (thisMother.getLifespan() + thisFather.getLifespan ())/2,
                                           (thisMother.getAngle () + thisFather.getAngle ())/2.0f);
        
        GeneticPolygon matGrandparents = mother.grandparents,
                       patGrandparents = father.grandparents;
                       
        if (matGrandparents == null)
            matGrandparents = grandparents;
        if (patGrandparents == null)
            patGrandparents = grandparents;
        
        width  = breedInt   (mother.width, father.width, matGrandparents.width, patGrandparents.width);
        
        if (width < minSize)
            width = minSize;
        else if (height > maxSize)
            width = maxSize;
            
        height = breedInt   (mother.height, father.height, matGrandparents.height, patGrandparents.height);
        
        if (height < minSize)
            height = minSize;
        else if (height > maxSize)
            height = maxSize;
            
        angle  = breedFloat (mother.angle, father.angle, matGrandparents.angle, patGrandparents.angle);            
        color  = breedColor (mother.color, father.color, matGrandparents.color, patGrandparents.color);            
        
        setupPoints (breedInt (mother.getNumPoints (), father.getNumPoints (), matGrandparents.getNumPoints (), patGrandparents.getNumPoints ()));
    }
    
    renderBitmap ();
}


void GeneticPolygon::rebound ()
{
    // if (npoints == 0); // This shouldn't happen.
    
    // Once a shape has been made from the idea width and height, the actual polygon width will not be 
    // the full width or height. For this reason, we need to find the left, right, top, and bottom bounds,
    // and translate all of the points so that the farthest point to the left and top are both 0.
    
    SI4 lowestX   = xpoints[0], // The largest positive integer.
        highestY  = ypoints[0],
        highestX  = lowestX,    // The largest negative integer.
        lowestY   = highestY;
    
    for (SI4 i=0; i < npoints; i++)
    {   
        SI4 currentX = xpoints[i],
            currentY = ypoints[i];
        
        if (currentX < lowestX)
            lowestX = currentX;
        else if (currentX > highestX)
            highestX = currentX;
            
        if (currentY > highestY)
            highestY = currentY;
        else if (currentY < lowestY)
            lowestY = currentY;
            
        //Print ("lowestX = " + lowestX + ", highestX = " + highestX + ", highestY = " + highestY + ", lowestY = " + lowestY + "\n");
    }
    
    // Now translate the pots such that the farthest point left and top point are both 0
    
    for (SI4 i=0; i < npoints; i++)
    {
        xpoints[i] = xpoints[i] - lowestX;
        ypoints[i] = ypoints[i] - lowestY;
    }
    
    //Print ("initWidth = " + width + ", initHeight = " + height + "\n");
    
    width  = highestX - lowestX + 1;
    height = highestY - lowestY + 1;
    
    //printGeometry ();
}

void GeneticPolygon::PrintGeometry ()
{
    SI4 i;
    
    Print ("\nPrinting Geometry...\nwidth = " + width + ", height = " + height + "\nnpoints = " + npoints + "\n");
    
    for (i=0; i < npoints; i++)
    {
        Print ("xpoints[" + i + "] = " + xpoints[i] + ", ypoints[" + i + "] = " + xpoints[i] + "\n");
    }
    
    Print ("\nChecking bounds...\n");
    
    for (i=0; i < npoints; i++)
    {
        if (xpoints[i] > width)
            Print ("Error: xpoint[" + i + "] = " + xpoints[i] + " which is greater than width!!!\n");
        else if (xpoints[i] < 0)
            Print ("Error: xpoint[" + i + "] = " + xpoints[i] + " which is less than 0!!!\n");
            
        if (ypoints[i] > height)
            Print ("Error: ypoint[" + i + "] = " + ypoints[i] + " which is greater than height!!!\n");
        else if (ypoints[i] < 0)
            Print ("Error: xpoint[" + i + "] = " + ypoints[i] + " whichis less than 0!!!\n");
    }
    Print ("\n\n");
}


void GeneticPolygon::setupPoints (SI4 numberOfPoints)
{
    if (numberOfPoints < 3)
    {
        Print ("Error: numberOfPoints < 3!\n");
        numberOfPoints = 3;
    }
    
    DBL deltaAngle = (2.0*Math.PI) / (DBL)numberOfPoints,
           rotationAngle,
           cosAngle,
           sinAngle,
           stretchFactor,
           doubleWidth  = (DBL) width,
           doubleHeight = (DBL) height,
           halfWidth = doubleWidth/2.0,
           halfHeight = doubleHeight/2.0,
           x,
           y;
    
    if (width > height)
    {
        stretchFactor = doubleWidth/doubleHeight; // We want stretchFactor to be greater than 1
        
        for (SI4 i=0; i < numberOfPoints; i++)
        {   
            rotationAngle = i * deltaAngle;
            
            cosAngle = Math.cos (rotationAngle);
            sinAngle = Math.sin (rotationAngle);
            
            x = halfWidth + (halfHeight*cosAngle)*stretchFactor;
            y = halfHeight + (halfHeight*sinAngle);
            
            addPoint ((SI4)x, (SI4) y);
        }
    }
    else
    {
        stretchFactor = doubleHeight/doubleWidth;
        
        for (SI4 i=0; i < numberOfPoints; i++)
        {   
            rotationAngle = i * deltaAngle;
            
            cosAngle = Math.cos (rotationAngle);
            sinAngle = Math.sin (rotationAngle);

            x = halfWidth + (halfWidth*cosAngle);
            y = halfHeight + (halfWidth*sinAngle)*stretchFactor;
            
            addPoint ((SI4)x, (SI4) y);
        }
    }
    
    rebound ();
}

BOL GeneticPolygon::age (SI4 ageTime)
{
    age += ageTime;
    
    if (age >= lifespan)
        return false;
    
    return true;
}

Color GeneticPolygon::mixColors (Color motherColor, Color fatherColor)
{
    float[] motherHSB = new float[3],
            fatherHSB = new float[3];
    
    Color.RGBtoHSB (motherColor.getRed (), motherColor.getGreen (), motherColor.getBlue (), motherHSB);
    Color.RGBtoHSB (fatherColor.getRed (), fatherColor.getGreen (), fatherColor.getBlue (), fatherHSB);
    return new Color (Color.HSBtoRGB((motherHSB[0]+fatherHSB[0])/2, (motherHSB[1]+fatherHSB[1])/2, (motherHSB[2]+fatherHSB[2])/2));
}

Color GeneticPolygon::breedColor (Color motherColor, Color fatherColor, Color  matGrandColor, Color patGrandColor)
{
    return mixColors (motherColor, fatherColor);
}

SI4 GeneticPolygon::mutateInt (SI4 thisValue, SI4 mutateDirection)
{
    DBL mutation,                //< The 
           TArray,                  // The TArray tells you if its getting bigger or smaller
           scalar=0;                // Tells you how much of a mutation there will be.
    
    // Output = thisValue * (1 + scallar * trend)
    
    if (mutateDirection > 0)
    {
        TArray = 1.0;
    }
    else if (mutateDirection == 0)
    {
        if (Math.random () < 0.5)
            TArray = 1.0;
        else
            TArray = -1.0;
    }
    else // mutateDirection
    {
        TArray = -1.0;
    }
    
    mutation = Math.random ();
    
    if (mutation < minorMutation)
    {
        if (mutation < minorMutation)
        {
            if (mutation < majorMutation)
                scalar = majorScalar;
            else
                scalar = minorScalar;
        }
    }
        
    SI4 newValue = (SI4)((DBL) thisValue * (1.0 + TArray*scalar));
        
    return newValue;
}

DBL GeneticPolygon::mutateFloat (DBL thisValue, SI4 mutateDirection)
{
    DBL mutation,
           TArray,      // The TArray tells you if its getting bigger or smaller
           scalar=0;    // Tells you how much of a mutation there will be.
    
    // Output = thisValue * (1 + scallar*trend)
    
    if (mutateDirection > 0)
    {
        TArray = 1.0f;
    }
    else if (mutateDirection == 0)
    {
        if (Math.random () < 0.5)
            TArray = 1.0;
        else
            TArray = -1.0;
    }
    else // mutateDirection
    {
        TArray = -1.0;
    }
    
    mutation = Math.random ();
    
    if (mutation < minorMutation)
    {
        if (mutation < minorMutation)
        {
            if (mutation < majorMutation)
                scalar = majorScalar;
            else
                scalar = minorScalar;
        }
    }
    return (DBL)((DBL)thisValue * (1.0 + TArray*scalar));
}

SI4 GeneticPolygon::breedInt (SI4 motherInt, SI4 fatherInt, SI4 matGrandInt, SI4 patGrandInt)
{
    // The way that this algorithm works is that we are trying to mimic nature. 
    // First we need to determine if we are going to grow bigger or smaller
    // the the parents. The idea is that we need to see what the reproductive trend is. If the grandparents was
    // greater than its offspring, then that shows that there is trend of having larger offspring. If both 
    // grandparents were larger, then the offspring will be larger.
    
    SI4 largerValue, smallerValue;
    
    // Step one: Determine which is the larger and smaller value.
    
    if (motherInt > fatherInt)
    {
        largerValue = motherInt;
        smallerValue = fatherInt;
    }
    else
    {
        largerValue = fatherInt;
        smallerValue = motherInt;
    }
    
    if (motherInt > matGrandInt)
    {
        if (fatherInt > patGrandInt) // Then both grandparents were bigger so grow bigger.
        {
            return mutateInt (largerValue, 1);
        }
        else
        {
        }
    }
    else // Then mother getting smaller
    {
        if (fatherInt < patGrandInt) // Then both grandparents were smaller so getting smaller
        {
            return mutateInt (smallerValue, -1);
        }
    }
    
    return mutateInt (smallerValue, 0);
}

BOL GeneticPolygon::containsGeneticPolygon (GeneticPolygon that)
{
    // The theory behind this algorithm is that in order for two polygons to intersect, at least one
    // point of either object has to be inside of the other.
    
    // Currently I am unaware of how Java does its contains (SI4, SI4) function so I have absolutly no
    // idea how efficient this is. The other method would be to compair their bitmaps. This might be 
    // able to be done in hardware via OpenCL.
    
    Print ("BOL contains (GeneticPolygon that)\n");
    SI4 i;
    
    for (i=0; i < this.npoints; i++)
        if (this.contains (xpoints[i], ypoints[i]))
            return false;
    
    for (i=0; i < that.npoints; i++)
        if (that.contains (xpoints[i], ypoints[i]))
            return false;
            
    return true;
}

DBL GeneticPolygon::breedFloat (DBL motherInt, DBL fatherInt, DBL matGrandInt, DBL patGrandInt)
{
    return 0.0f;
}

/*  @brief  Function that returns the BufferedImage for this this.

 */

BufferedImage GeneticPolygon::getBitmap ()
{
    return bitmap;
}

SI4 GeneticPolygon::Width ()
{
    return width;
}

SI4 GeneticPolygon::Height ()
{
    return height;
}   

DBL GeneticPolygon::getAngle ()
{
    return angle;
}

Color GeneticPolygon::getColor ()
{
    return color;
}

long GeneticPolygon::getNumPixels ()
{
    return numPixels;
}

SI4 GeneticPolygon::getLifespan ()
{
    return lifespan;
}

SI4 GeneticPolygon::getNumPoints ()
{
    return this.npoints;
}

// Function that prints out the this to a bitstream

void GeneticPolygon::printGenes (SI4[] thisStream, SI4 currentIndex)
{
    thisStream[currentIndex  ] = width;
    thisStream[currentIndex+1] = height;
    thisStream[currentIndex+2] = (SI4)angle;
    thisStream[currentIndex+5] = color.getRGB ();
    thisStream[currentIndex+6] = (SI4)this.npoints;
    
    currentIndex+=7;
    
    for (SI4 i=0; i < super.npoints; i++)
    {
        thisStream[currentIndex  ] = xpoints[i];
        thisStream[currentIndex+1] = ypoints[i];
        currentIndex+=2;
    }
}

/* @brief   Function that compares 
 */

BOL GeneticPolygon::intersects (Polygon thatPolygon)
{
    Rectangle bounds = thatPolygon.getBounds ();
    
    SI4 offsetX = (width-bounds.width)/2,
        offsetY = (height-bounds.height)/2;
        
    //Print ("offsetX = " + offsetX + ", offsetY = " + offsetY + "\n");
    return intersects (thatPolygon, offsetX, offsetY);
}

BOL GeneticPolygon::intersects (GeneticPolygon thatPolygon)
{
    SI4 offsetX = (width-thatPolygon.width)/2,
        offsetY = (height-thatPolygon.height)/2;
        
    //Print ("offsetX = " + offsetX + ", offsetY = " + offsetY + "\n");
    return intersects (thatPolygon, offsetX, offsetY);
}

BOL GeneticPolygon::intersects (Polygon thatPolygon, DBL offsetX, DBL offsetY)
{
    return intersects (thatPolygon, (SI4)offsetX, (SI4)offsetY);
}

BOL GeneticPolygon::intersects (Polygon thatPolygon, SI4 offsetX, SI4 offsetY)
{
    // In order to determine if one Polygons ccontain eachother, we have to check every point in both
    // Polygon(s).
 
    if (thatPolygon == null)
        return false;
    
    for (SI4 i=0; i < thatPolygon.npoints; i++)
    {
        if (super.contains (thatPolygon.xpoints[i]+offsetX, thatPolygon.ypoints[i]+offsetX))
        {
            return true;
        }
    }
    for (SI4 i=0; i < npoints; i++)
    {
        if (thatPolygon.contains (xpoints[i]-offsetX, ypoints[i]-offsetY))
            return true;
    }
    return false;
}

void GeneticPolygon::equalizePoints ()
{
    DBL centerX = (DBL) width /2.0,
           centerY = (DBL) height/2.0,
           deltaY = ypoints[1] - ypoints[0],
           deltaX = xpoints[1] - xpoints[0],
           deltaAngle = (2*Math.PI) / (DBL)this.npoints,
           initAngle = Math.atan (deltaY/deltaX),
           currentAngle,
           idealAngle,
           rotationAngle,
           x,
           y,
           cosAngle,
           sinAngle;
    
    for (SI4 i=0; i < this.npoints-1; i++)
    {   
        // Step 1: find angle between the current and next point.
        
        x = (DBL) xpoints[i];
        y = (DBL) ypoints[i];
        
        deltaX = xpoints[i+1] - x;
        deltaY = ypoints[i+1] - y;
        currentAngle = Math.atan ((deltaY/deltaX));
        
        // Step 2: figure out what the angle should be.
        
        idealAngle = i*deltaAngle + initAngle;
        
        // Step 3: rotate the point closer to the 
        
        if (currentAngle < idealAngle)
            rotationAngle = (DBL)angularMutation;
        else if (currentAngle > idealAngle)
            rotationAngle = (DBL)angularMutation*-1.0;
        else
            rotationAngle = 0.0;
        
        cosAngle = Math.cos (rotationAngle);
        sinAngle = Math.sin (rotationAngle);
        
        xpoints[i] = (SI4)((x+centerX)*cosAngle - (y+centerY)*sinAngle);
        ypoints[i] = (SI4)((y+centerY)*sinAngle + (y+centerY)*cosAngle);
    }
    
    rebound ();
}

void GeneticPolygon::renderBitmap ()
{
    bitmap = new BufferedImage (width, height, BufferedImage.TYPE_INT_ARGB);
    
    Graphics2D g2d = bitmap.createGraphics();
    //g2d.setComposite(AlphaComposite.Clear);
    //g2d.fillRect(0, 0, width, height);
    
    g2d.setColor(color);
    g2d.fillPolygon (this);
    
    g2d.setColor(Color.BLACK);
    g2d.drawPolygon (this);
    
    //g2d.fillRect (0,0,width,height); //< This is to test to see if it draws anything.
    
    //g2d.dispose();
    
    // Now count the number of pixels.
    SI4 thisNumPixels = 0;
    
    for (SI4 x=0; x < width; x++)
        for (SI4 y=0; y < height; y++)
            if (contains (x, y))
                thisNumPixels++;
    
    numPixels = thisNumPixels;
    
    //JOptionPane.showMessageDialog(null, new JLabel (new ImageIcon (bitmap)), "About", JOptionPane.PLAIN_MESSAGE, null);
}

DBL GeneticPolygon::crossOver (DBL dominant, DBL recessive)
{
    /* Cross over scheme.
        In real anamals, there are dominant and recessive genes. The dominant gene is represented
        by the most significant digit and the recessive is represented by the lest significant.
        
        Currently, the bitmasks were selected by intuition and should be reevaluated.
     */
     
     if (dominant > 1.0f)
        JOptionPane.showMessageDialog(null, "Error! Dominant DBL was greater than 1!!!");
     else if (dominant < 0.0f)
        JOptionPane.showMessageDialog(null, "Error! Dominant DBL was less than 0!!!");
     
     if (recessive > 1.0f)
        JOptionPane.showMessageDialog(null, "Error! Recessive DBL was greater than 1!!!");
     else if (recessive < 0.0f)
        JOptionPane.showMessageDialog(null, "Error! Recessive DBL was less than 0!!!");
     
     
    String dominantString = Double.toString(dominant),
           recessiveString = Double.toString(recessive);
    
    SI4 dominantStringLength = dominantString.length (),
        recessiveStringLength = recessiveString.length (),
        longestStringLength = (dominantStringLength > recessiveStringLength) ? recessiveStringLength : recessiveStringLength,
        i;
        
    CH1[] newString = new CH1[longestStringLength];
        
    BOL dominantTurn = true;
    
    for (i=2; i < longestStringLength; i++)
    {
        if (dominantTurn)
        {
            dominantTurn = false;
            
            if (i < dominantStringLength)
                newString[i] = dominantString.charAt(i);
            else
                newString[i] = '0';
        }
        else // resessiveTurn
        {
            dominantTurn = true;
            
            if (i < recessiveStringLength)
                newString[i] = recessiveString.charAt(i);
            else
                newString[i] = '0';
        }
    }
    
    return Float.parseFloat (new String (newString));
}

void GeneticPolygon::rotate ()
{
    // function uses standard 2D rotation matrix.
    
    DBL cosAngle = Math.cos ((DBL)angle),
           sinAngle = Math.sin ((DBL)angle),
           x,
           y;
    
    for (SI4 i=0; i < this.npoints; i++)
    {
        x = (DBL) xpoints[i];
        y = (DBL) ypoints[i];
        
        xpoints[i] = (SI4) (x*cosAngle - y*sinAngle);
        ypoints[i] = (SI4) (x*sinAngle + y*cosAngle);
    }
    rebound ();
}

DBL GeneticPolygon::boundBetween0and1 (DBL inputValue)
{
    if (inputValue < 0)
        return 0;
    
    else if (inputValue > 1)
        return 1;
    
    return inputValue;
}

DBL GeneticPolygon::randomizeDouble (DBL inputValue)
{
    DBL scalar = 0.0,
           randomNumber = Math.random ();
    
    if (randomNumber < majorMutation)
    {
        scalar = 1.0 + Math.random () * majorScalar;
    }
    else
        scalar = 1.0 + Math.random () * minorScalar;
    
    DBL newValue = inputValue * scalar;
    
    return newValue;
}

SI4 GeneticPolygon::randomizeInt (SI4 inputValue)
{
    if (inputValue < 0)
        inputValue = inputValue * -1; //< Chop off the negative values.
    
    DBL scalar = 1.0 + (Math.random ()*2.0 - 1.0) * minorScalar;
    
    SI4 newValue = (SI4)((DBL)inputValue*scalar);
    
    return newValue;
}

}   //< namespace igeek
}   //< namespace kabuki
