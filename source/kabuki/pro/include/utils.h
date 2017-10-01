/** Misc C++ Utils
    @file   utils.h
    @author Cale McCollough
    @brief  Various C++ utils shared between projects.
*/

#ifndef UTILS_H
#define UTILS_H

#include "config.h"
#include <time.h>

namespace _pro {

enum {
    NUM_SEC_MIN = 60 * 60,
    NUM_SEC_HOUR = 60 * NUM_SEC_MIN,
    NUM_SEC_DAY = 24 * NUM_SEC_HOUR,
    TIME_EPOCH = 1900,
};

/** Reads a single char from the keyboard 
    @param header The header to print. */
char ReadChar (const char* header);

/** Reads an int from the keyboard.
    @param header The header to print. 
    @param number The number to read. */
int ReadInt (const char* header);

/** Reads a float from the keyboard. 
    @param header The header to print. 
    @param number The number to read. */

float ReadFloat (const char* header);

/** Reads a new string from the keyboard.
    @header The header to print.
    @return Returns a new string that you have to delete. */
char* ReadString (const char* header, int buffer_size);

/** Reads a new string from the keyboard.
    @param  header      The header to print.
    @param  buffer      A buffer to read keyboard input to.
    @param  buffer_size The size of the buffer.
    @return Returns a new string that you have to delete. */
void ReadString (const char* header, char* buffer, int buffer_size);

/** Clones the given string. */
char* CloneString (const char* string);

/** Prints the given string with a '|' at the beginning followed by a new line */
void PrintBar (const char* string);

/** Prints a number of lines to the console. */
void PrintLines (int num_lines = 1);

/** Prints a line break. */
void PrintBreak (const char* header = "\n_", char c = '_', int num_lines = 0, 
                 int console_width = 80);

/** Prints the given string centered. */
void PrintCentered (const char* string, int width, bool is_last = false,
                    char column_delimeter = '|');

/** Prints a line break with the given number of columns. */
void PrintColumnBreak (int num_columns, char column_delimeter = '|', char break_char = '-', 
                       int width = 80);

/** Prints the given string indented and bulleted with and '|' at the beginning 
    and end of the line representing a page.

    @code
    print ("List Item 1", 0
    @endcode

    @param string The string to print.
    @param indent The number of tabs indent level.
    @param bullet The type of bullet to print.
    @param index  The index of the bullet in the list.
    @param tab_size The number of spaces per tab.
    @param num_columns The number of columns per line.
 */
void PrintPage (const char* string = "", int indentation = 0,
                 char bullet_type = '*', int index = 0, int tab_size = 4,
                 int num_columsn = 80);

/** Prints the given time to the console. */
void PrintTime (time_t t);

/** Prints the given tm struct to the console. */
bool PrintDateTime (tm* moment);

/** Prints the given time to the console. */
void PrintDateTime (time_t t);

/** Prints the given moment to the string buffer. */
bool PrintDateTimeString (char* buffer, int buffer_size, tm* moment);

/** Prints the given time to the string buffer. */
bool PrintDateTimeString (char* buffer, int buffer_size, time_t t);

/** Gets the number of days in a months. 
    @todo Maybe get some open-source date utility? */
int NumDaysMonth (time_t t);

/** Gets the number of days in a months. 
    @param month The month index 0-11.
    @param year   */
int NumDaysMonth (int month, int year = 1);

/** Gets the abbreviated day of the week string of the given day number 1-7. */ 
const char* DayOfWeek (int day_num);

/** Gets the abbreviated day of the week string of the given day number 1-7. */ 
char DayOfWeekInitial (int day_number);

/** Returns the pointer to the next char in the string that is not an ASCII
    number.
    @return A pointer to the next non-number in the input string. */
const char* NextNonNumberString (const char* input);

/** Returns the pointer to the next char in the string that is not an ASCII 
    number.
    @return A pointer to the next non-number in the input string. */
char* NextNonNumber (char* input);

/** Returns the pointer to the next char in the string that is not an ASCII 
    number.
    @return A pointer to the next non-number in the input string. */
char* NextNonNumber (char* input);

/** Skips the leading zeros of a number if there are any. */
const char* SkipLeadingZerosString (const char* input);

/** Skips the leading zeros of a number if there are any. */
char* SkipLeadingZeros (char* input);

/** Compares the two the time and prints the results. */
int CompareTimes (time_t t_a, time_t t_b);

/** Compares the given time_t to the time and prints the results. */
int CompareTimes (time_t t, int year, int month, int day,
                   int  hour, int minute, int second);

/** Parses a time or time delta from a a string starting with an '@' sign..
    @param input  The string to parse.
    @param hour   The location to write the number of hours to.
    @param minute The location to write the number of minutes to.
    @param Second The location to write the number of seconds to.
    @return The offset The end of where the parser exited successfully at.
    @code
    @4        (This is 4AM)
    @4PM      (No space required)
    @4:20P    (Or M)
    @4:20 PM
    @16:20
    @4:20 am
    @4:20a.m.
    @4:20:00
    @4:20:00AM
    @16:20:00
    @endcode
*/
const char* ParseTime (const char* input, int* hour, int* minute, int* second);

/** Parses a time or time delta from a a string starting with an '@' sign..
    @param hour   The location to write the number of hours to.
    @param minute The location to write the number of minutes to.
    @param Second The location to write the number of seconds to.
    @return The offset The end of where the parser exited successfully at.
*/
char* ParseTime (char* input, int* hour, int* minute, int* second);

/** Converts a keyboard input to string and deletes the string. 
    The only format this function currently supports is 2017-08-12@11:36:00PM".
    @todo Add new and auto-detect formats.
          1/2/99,  1/2/1999, 1-2-1999, 2017-8-12
*/
const char* ParseTimeString (const char* input, tm* result);

/** Converts a keyboard input to a human-readable manipulatable form. */
char* ParseTime (char* input, tm* result);

/** Converts a keyboard input to a time_t. */
const char* ParseTimeString (const char* input, time_t& result);

/** Converts a keyboard input to a time_t. */
char* ParseTime (char* input, time_t& result);

/** Zeros out the calendar_time struct.
    @param calendar_time A calendar time struct to zero out. */
void ZeroTime (tm* calendar_time);

/** Skips all the spaces at the start of the string. */
const char* SkipSpacesString (const char* input);

/** Skips all the spaces at the start of the string. */
char* SkipSpaces (char* string);

/** Gets the end of the current token from the string.
    @return Returns a pointer to the end of the token. */
const char* EndOfTokenString (const char* input);

/** Gets the end of the current token from the string.
    @return Returns a pointer to the end of the token. */
char* EndOfToken (char* input);

/** Scrolls over to the next double quote mark. */
const char* EndOfString (const char* input, char delimiter = '\"');

/** Scrolls over to the next double quote mark. */
char* EndOf (char* input, char delimiter = '\"');

/** Compares the source and query string using the delimiter to terminate the query. */
const char* CompareTokenString (const char* input, const char* query);

/** Compares the source and query string using the delimiter to terminate the query. */
char* CompareToken (const char* input, const char* query);

/** Compares the source and query string using the delimiter to terminate the query. */
const char* CompareString (const char* input, const char* query, char delimiter = 0);

/** Compares the source and query string using the delimiter to terminate the query. */
char* Compare (char* source, const char* query, char delimiter = 0);

/** Parses the given string for the given token, setting the following 
    whitespace to null.
    @param  string The string to search.
    @param  token  The token to search for.
    @return Returns null if the search failed and a pointer to the first hit
            upon success.
*/
const char* ParseString (const char* input, char* destination, int size, char delimiter = 0);

/** Parses the given string for the given token, setting the following 
    whitespace to null.
    @param  string The string to search.
    @param  token  The token to search for.
    @return Returns null if the search failed and a pointer to the first hit
            upon success.
*/
char* Parse (char* input, char* destination, int size, char delimiter = 0);

/** Parses the token by scrolling to the end of it and setting the end to null.
    @param  input The string to search.
    @return Returns null if the search failed and a pointer to the first hit
            upon success. */
char* ParseToken (char* input);

/** Searches the given string for the given string.
    @param  input      The string to search.
    @param  query      The string to search for.
    @param  delimiter The delimiter for the token, Example: '\"'
    @return Returns null if the parsing failed and a pointer to the first char
            after the end of the token upon success. */
const char* FindString (const char* input, const char* query, char delimiter = 0);

/** Searches the given string for the given string, setting the following 
    whitespace to null.
    @param  input      The string to search.
    @param  query      The token to search for.
    @param  delimiter The delimiter for the token, Example: '\"'
    @return Returns null if the search failed and a pointer to the first hit
            upon success.
*/
char* Find(char* input, const char* string, char delimiter = 0);

/** Parses the string for a int.
    @return Returns null there was not a number and a pointer to after the 
            number ends in the string upon success. */

const char* ParseIntString (const char* string, int* value);

/** Parses the string for a int.
    @return Returns null there was not a number and a pointer to after the 
            number ends in the string upon success. */
char* ParseInt (char* string, int* value);

/** Parses the string for a float.
    @return Returns null there was not a number and a pointer to after the 
            number ends in the string upon success. */

const char* ParseFloatString (const char* string, float* value);

/** Parses the string for a float.
    @return Returns null there was not a number and a pointer to after the 
            number ends in the string upon success. */
char* ParseFloat (char* string, float* value);

/** Returns the length of the string with the given delimiter or null-term 
    char. */
int StringLength (const char* input, char delimiter);

/** Checks if the given string is a token. 
    @param input The string to check.
    @returns Returns true if the given string is a token. */
bool IsToken (const char* input);

}        //< namespace _pro
#endif   //< UTILS_H

