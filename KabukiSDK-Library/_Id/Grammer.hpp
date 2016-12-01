/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_Doc/Grammer.hpp
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

#include <FreeI2P.hpp>
#include <KabukiSDK-Config.hpp>

namespace _Id {
/** Used for settings rules to text grammer.
	Useful for rules for passwords.
*/
class Grammer
{
	public:
	
	enum
	{
		ToShortFlag = 1,       			//< 
		ToLongFlag,                     //< 
		ContainsInvalidCharsFlag,       //< 
		MissingMandatoryCharsFlag,      //< 
		MustStartWithCharFlag        	//< 
	};
	
	Grammer ();
	/*< Default Contructor initializes with no rules. */

	Grammer (Grammer initState);
	/*< Copy Constructor. */

	bool IsValid (const char* aString);
	/*< Verifies thisstring to see if it has proper grammer. */

	int GetMinLength ();
	/*< Gets the min length of a valid symbol. */
	
	bool SetMinLength (int value);
	/*< Sets the min length of a valid symbol. */

	int GetMaxLength ();
	/*< Gets the max length. */
    
	bool SetMinLength (int value);
	/*< Gets the max length. */

	bool IsIllegal (char aChar);
	/*< Checks to see if thisChar is an illegal char. */

	bool MakeIllegal (char thisChar, string descstring);
	/*< Adds char index of illegal chars.
		@pre thisChar  must be > 0.
		@param thisChar The illegal char to add.
	*/

	bool MakeIllegal (int startIndex, int stopIndex, string descstring);
	/*< Adds char or range of indexs of illegal chars.
		@pre startIndex and stopIndex must be a printable char.
		@param startIndex The starting ASCII char index.
		@param stopIndex  The stoping ASCII char index. */

	bool MakeMandatory (int startIndex, int stopIndex, int numInstances, string descstring);
	/*< Adds a char or set of range of char indexes that must be included in a valid symbol.
		@pre startIndex and stopIndex must be a printable char and numInstances must be > 0 and < maxLength.
		@param startIndex The starting ASCII char index.
		@param stopIndex  The stoping ASCII char index.
		@param numInstances  The number of times the symbol range must occur. */
    
    void print (I2P::Terminal& slot);
    /*< Prints this object to a terminal. */

	private:

	int numIllegalCharRanges,   		//< The number of illegal char ranges.
		numMandatoryCharRanges,         //< The number of mandatory char ranges.
		minLength,                      //< The min length of a valid symbol.
		maxLength;                      //< The max length of a valid symbol.

	int illegalChars,           		//< A 2 column table of ints that represents all of illegal char ranges.
		 mandatoryChars,                //< A 3 column table of ints that represents all of mandatory char ranges and how many are mandatory.
		 mustStartWithChars;            //< A 2 column table of ints that represents the range (s) of chars that the symbol must start with.

	string illegalCharsDesc,    		//< An array of strings that describes what illegal char (s) are.
		mandatoryCharsDesc,             //< An array of strings that describes what what the mandatory char (s) are.
		mustStartWithCharsDesc;         //< An array of strings that describes what mandatory first char (s) are.
};

}   //< namespace _Id
 