////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2017 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//  you must not claim that you wrote the original software.
//  If you use this software in a product, an acknowledgment
//  in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//  and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef CHINESEROOM_STRING_H
#define CHINESEROOM_STRING_H

#include "config.h"

#if USING_UTF
#include "utf.h"

namespace _ {

template<typename uint_t>
class String
/** Utility string class that automatically handles conversions between types and encodings.
    @ingroup _
   
    _::String is a utility string class defined mainly for
    convenience. It is a Unicode string (implemented using
    UTF-32), thus it can store any character in the world
 (European, Chinese, Arabic, Hebrew, etc.).
   
    It automatically handles conversions from/to ANSI and
    wide strings, so that you can work with standard string
    classes and still be compatible with functions taking a
    _::String.
   
    @code
    _::String s;
   
    std::string s1 = s;  // automatically converted to ANSI string
    std::wstring s2 = s; // automatically converted to wide string
    s = "hello";         // automatically converted from ANSI string
    s = L"hello";        // automatically converted from wide string
    s += 'a';            // automatically converted from ANSI string
    s += L'a';           // automatically converted from wide string
    @endcode
   
    Conversions involving ANSI strings use the default user locale. However
    it is possible to use a custom locale if necessary:
    
    @code
    std::locale locale;
    _::String s;
    ...
    std::string s1 = s.toANSIString (locale);
    s = _::String ("hello", locale);
    @endcode
   
    _::String defines the most important functions of the
    standard std::string class: removing, random access, iterating,
    appending, comparing, etc. However it is a simple class
    provided for convenience, and you may have to consider using
    a more optimized class if your program requires complex string
    handling. The automatic conversion functions will then take
    care of converting your string to _::String whenever SFML
    requires it.
   
    Please note that SFML also defines a low-level, generic
    interface for Unicode handling, see the _::UTF classes. */
{
    public:

    typedef std::basic_string<uint32_t>::iterator       Iterator;      //< Iterator type
    typedef std::basic_string<uint32_t>::const_iterator ConstIterator; //< Read-only iterator type

    static const std::size_t InvalidPos; //< Represents an invalid position in the string
    
    /** Default constructor.
        This constructor creates an empty string. */
    String () {}
    
    /** Construct from a single ANSI character and a locale
        The source character is converted to UTF-32 according
        to the given locale.

        @param ansiChar ANSI character to convert
        @param locale   Locale to use for conversion. */
    String (byte ansiChar, const std::locale& locale = std::locale ()) {
        string_ += UTF32::decodeANSI (ansiChar, locale);
    }
    
    /** Construct from single wide character
        @param wideChar Wide character to convert. */
    String (wchar_t wideChar) {
        string_ += UTF32::decodeWide (wideChar);
    }
    
    /** Construct from single UTF-32 character.
        @param utf32Char UTF-32 character to convert. */
    String (uint32_t utf32Char)  {
        string_ += utf32Char;
    }

    /** Construct from a null-terminated C-style ANSI string and a locale
        The source string is converted to UTF-32 according
        to the given locale.

        @param ansiString ANSI string to convert
        @param locale     Locale to use for conversion. */
    String (const byte* ansiString, const std::locale& locale = std::locale ()) {
        if (ansiString)
        {
            std::size_t length = strlen (ansiString);
            if (length > 0)
            {
                string_.reserve (length + 1);
                UTF32::fromANSI (ansiString, ansiString + length, std::back_inserter (string_), locale);
            }
        }
    }
    
    /** Construct from an ANSI string and a locale.
        The source string is converted to UTF-32 according
        to the given locale.

        @param ansiString ANSI string to convert
        @param locale     Locale to use for conversion. */
    String (const std::string& ansiString, const std::locale& locale = std::locale ()) {
        string_.reserve (ansiString.length () + 1);
        UTF32::fromANSI (ansiString.begin (), ansiString.end (), std::back_inserter (string_), locale);
    }

    String (const wchar_t* wideString)
    /** Construct from null-terminated C-style wide string.
        @param wideString Wide string to convert. */
    {
        if (wideString)
        {
            std::size_t length = std::wcslen (wideString);
            if (length > 0)
            {
                string_.reserve (length + 1);
                UTF32::fromWide (wideString, wideString + length, std::back_inserter (string_));
            }
        }
    }
    
