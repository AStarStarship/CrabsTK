////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2016 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef SFML_FILEINPUTSTREAM_HPP
#define SFML_FILEINPUTSTREAM_HPP

#include <ChineseRoom-Config.hpp>
#include "InputStream.hpp"
#include <cstdio>
#include <string>

#ifdef ANDROID
namespace _ 
{
namespace priv
{
class _ChineseRoom_ ResourceStream;
}
}
#endif


namespace _ {
    
class _ChineseRoom_ FileInputStream : public InputStream
/*< Implementation of input stream based on a file.
    @ingroup system

    This class is a specialization of InputStream that reads from a file on 
    disk.

    It wraps a file in the common InputStream interface and therefore allows to 
    use generic classes or functions that accept such a stream, with a file on 
    disk as the data source.

    In addition to the virtual functions inherited from InputStream, 
    FileInputStream adds a function to specify the file to open.

    SFML resource classes can usually be loaded directly from a filename, so 
    this class shouldn't be useful to you unless you create your own algorithms 
    that operate on an InputStream.

    Usage example:

    @code
    void process (InputStream& stream);

    FileInputStream stream;
    if (stream.open ("some_file.dat"))
    process (stream);
    @endcode

    InputStream, MemoryInputStream */
{
    public:
    
    FileInputStream ();
    /*< Default constructor. */

    virtual ~FileInputStream ();
    /*< Default destructor. */

    bool open (const std::string& filename);
    /*< Open the stream from a file path

        @param filename Name of the file to open
        @return True on success, false on error. */
    
    virtual int64_t read (void* data, int64_t size);
    /*< Read data from the stream

        After reading, the stream's reading position must be
        advanced by the amount of bytes read.

        @param data Buffer where to copy the read data
        @param size Desired number of bytes to read
        @return The number of bytes actually read, or -1 on error. */

    virtual int64_t seek (int64_t position);
    /*< Change the current reading position.

        @param position The position to seek to, from the beginning.
        @return The position actually sought to, or -1 on error. */

    virtual int64_t tell ();
    /*< Get the current reading position in the stream.

        @return The current position, or -1 on error.. */

    virtual int64_t getSize ();
    /*< Return the size of the stream.
    
        @return The total number of bytes available in the stream, or -1 on error. */

    private:

    _NonCopyable_ (FileInputStream)

#ifdef ANDROID
    priv::ResourceStream* m_file;
#else
    std::FILE* m_file; //< stdio file stream
#endif
};

} //< _


#endif // SFML_FILEINPUTSTREAM_HPP
