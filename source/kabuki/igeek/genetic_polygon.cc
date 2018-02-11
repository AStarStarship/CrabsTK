/** Kabuki Theater - Cell Interactive Genetic Algorihm
    @version   0.x
    @file      /.../Source/Kabuki_Theater/Cell_IGA-Impl/GeneticPolygon.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 Cale McCollough <calemccollough.github.io>

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

#include <Kabuki_Theater/Cell_IGA/GeneticPolygon.h>

namespace Kabuki_Theater { namespace Cell_IGA {

Color GeneticPolygon::getDefaultColor (int numPoints)
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
    return new Color ((int)Math.random ());
}

GeneticPolygon::GeneticPolygon ()
{
    int numPoints = (int)(3.0 + Math.random () * maxRandomPoints);
    
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

GeneticPolygon::GeneticPolygon (int thisNumPoints, int thisColorARGB)
{
    mother = father = null;
    lifespan = defaultLifespan;
    
    width           = randomizeInt    (defaultSize);
    height          = randomizeInt    (defaultSize);
    age             = randomizeDouble (defaultLifespan);
    angle           = randomizeDouble (defaultAngle);
    
    color = new Color (thisColorARGB);
    
    setupPoints (thisNumPoints);
    
    renderBitmap ();
}

GeneticPolygon::GeneticPolygon (int thisNumPoints, int thisWidth, int thisHeight, int thisColorARGB, 
    int thisLifespan, double thisAngle)
{
    mother = father = null;
    lifespan = thisLifespan;
    
    age             = (int) thisLifespan;
    
    width           = randomizeInt    (thisWidth);
    height          = randomizeInt    (thisHeight);
    angle           = randomizeDouble (thisAngle);
    
    color = new Color (thisColorARGB);
    
    setupPoints (thisNumPoints);
    renderBitmap ();
}

GeneticPolygon::GeneticPolygon (GeneticPolygon thisMother, GeneticPolygon thisFather)
{
    mother = thisMother;
    father = thisFather;
    
    if (mother == null || father == null)
    {
        if (mother == null)
            System.out.print ("Error breeding GeneticPolygon: Mother was null!\n");
        if (father == null)
            System.out.print ("Error breeding GeneticPolygon: Father was null!\n");
        
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
                                           (thisMother.getWidth () + thisFather.getWidth ())/2,
                                           (thisMother.getHeight () + thisFather.getHeight ())/2,
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
    
    int lowestX   = xpoints[0], // The largest positive integer.
        highestY  = ypoints[0],
        highestX  = lowestX,    // The largest negative integer.
        lowestY   = highestY;
    
    for (int i=0; i < npoints; i++)
    {   
        int currentX = xpoints[i],
            currentY = ypoints[i];
        
        if (currentX < lowestX)
            lowestX = currentX;
        else if (currentX > highestX)
            highestX = currentX;
            
        if (currentY > highestY)
            highestY = currentY;
        else if (currentY < lowestY)
            lowestY = currentY;
            
        //System.out.print ("lowestX = " + lowestX + ", highestX = " + highestX + ", highestY = " + highestY + ", lowestY = " + lowestY + "\n");
    }
    
    // Now translate the pots such that the farthest point left and top point are both 0
    
    for (int i=0; i < npoints; i++)
    {
        xpoints[i] = xpoints[i] - lowestX;
        ypoints[i] = ypoints[i] - lowestY;
    }
    
    //System.out.print ("initWidth = " + width + ", initHeight = " + height + "\n");
    
    width  = highestX - lowestX + 1;
    height = highestY - lowestY + 1;
    
    //printGeometry ();
}

void GeneticPolygon::printGeometry ()
{
    int i;
    
    System.out.print ("\nPrinting Geometry...\nwidth = " + width + ", height = " + height + "\nnpoints = " + npoints + "\n");
    
    for (i=0; i < npoints; i++)
    {
        System.out.print ("xpoints[" + i + "] = " + xpoints[i] + ", ypoints[" + i + "] = " + xpoints[i] + "\n");
    }
    
    System.out.print ("\nChecking bounds...\n");
    
    for (i=0; i < npoints; i++)
    {
        if (xpoints[i] > width)
            System.out.print ("Error: xpoint[" + i + "] = " + xpoints[i] + " which is greater than width!!!\n");
        else if (xpoints[i] < 0)
            System.out.print ("Error: xpoint[" + i + "] = " + xpoints[i] + " which is less than 0!!!\n");
            
        if (ypoints[i] > height)
            System.out.print ("Error: ypoint[" + i + "] = " + ypoints[i] + " which is greater than height!!!\n");
        else if (ypoints[i] < 0)
            System.out.print ("Error: xpoint[" + i + "] = " + ypoints[i] + " whichis less than 0!!!\n");
    }
    System.out.print ("\n\n");
}


void GeneticPolygon::setupPoints (int numberOfPoints)
{
    if (numberOfPoints < 3)
    {
        System.out.print ("Error: numberOfPoints < 3!\n");
        numberOfPoints = 3;
    }
    
    double deltaAngle = (2.0*Math.PI) / (double)numberOfPoints,
           rotationAngle,
           cosAngle,
           sinAngle,
           stretchFactor,
           doubleWidth  = (double) width,
           doubleHeight = (double) height,
           halfWidth = doubleWidth/2.0,
           halfHeight = doubleHeight/2.0,
           x,
           y;
    
    if (width > height)
    {
        stretchFactor = doubleWidth/doubleHeight; // We want stretchFactor to be greater than 1
        
        for (int i=0; i < numberOfPoints; i++)
        {   
            rotationAngle = i * deltaAngle;
            
            cosAngle = Math.cos (rotationAngle);
            sinAngle = Math.sin (rotationAngle);
            
            x = halfWidth + (halfHeight*cosAngle)*stretchFactor;
            y = halfHeight + (halfHeight*sinAngle);
            
            addPoint ((int)x, (int) y);
        }
    }
    else
    {
        stretchFactor = doubleHeight/doubleWidth;
        
        for (int i=0; i < numberOfPoints; i++)
        {   
            rotationAngle = i * deltaAngle;
            
            cosAngle = Math.cos (rotationAngle);
            sinAngle = Math.sin (rotationAngle);

            x = halfWidth + (halfWidth*cosAngle);
            y = halfHeight + (halfWidth*sinAngle)*stretchFactor;
            
            addPoint ((int)x, (int) y);
        }
    }
    
    rebound ();
}

bool GeneticPolygon::age (int ageTime)
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

int GeneticPolygon::mutateInt (int thisValue, int mutateDirection)
{
    double mutation,                //< The 
           vector,                  // The vector tells you if its getting bigger or smaller
           scalar=0;                // Tells you how much of a mutation there will be.
    
    // Output = thisValue * (1 + scallar * trend)
    
    if (mutateDirection > 0)
    {
        vector = 1.0;
    }
    else if (mutateDirection == 0)
    {
        if (Math.random () < 0.5)
            vector = 1.0;
        else
            vector = -1.0;
    }
    else // mutateDirection
    {
        vector = -1.0;
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
        
    int newValue = (int)((double) thisValue * (1.0 + vector*scalar));
        
    return newValue;
}

double GeneticPolygon::mutateFloat (double thisValue, int mutateDirection)
{
    double mutation,
           vector,      // The vector tells you if its getting bigger or smaller
           scalar=0;    // Tells you how much of a mutation there will be.
    
    // Output = thisValue * (1 + scallar*trend)
    
    if (mutateDirection > 0)
    {
        vector = 1.0f;
    }
    else if (mutateDirection == 0)
    {
        if (Math.random () < 0.5)
            vector = 1.0;
        else
            vector = -1.0;
    }
    else // mutateDirection
    {
        vector = -1.0;
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
    return (double)((double)thisValue * (1.0 + vector*scalar));
}

int GeneticPolygon::breedInt (int motherInt, int fatherInt, int matGrandInt, int patGrandInt)
{
    // The way that this algorithm works is that we are trying to mimic nature. 
    // First we need to determine if we are going to grow bigger or smaller
    // the the parents. The idea is that we need to see what the reproductive trend is. If the grandparents was
    // greater than its offspring, then that shows that there is trend of having larger offspring. If both 
    // grandparents were larger, then the offspring will be larger.
    
    int largerValue, smallerValue;
    
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

bool GeneticPolygon::containsGeneticPolygon (GeneticPolygon that)
{
    // The theory behind this algorithm is that in order for two polygons to intersect, at least one
    // point of either object has to be inside of the other.
    
    // Currently I am unaware of how Java does its contains (int, int) function so I have absolutly no
    // idea how efficient this is. The other method would be to compair their bitmaps. This might be 
    // able to be done in hardware via OpenCL.
    
    System.out.print ("bool contains (GeneticPolygon that)\n");
    int i;
    
    for (i=0; i < this.npoints; i++)
        if (this.contains (xpoints[i], ypoints[i]))
            return false;
    
    for (i=0; i < that.npoints; i++)
        if (that.contains (xpoints[i], ypoints[i]))
            return false;
            
    return true;
}

double GeneticPolygon::breedFloat (double motherInt, double fatherInt, double matGrandInt, double patGrandInt)
{
    return 0.0f;
}

/*  @brief  Function that returns the BufferedImage for this this.

 */

