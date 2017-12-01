/** kabuki:cards
    @file    /.../KabukiTheater-Examples/kabuki_cards/kabuki_cards/Blackjack/Dealer.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#include "Card.h"

namespace DojoCardGames {

DojoCard::Card ()
{
    faceValue=0;
    suit=0;
	suitstring="Empty";
}  
   
void DojoCard::setCard( int_w newSuit,  int_w newFace)
{
    suit=newSuit;
    faceValue=newFace;

    switch (suit)
    {
        case 0:
        suitstring = " ";
        break;
        case 1:
        suitstring = "heart";
        break;
        case 2:
        suitstring = "diamond";
        break;			
        case 3:
        suitstring = "club";			
        break;
        case 4:
        suitstring = "spaid";			
        break;
    }
    suitIcon = new ImageIcon (suitstring.concat(".jpg"));
}
   
void DojoCard::setCard(Card cardTemp)
{
    suitstring = cardTemp.suitstring();
    faceValue = cardTemp.faceValue();
    suitstring= cardTemp.suitstring();
    suitIcon = new ImageIcon (suitstring.concat(".jpg"));
}
	
string DojoCard::suitstring()
{
    return suitstring;
}
   
 int_w DojoCard::faceValue()
{
    return faceValue;
}
   
 int_w DojoCard::compareTo(Object other)
{
    Card c = (Card)other;
         
    if (this.suitstring() == null)
        return 1;
    if (this.suitstring().equals(c.suitstring()))
    {
        if (this.faceValue < c.faceValue)
        return -1;
        if (this.faceValue > c.faceValue)
        return 1;
    }
    else
    {
        return 1;
    }
    return 0;
}
   
boolean DojoCard::equals (Card compairCard)
{
    if (compairCard.compareTo(this) == 0)
        return true;
    return false;
}
	
string DojoCard::tostring()
{ 
    string cardstring;

	if (faceValue == 1)
		cardstring = ("Ace of " + suitstring + ", ");
	else
		if (faceValue == 11)
        cardstring = ("Jack of " + suitstring + ", ");
		else
			if (faceValue == 12)
   			cardstring = ("Queen of " + suitstring + ", ");
			else
				if (faceValue == 13)
					cardstring = ("King of " +suitstring + ", ");
			else
				if (faceValue == 0)
					cardstring = ("Empty" + ", ");
				else
				    cardstring = (faceValue + " of " + suitstring + ", ");
    return cardstring;
}
   
int_w DojoCard::xPos()
{ return xPos; }
   
void DojoCard::setX(int_w newX)
{ xPos = newX; }
   
int_w DojoCard::yPos()
{ return yPos; }
   
void DojoCard::setY(int_w newY)
{ yPos = newY; }
   
void DojoCard::draw(Component g, Graphics page)
{  
    xPos = x;
    yPos = y;
      
    if (!suitstring.equals("Empty"))
    {
        page.setColor (Color.white);
        page.fillRoundRect (xPos,yPos, 60, 90, 15, 15);
        page.setColor (Color.black);
        page.drawRoundRect (xPos,yPos, 60, 90, 15, 15);
        if (faceValue==1)
        page.drawstring ("Ace", xPos+28, yPos+50);
        else
        if (faceValue==11)
            page.drawstring ("J", xPos+28, yPos+50);
        else
        {
            if (faceValue==12)
                page.drawstring ("Q", xPos+28, yPos+50);
            else
            {
                if (faceValue==13)
                    page.drawstring ("K", xPos+28, yPos+50);
                else
                {
                    page.drawstring (Integer.tostring(faceValue), xPos+6, yPos+36);
                    page.drawstring (Integer.tostring(faceValue), xPos+41, yPos+60);
                }
                }
        }
        suitIcon.paintIcon (g, page, x+5 , y+5 );
        suitIcon.paintIcon (g, page, x+40, y+5 );
        suitIcon.paintIcon (g, page, x+5 , y+65);
        suitIcon.paintIcon (g, page, x+40, y+65);
    }
}

}   //< kabuki_cards
