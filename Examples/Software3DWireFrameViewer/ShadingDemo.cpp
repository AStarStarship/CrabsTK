/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Examples/Software3DWireFrameViewer/ShadingDemo.cpp
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

#include "SoftwareEngine.hpp"

namespace KabukiSDKExamples {

class ShadingDemo: public hal.grfx3d.SoftwareEngine
{
    final int WIDTH = 640, HEIGHT = 480, halfWIDTH = 320, halfHEIGHT = 240;
    int clipMinX = 0, clipMinY = 0, clipMaxX = WIDTH, clipMaxY = HEIGHT, clipNearZ = 0, clipFarZ = 640;
    float[] cos, sin;
    float[][] identityMatrix, zeroMatrix;
    int viewingDistance, aspectRatio;
    int step = 0, lineStep = 0;
    bool trip = false;
    Shape3d testModel;
    d3.Vector_d viewd3.Point_d, lightSource, lineOfSight;
    int ambientLightLevel = 7;
    
    void init ()
    {  viewingDistance = 250;
        aspectRatio = WIDTH/HEIGHT;
        zeroMatrix = new float[4][4];
        for (int y = 0; y<4; y++)
            for (int x = 0; x<4; x++)
                zeroMatrix[x][y] = 0;
        identityMatrix = zeroMatrix;
        identityMatrix[0][0] = 1;
        identityMatrix[1][1] = 1;
        identityMatrix[2][2] = 1;
        identityMatrix[3][3] = 1;
        buildSinCosTable ();
        
        testModel = new Shape3d ("pyramid.plg", new d3.Point_d (0,0,200), this);
        testModel.rotate(0,0,0);     
                      
        viewd3.Point_d = new d3.Vector_d (0,0,0);
        lightSource = new d3.Vector_d ((float)-.913913,(float).389759,(float)-.113369);
        
        this.addKeyListener (new DirectionListener());
        addMouseListener (new ClickListener());
        setBackground (Color.white);
        setSize (WIDTH, HEIGHT);
    }
    
    void paint (Cell& C)
    {  page.fillRect (0,0,WIDTH,HEIGHT);
        for (int i = 0; i<testModel.numVertices; i++)
        {  testModel.cameraVertices[i]  = 
            testModel.worldVertices[i]  = new d3.Point_d (testModel.localVertices[i].x + testModel.position.x,
                                                                  testModel.localVertices[i].y + testModel.position.y,
                                                                  testModel.localVertices[i].z + testModel.position.z);
        } 
        removeBackFacesAndShade (testModel);
        renderShapeSolid (testModel, page);
    }
    
    void renderShapeSolid (Shape3d renderShape, Cell& C)
    {  bool polygonIsQuad = false;
        int vertexA, vertexB, vertexC, vertexD;
        float z1, z2, z3, z4;
        d3.Point_d one, two, three, four;
        d3.Point_d[] convertedd3.Point_d = new d3.Point_d[renderShape.cameraVertices.length];
        for (int i = 0; i<renderShape.cameraVertices.length; i++)
            convertedd3.Point_d[i] = new d3.Point_d (halfWIDTH + renderShape.cameraVertices[i].x*viewingDistance/renderShape.cameraVertices[i].z,
                                                     halfHEIGHT - aspectRatio*renderShape.cameraVertices[i].y*viewingDistance/renderShape.cameraVertices[i].z);
        for (int currPoly = 0; currPoly<renderShape.polygon.length; currPoly++)
        {  if (renderShape.polygon[currPoly].isVisible||!renderShape.polygon[currPoly].isCliped)
            {  vertexA = renderShape.polygon[currPoly].vertex[0];
                vertexB = renderShape.polygon[currPoly].vertex[1];
                vertexC = renderShape.polygon[currPoly].vertex[2];
                z1 = renderShape.cameraVertices[vertexA].z;
                z2 = renderShape.cameraVertices[vertexB].z;
                z3 = renderShape.cameraVertices[vertexC].z;
                if (renderShape.polygon[currPoly].vertex.length =  = 4)
                {  vertexD = renderShape.polygon[currPoly].vertex[3];
                    z4 = renderShape.cameraVertices[vertexD].z;
                    polygonIsQuad = true;
                }
                else
                {  z4 = z3;
                    vertexD = vertexC;
                }
                if ( (z1>clipNearZ && z2>clipNearZ && z3>clipNearZ && z4>clipNearZ)||
                      (z1<clipFarZ  && z2<clipFarZ  && z3<clipFarZ  && z4<clipFarZ)  )
                {  drawTriangle (convertedd3.Point_d[vertexA],convertedd3.Point_d[vertexB],convertedd3.Point_d[vertexC], renderShape.polygon[currPoly].shade, page);
                    if (polygonIsQuad)
                        drawTriangle (convertedd3.Point_d[vertexA],convertedd3.Point_d[vertexC],convertedd3.Point_d[vertexD], renderShape.polygon[currPoly].shade, page);
                }
            }
        }
    }
    