BufferedImage GeneticPolygon::getBitmap ()
{
    return bitmap;
}

int GeneticPolygon::getWidth ()
{
    return width;
}

int GeneticPolygon::getHeight ()
{
    return height;
}   

double GeneticPolygon::getAngle ()
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

int GeneticPolygon::getLifespan ()
{
    return lifespan;
}

int GeneticPolygon::getNumPoints ()
{
    return this.npoints;
}

// Function that prints out the this to a bitstream

void GeneticPolygon::printGenes (int[] thisStream, int currentIndex)
{
    thisStream[currentIndex  ] = width;
    thisStream[currentIndex+1] = height;
    thisStream[currentIndex+2] = (int)angle;
    thisStream[currentIndex+5] = color.getRGB ();
    thisStream[currentIndex+6] = (int)this.npoints;
    
    currentIndex+=7;
    
    for (int i=0; i < super.npoints; i++)
    {
        thisStream[currentIndex  ] = xpoints[i];
        thisStream[currentIndex+1] = ypoints[i];
        currentIndex+=2;
    }
}

/* @brief   Function that compares 
 */

bool GeneticPolygon::intersects (Polygon thatPolygon)
{
    Rectangle bounds = thatPolygon.getBounds ();
    
    int offsetX = (width-bounds.width)/2,
        offsetY = (height-bounds.height)/2;
        
    //System.out.print ("offsetX = " + offsetX + ", offsetY = " + offsetY + "\n");
    return intersects (thatPolygon, offsetX, offsetY);
}