    /** Construct from a wide string.
        @param wideString Wide string to convert. */
    String (const std::wstring& wideString) {
        string_.reserve (wideString.length () + 1);
        UTF32::fromWide (wideString.begin (), wideString.end (), std::back_inserter (string_));
    }
    
    /** Construct from a null-terminated C-style UTF-32 string.
        @param utf32String UTF-32 string to assign. */
    String (const uint32_t* utf32String) {
        if (utf32String)
            string_ = utf32String;
    }
    
    /** Construct from an UTF-32 string.
        @param utf32String UTF-32 string to assign. */
    String (const std::basic_string<uint32_t>& utf32String)
    :   string_ (utf32String)
    {}

    String (const String& copy)
    /** Copy constructor
        @param copy Instance to copy. */
    :  string_ (copy.string_)
    {}
    
    /** Create a new _::String from a UTF-8 encoded string.

        @param begin Forward iterator to the beginning of the UTF-8 sequence
        @param end   Forward iterator to the end of the UTF-8 sequence

        @return A _::String containing the source string

        @see fromUTF16, fromUTF32. */
    template <typename uint_t>
    static String fromUTF8 (uint_t begin, uint_t end) {
        String string
        UTF8::toUTF32 (begin, end, std::back_inserter (string.string_))
        return string
    }
    
    /** Create a new _::String from a UTF-16 encoded string.

        @param begin Forward iterator to the beginning of the UTF-16 sequence
        @param end   Forward iterator to the end of the UTF-16 sequence

        @return A _::String containing the source string

        @see fromUTF8, fromUTF32. */
    template <typename uint_t>
    static String fromUTF16 (uint_t begin, uint_t end) {
        String string
        UTF16::toUTF32 (begin, end, std::back_inserter (string.string_))
        return string
    }
    
    /** Create a new _::String from a UTF-32 encoded string.

        This function is provided for consistency, it is equivalent to
        using the constructors that takes a const _::uint32_t* or
        a std::basic_string<_::uint32_t>.

        @param begin Forward iterator to the beginning of the UTF-32 sequence
        @param end   Forward iterator to the end of the UTF-32 sequence

        @return A _::String containing the source string

        @see fromUTF8, fromUTF16. */
    template <typename uint_t>
    static String fromUTF32 (uint_t begin, uint_t end) {
        String string
        string.string_.assign (begin, end)
        return string
    }
    
    /** Implicit conversion operator to std::string (ANSI string).
        The current global locale is used for conversion. If you
        want to explicitly specify a locale, see toANSIString.
        Characters that do not fit in the target encoding are
        discarded from the returned string.
        This operator is defined for convenience, and is equivalent
        to calling toANSIString ().

        @return Converted ANSI string

        @see toANSIString, operator std::wstring. */
    operator std::string () const {
        return toANSIString ();
    }
    
    /** Implicit conversion operator to std::wstring (wide string).
        Characters that do not fit in the target encoding are
        discarded from the returned string.
        This operator is defined for convenience, and is equivalent
        to calling toWideString ().
        @return Converted wide string
        @see toWideString, operator std::string. */
    operator std::wstring () const {
        return toWideString ();
    }
    
    /** Convert the Unicode string to an ANSI string.
        The UTF-32 string is converted to an ANSI string in
        the encoding defined by \a locale.
        Characters that do not fit in the target encoding are
        discarded from the returned string.
        @param locale Locale to use for conversion
        @return Converted ANSI string
        @see toWideString, operator std::string. */
    std::string toANSIString (const std::locale& locale = std::locale ()) const
    {
        // Prepare the output string
        std::string output;
        output.reserve (string_.length () + 1);

        // Convert
        UTF32::toANSI (string_.begin (), string_.end (), std::back_inserter (output), 0, locale);

        return output;
    }
    
    /** Convert the Unicode string to a wide string.
        Characters that do not fit in the target encoding are
        discarded from the returned string.
        @return Converted wide string.
        @see toANSIString, operator std::wstring. */
    std::wstring toWideString () const {
        // Prepare the output string
        std::wstring output;
        output.reserve (string_.length () + 1);

        // Convert
        UTF32::toWide (string_.begin (), string_.end (), std::back_inserter (output), 0);

        return output;
    }
    