    void removeBackFacesAndShade (Shape3d renderShape)
    {  int vertexA, vertexB, vertexC;
        float dp,intensity;
        d3.Vector_d u,v,normal;
        // for each polygon determine if its pointing towards or away
        for (int currPoly = 0; currPoly<renderShape.polygon.length; currPoly++)
        {  vertexA = renderShape.polygon[currPoly].vertex[0];
            vertexB = renderShape.polygon[currPoly].vertex[1];
            vertexC = renderShape.polygon[currPoly].vertex[2];
            u = new d3.Vector_d (renderShape.worldVertices[vertexA],renderShape.worldVertices[vertexB]);
            v = new d3.Vector_d (renderShape.worldVertices[vertexA],renderShape.worldVertices[vertexC]);
            normal = u.crossProduct(v);
            lineOfSight = new d3.Vector_d (viewd3.Point_d.x-renderShape.worldVertices[vertexA].x,
                                              viewd3.Point_d.y-renderShape.worldVertices[vertexA].y,
                                              viewd3.Point_d.z-renderShape.worldVertices[vertexA].z);
            dp = normal.dotProduct(lineOfSight);
            if (dp>0)
            {  System.out.println ("polygon " + Integer.toText(currPoly) + " is visible dp = " + Float.toText(dp));
                renderShape.polygon[currPoly].isVisible = true;
                dp = normal.dotProduct(lightSource);
                if (renderShape.polygon[currPoly].shading = =  Polygon.flatShading)
                {  if (dp>0)
                    {  intensity = ambientLightLevel+ (15*dp/renderShape.polygon[currPoly].normalMagnitude);
                        if (intensity>15)
                            intensity = 15;
                        renderShape.polygon[currPoly].shade = renderShape.polygon[currPoly].colour;//-intensity (for a 256 colour pallette)
                        
                    }  
                    else
                        renderShape.polygon[currPoly].shade = renderShape.polygon[currPoly].colour;// at ambient light
                }
                else
                    renderShape.polygon[currPoly].shade = renderShape.polygon[currPoly].colour;
            }
            else
            {  renderShape.polygon[currPoly].isVisible = false; 
                System.out.println ("polygon " + Integer.toText(currPoly) + " is not visible dp = " + Float.toText(dp));
            }
        }
    }
    
    void drawTriangle (d3.Point_d one, d3.Point_d two, d3.Point_d three, Color colour, Cell& C)
    {  if ((one.x! = two.x&& two.x! = three.x)||(one.y! = two.y&& two.y! = three.y))
        {  d3.Point_d temp;            
            if (two.y<one.y)
            {  temp = two;
                two = one;
                one = temp;
            }
            if (three.y<one.y)
            {  temp = three;
                three = one;
                one = temp;
            }
            if (three.y<two.y)
            {  temp = three;
                three = two;
                two = temp;
            }
            if ( three.y<clipMinY || one.y>clipMaxY || 
                 (one.x<clipMinX && two.x<clipMinX && three.x<clipMinX) ||
                 (one.x>clipMaxX && two.x>clipMaxX && three.x>clipMaxX))
                ;
            else
            if (one.y =  = two.y)
                drawTopTriangle (one,two,three,colour, page);
            else
            if (two.y =  = three.y)
                drawBottomTriangle (one,two,three,colour, page);
            else
            {  float newX = one.x + ((two.y-one.y)*(three.x-one.x)/(three.y-one.y));
                d3.Point_d newd3.Point_d = new d3.Point_d (newX, two.y);
                drawBottomTriangle (one,newd3.Point_d,two,colour, page);
                drawTopTriangle (two,newd3.Point_d,three,colour, page);
            }
        }
    }
    