bool GeneticPolygon::intersects (GeneticPolygon thatPolygon)
{
    int offsetX = (width-thatPolygon.width)/2,
        offsetY = (height-thatPolygon.height)/2;
        
    //System.out.print ("offsetX = " + offsetX + ", offsetY = " + offsetY + "\n");
    return intersects (thatPolygon, offsetX, offsetY);
}

bool GeneticPolygon::intersects (Polygon thatPolygon, double offsetX, double offsetY)
{
    return intersects (thatPolygon, (int)offsetX, (int)offsetY);
}

bool GeneticPolygon::intersects (Polygon thatPolygon, int offsetX, int offsetY)
{
    // In order to determine if one Polygons ccontain eachother, we have to check every point in both
    // Polygon(s).
 
    if (thatPolygon == null)
        return false;
    
    for (int i=0; i < thatPolygon.npoints; i++)
    {
        if (super.contains (thatPolygon.xpoints[i]+offsetX, thatPolygon.ypoints[i]+offsetX))
        {
            return true;
        }
    }
    for (int i=0; i < npoints; i++)
    {
        if (thatPolygon.contains (xpoints[i]-offsetX, ypoints[i]-offsetY))
            return true;
    }
    return false;
}

void GeneticPolygon::equalizePoints ()
{
    double centerX = (double) width /2.0,
           centerY = (double) height/2.0,
           deltaY = ypoints[1] - ypoints[0],
           deltaX = xpoints[1] - xpoints[0],
           deltaAngle = (2*Math.PI) / (double)this.npoints,
           initAngle = Math.atan (deltaY/deltaX),
           currentAngle,
           idealAngle,
           rotationAngle,
           x,
           y,
           cosAngle,
           sinAngle;
    
    for (int i=0; i < this.npoints-1; i++)
    {   
        // Step 1: find angle between the current and next point.
        
        x = (double) xpoints[i];
        y = (double) ypoints[i];
        
        deltaX = xpoints[i+1] - x;
        deltaY = ypoints[i+1] - y;
        currentAngle = Math.atan ((deltaY/deltaX));
        
        // Step 2: figure out what the angle should be.
        
        idealAngle = i*deltaAngle + initAngle;
        
        // Step 3: rotate the point closer to the 
        
        if (currentAngle < idealAngle)
            rotationAngle = (double)angularMutation;
        else if (currentAngle > idealAngle)
            rotationAngle = (double)angularMutation*-1.0;
        else
            rotationAngle = 0.0;
        
        cosAngle = Math.cos (rotationAngle);
        sinAngle = Math.sin (rotationAngle);
        
        xpoints[i] = (int)((x+centerX)*cosAngle - (y+centerY)*sinAngle);
        ypoints[i] = (int)((y+centerY)*sinAngle + (y+centerY)*cosAngle);
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
    int thisNumPixels = 0;
    
    for (int x=0; x < width; x++)
        for (int y=0; y < height; y++)
            if (contains (x, y))
                thisNumPixels++;
    
    numPixels = thisNumPixels;
    
    //JOptionPane.showMessageDialog(null, new JLabel (new ImageIcon (bitmap)), "About", JOptionPane.PLAIN_MESSAGE, null);
}

double GeneticPolygon::crossOver (double dominant, double recessive)
{
    /* Cross over scheme.
        In real anamals, there are dominant and recessive genes. The dominant gene is represented
        by the most significant digit and the recessive is represented by the lest significant.
        
        Currently, the bitmasks were selected by intuition and should be reevaluated.
     */
     
     if (dominant > 1.0f)
        JOptionPane.showMessageDialog(null, "Error! Dominant double was greater than 1!!!");
     else if (dominant < 0.0f)
        JOptionPane.showMessageDialog(null, "Error! Dominant double was less than 0!!!");
     
     if (recessive > 1.0f)
        JOptionPane.showMessageDialog(null, "Error! Recessive double was greater than 1!!!");
     else if (recessive < 0.0f)
        JOptionPane.showMessageDialog(null, "Error! Recessive double was less than 0!!!");
     
     
    String dominantString = Double.toString(dominant),
           recessiveString = Double.toString(recessive);
    
    int dominantStringLength = dominantString.length (),
        recessiveStringLength = recessiveString.length (),
        longestStringLength = (dominantStringLength > recessiveStringLength) ? recessiveStringLength : recessiveStringLength,
        i;
        
    char[] newString = new char[longestStringLength];
        
    bool dominantTurn = true;
    
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
    
    double cosAngle = Math.cos ((double)angle),
           sinAngle = Math.sin ((double)angle),
           x,
           y;
    
    for (int i=0; i < this.npoints; i++)
    {
        x = (double) xpoints[i];
        y = (double) ypoints[i];
        
        xpoints[i] = (int) (x*cosAngle - y*sinAngle);
        ypoints[i] = (int) (x*sinAngle + y*cosAngle);
    }
    rebound ();
}

double GeneticPolygon::boundBetween0and1 (double inputValue)
{
    if (inputValue < 0)
        return 0;
    
    else if (inputValue > 1)
        return 1;
    
    return inputValue;
}

double GeneticPolygon::randomizeDouble (double inputValue)
{
    double scalar = 0.0,
           randomNumber = Math.random ();
    
    if (randomNumber < majorMutation)
    {
        scalar = 1.0 + Math.random () * majorScalar;
    }
    else
        scalar = 1.0 + Math.random () * minorScalar;
    
    double newValue = inputValue * scalar;
    
    return newValue;
}

int GeneticPolygon::randomizeInt (int inputValue)
{
    if (inputValue < 0)
        inputValue = inputValue * -1; //< Chop off the negative values.
    
    double scalar = 1.0 + (Math.random ()*2.0 - 1.0) * minorScalar;
    
    int newValue = (int)((double)inputValue*scalar);
    
    return newValue;
}

}   //< namespace Kabuki_Theater { namespace Cell_IGA {