    /** Convert the Unicode string to a UTF-8 string.
        @return Converted UTF-8 string.
        @see toUTF16, toUTF32. */
    std::basic_string<byte> toUTF8 () const {
        // Prepare the output string
        std::basic_string<byte> output;
        output.reserve (string_.length ());

        // Convert
        UTF32::toUTF8 (string_.begin (), string_.end (), std::back_inserter (output));

        return output;
    }
    
    /** Convert the Unicode string to a UTF-16 string.
        @return Converted UTF-16 string.
        @see toUTF8, toUTF32. */
    std::basic_string<uint16_t> toUTF16 () const {
        // Prepare the output string
        std::basic_string<uint16_t> output;
        output.reserve (string_.length ());

        // Convert
        UTF32::toUTF16 (string_.begin (), string_.end (), std::back_inserter (output));

        return output;
    }
    
    /** Convert the Unicode string to a UTF-32 string.
        This function doesn't perform any conversion, since the
        string is already stored as UTF-32 internally.
        @return Converted UTF-32 string
        @see toUTF8, toUTF16. */
    std::basic_string<uint32_t> toUTF32 () const {
        return string_;
    }
    
    /** Overload of assignment operator
        @param right Instance to assign
        @return Reference to self. */
    String& operator = (const String& right) {
        string_ = right.string_;
        return *this;
    }
    
    /** Overload of += operator to append an UTF-32 string.
        @param right String to append.
        @return Reference to self. */
    String& operator += (const String& right) {
        string_ += right.string_;
        return *this;
    }
    
    /** Overload of [] operator to access a character by its position.
        This function provides read-only access to characters.
        Note: the behavior is undefined if \a index is out of range.
        @param index The index of the character to get
        @return Character at position \a index. */
    uint32_t operator [] (std::size_t index) const {
        return string_[index];
    }
    
    /** Overload of [] operator to access a character by its position.
        This function provides read and write access to characters.
        Note: the behavior is undefined if \a index is out of range.
        @param index The index of the character to get.
        @return Reference to the character at position \a index. */
    uint32_t& operator [] (std::size_t index) {
        return string_[index];
    }
    
    /** Clears the string.
        This function removes all the characters from the string.
        @see isEmpty, erase. */
    void clear () {
        string_.clear ();
    }
    
    /** Get the size of the string.
        @return Number of characters in the string.
        @see isEmpty. */
    std::size_t getSize () const {
        return string_.size ();
    }
    
    /** Check whether the string is empty or not.
        @return True if the string is empty (i.e. contains no character).
        @see clear, getSize. */
    bool isEmpty () const {
        return string_.empty ();
    }
    
    /** Erase one or more characters from the string

        This function removes a sequence of \a count characters
        starting from \a position.

        @param position The position of the first character to erase.
        @param count    Number of characters to erase. */
    void erase (std::size_t position, std::size_t count = 1) {
        string_.erase (position, count);
    }
    
    /** Insert the given string starting from the given position.
        @param position The position to insert at.
        @param str      Characters to insert. */
    void Insert (std::size_t position, const String& s) {
        string_.insert (position, str.string_);
    }
    
    /** Find a sequence of one or more characters in the string.
        This function searches for the characters of \a str
        in the string, starting from \a start.
        @param str   Characters to find
        @param start Where to begin searching.
        @return Position of \a str in the string, or String::InvalidPos if not found. */
    std::size_t find (const String& str, std::size_t start = 0) const {
        return string_.find (str.string_, start);
    }
    
    /** Replace a substring with another string.
        This function replaces the substring that starts at index \a position
        and spans \a length characters with the string \a replaceWith.
        @param position Index of the first character to be replaced
        @param length   Number of characters to replace. You can pass InvalidPos to
            replace all characters until the end of the string.
        @param replaceWith String that replaces the given substring. */
    void replace (std::size_t position, std::size_t length, 
                  const String& replaceWith) {
        string_.replace (position, length, replaceWith.string_);
    }
    
    /** Replace all occurrences of a substring with a replacement string.
        This function replaces all occurrences of \a searchFor in this string
        with the string \a replaceWith.
        @param searchFor   The value being searched for
        @param replaceWith The value that replaces found \a searchFor values. */
    void replace (const String& searchFor, const String& replaceWith) {
        std::size_t step = replaceWith.getSize ();
        std::size_t len = searchFor.getSize ();
        std::size_t pos = find (searchFor);

        // Replace each occurrence of search
        while (pos != InvalidPos)
        {
            replace (pos, len, replaceWith);
            pos = find (searchFor, pos + step);
        }
    }