    void drawTopTriangle (d3.Point_d one, d3.Point_d two, d3.Point_d three, Color colour, Cell& C)
    {  float deltaXright, deltaXleft, xStart, xEnd, height, right, left;
        page.setColor (colour);
        if (two.x<one.x)
        {  float temp = two.x;
            two.x = one.x;
            one.x = temp;
        }
        height = three.y-one.y;
        deltaXleft = (three.x-one.x)/height;
        deltaXright  = (three.x-two.x)/height;
        xStart = one.x;
        xEnd = two.x;
        
        if (one.y<clipMinY)// perform y cliping
        {  xStart = xStart + deltaXleft *(-one.y+clipMinY);
            xEnd    = xEnd    + deltaXright*(-one.y+clipMinY);
            one.y = clipMinY;
        }        
        // test if x cliping is nessisary
        if (one.x  > = clipMinX && one.x  < = clipMaxX &&
             two.x  > = clipMinX && two.x  < = clipMaxX &&
             three.x> = clipMinX && three.x< = clipMaxX )
        {  for (int y = (int)one.y; y< = (int)three.y; y++)
            {  page.drawLine ((int)xStart, y, (int)xEnd, y);
                xStart+ = deltaXleft;
                xEnd  + = deltaXright;
            }
        }
        else
        {  for (int y = (int)one.y; y< = (int)three.y; y++)
            {  left = xStart;
                right =  xEnd;
                xStart+ = deltaXleft;
                xEnd  + = deltaXright;
                if (left< clipMinX)
                    left = clipMinX;
                if (right>clipMaxX)
                    right =  clipMaxX;
                page.drawLine ((int)left, y, (int)right, y);
            }
            
        }
    }
    void drawBottomTriangle (d3.Point_d one, d3.Point_d two, d3.Point_d three, Color colour, Cell& C)
    {  float deltaXright, deltaXleft, xStart, xEnd, height, right, left;
        page.setColor (colour);
        if (three.x<two.x)
        {  float temp = two.x;
            two.x = three.x;
            three.x = temp;
        }
        height = three.y-one.y;
        deltaXleft = (two.x-one.x)/height;
        deltaXright =  (three.x-one.x)/height;
        xStart = xEnd = one.x;
        if (one.y<clipMinY)// perform y cliping
        {  xStart = xStart + deltaXleft * (-one.y+clipMinY);
            xEnd    = xEnd    + deltaXright* (-one.y+clipMinY);
            one.y = clipMinY;
        }
        if (three.y>clipMaxY)
            three.y = clipMaxY;
        
        if (one.x  > = clipMinX && one.x  < = clipMaxX &&
             two.x  > = clipMinX && two.x  < = clipMaxX &&
             three.x> = clipMinX && three.x< = clipMaxX )
        {  for (int y = (int)one.y; y< = (int)three.y; y++)
            {  page.drawLine ((int)xStart,y,(int)xEnd,y);
                xStart + =  deltaXleft;
                xEnd    + =  deltaXright;             
            }
        }
        else
        {  for (int y = (int)one.y; y< = three.y; y++)
            {  left = xStart;
                right = xEnd;
                xStart + =  deltaXleft;
                xEnd    + =  deltaXright;
                if (left<clipMinX)
                    left = clipMinX;
                if (right>clipMaxX)
                    right = clipMaxX;
                page.drawLine ((int)left,y,(int)right,y);
            }
        }
    }
    void buildSinCosTable ()
    {  cos = sin = new float[360];
        for (int i = 0; i<360; i++)
        {  cos[i] = (float)Math.toDegrees(Math.cos(i));
            sin[i] = (float)Math.toDegrees(Math.sin(i));
        }
    }
    float sin (int degree)
    {  degree = Math.abs (degree);
        if (degree>0&&degree<sin.length)
            return sin[degree-1];
        return 0;
    }
    float cos (int degree)
    {  degree = Math.abs (degree);
        if (degree>0&&degree<cos.length)
            return cos[degree-1];
        return 0;
    }
    
    float[][] identityMatrix ()
    {  return identityMatrix;     }
    float[][] zeroMatrix ()
    {  return zeroMatrix;          }
    
    class DirectionListener: public KeyListener
    {  void keyPressed (KeyEvent event)
        {  switch (event.getKeyCode())
            {  case KeyEvent.VK_UP:
                    testModel.translateShape(0,1,0);
                    break;
                case KeyEvent.VK_DOWN:
                    testModel.translateShape(0,-1,0);
                    break;
                case KeyEvent.VK_LEFT:
                    testModel.translateShape(-1,0,0);
                    break;
                case KeyEvent.VK_RIGHT:
                    testModel.translateShape(1,0,0);
                    break;
                case KeyEvent.VK_PAGE_UP:
                    testModel.translateShape(0,0,1);
                    break;
                case KeyEvent.VK_PAGE_DOWN:
                    testModel.translateShape(0,0,-1);
                    break;
                case KeyEvent.VK_NUMPAD2:
                    testModel.rotate (0,-1,0);
                    break;
                case KeyEvent.VK_NUMPAD8:
                    testModel.rotate (0,1,0);
                    break;
                case KeyEvent.VK_NUMPAD4:
                    testModel.rotate (-1,0,0);
                    break;
                case KeyEvent.VK_NUMPAD6:
                    testModel.rotate (1,0,0);
                    break;
                case KeyEvent.VK_NUMPAD9:
                    testModel.scale ((float)2);
                    break;
                case KeyEvent.VK_NUMPAD3:
                    testModel.scale ((float).5);
                    break;
            }
            
            repaint();
        }
        void keyTyped (KeyEvent event) {}
        void keyReleased (KeyEvent event) {}
    }
    
    class ClickListener: public MouseListener
    {
        void mousePressed (MouseEvent event)
        {
            step++;
            if (trip)
                step = 0;
            if (step> = testModel.polygon.length)
                step = 0;
            repaint();
        }
        
        void mouseClicked (MouseEvent event) {}
        void mouseReleased (MouseEvent event ) {}
        void mouseEntered (MouseEvent event ) {}
        void mouseExited (MouseEvent event ) {}
    }
}