    String substring (std::size_t position, std::size_t length = InvalidPos) const
    /** Return a part of the string.
        This function returns the substring that starts at index \a position
        and spans \a length characters.
        @param position Index of the first character
        @param length   Number of characters to include in the substring (if
            the string is shorter, as many characters as possible
            are included). \ref InvalidPos can be used to include all
            characters until the end of the string.
        @return String object containing a substring of this object. */
    {
        return string_.substr (position, length);
    }

    const uint32_t* getData () const
    /** Get a pointer to the C-style array of characters.
        This functions provides a read-only access to a
        null-terminated C-style representation of the string.
        The returned pointer is temporary and is meant only for
        immediate use, thus it is not recommended to store it.
        @return Read-only pointer to the array of characters. */
    {
        return string_.c_str ();
    }

    Iterator begin ()
    /** Return an iterator to the beginning of the string.
        @return Read-write iterator to the beginning of the string characters
        @see end. */
    {
        return string_.begin ();
    }

    ConstIterator begin () const
    /** Return an iterator to the beginning of the string.
        @return Read-only iterator to the beginning of the string characters.
        @see end. */
    {
        return string_.begin ();
    }

    Iterator end ()
    /** Return an iterator to the end of the string.

        The end iterator refers to 1 position past the last character
        thus it represents an invalid character and should never be
        accessed.

        @return Read-write iterator to the end of the string characters.
        @see begin. */
    {
        return string_.end ();
    }

    ConstIterator end () const
    /** Return an iterator to the end of the string.
        The end iterator refers to 1 position past the last character;
        thus it represents an invalid character and should never be
        accessed.
        @return Read-only iterator to the end of the string characters.
        @see begin. */
    {
        return string_.end ();
    }

    private:

    friend bool operator == (const String& left, const String& right);

    friend bool operator < (const String& left, const String& right);

    std::basic_string<uint_t> string_; //< Internal string of UTF-32 characters
};

/** Overload of == operator to compare two UTF-32 strings.
    @relates String
    @param left  Left operand (a string).
    @param right Right operand (a string).
    @return True if both strings are equal. */
template<typename uint_t>
bool operator == (const String<uint_t>& left, const String<uint_t>& right) {
    return left.string_ == right.string_;
}

/** Overload of != operator to compare two UTF-32 strings.
    @relates String
    @param left  Left operand (a string).
    @param right Right operand (a string).
    @return True if both strings are different. */
template<typename uint_t>
bool operator != (const String<uint_t>& left, const String<uint_t>& right) {
    return ! (left == right);
}

/** Overload of < operator to compare two UTF-32 strings.
    @relates String
    @param left  Left operand (a string)
    @param right Right operand (a string)
    @return True if \a left is lexicographically before \a right */
template<typename uint_t>
bool operator < (const String<uint_t>& left, const String<uint_t>& right) {
    return left.string_ < right.string_;
}

/** Overload of > operator to compare two UTF-32 strings.
    @relates String
    @param left  Left operand (a string)
    @param right Right operand (a string)
    @return True if \a left is lexicographically after \a right */
template<typename uint_t>
bool operator > (const String<uint_t>& left, const String<uint_t>& right) {
    return right < left;
}

/** Overload of <= operator to compare two UTF-32 strings.
    @relates String
    @param left  Left operand (a string)
    @param right Right operand (a string)
    @return True if \a left is lexicographically before or equivalent to \a right */
template<typename uint_t>
bool operator <= (const String<uint_t>& left, const String<uint_t>& right) {
    return ! (right < left);
}

/** Overload of >= operator to compare two UTF-32 strings.
    @relates String
    @param left  Left operand (a string)
    @param right Right operand (a string)
    @return True if \a left is lexicographically after or equivalent to \a right. */
template<typename uint_t>
bool operator >= (const String<uint_t>& left, const String<uint_t>& right) {
    return ! (left < right);
}

/** @relates String
    @brief Overload of binary + operator to concatenate two strings
    @param left  Left operand (a string)
    @param right Right operand (a string)
    @return Concatenated string */
template<typename uint_t>
String<uint_t> operator + (const String<uint_t>& left, const String<uint_t>& right) {
    String string = left;
    string += right;

    return string;
}

}       //< namespace _
#endif  //< USING_UTF
#endif  //< CHINESEROOM_STRING_H
