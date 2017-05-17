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

#ifndef SFML_IPADDRESS_HPP
#define SFML_IPADDRESS_HPP
// Headers
////////////////////////////////////////////////////////////
#include <_Net/Export.hpp>
#include <_/Time.hpp>
#include <istream>
#include <ostream>
#include <string>


namespace _Net
{
////////////////////////////////////////////////////////////
/// \brief Encapsulate an IPv4 network address
///
////////////////////////////////////////////////////////////
class SFML_NETWORK_API IPAddress
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// This constructor creates an empty (invalid) address
    ///
    ////////////////////////////////////////////////////////////
    IPAddress();

    ////////////////////////////////////////////////////////////
    /// \brief Construct the address from a string
    ///
    /// Here \a address can be either a decimal address
    /// (ex: "192.168.1.56") or a network name (ex: "localhost").
    ///
    /// @param address IP address or network name
    ///
    ////////////////////////////////////////////////////////////
    IPAddress(const std::string& address);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the address from a string
    ///
    /// Here \a address can be either a decimal address
    /// (ex: "192.168.1.56") or a network name (ex: "localhost").
    /// This is equivalent to the constructor taking a std::string
    /// parameter, it is defined for convenience so that the
    /// implicit conversions from literal strings to IPAddress work.
    ///
    /// @param address IP address or network name
    ///
    ////////////////////////////////////////////////////////////
    IPAddress(const char* address);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the address from 4 bytes
    ///
    /// Calling IPAddress(a, b, c, d) is equivalent to calling
    /// IPAddress("a.b.c.d"), but safer as it doesn't have to
    /// parse a string to get the address components.
    ///
    /// @param byte0 First byte of the address
    /// @param byte1 Second byte of the address
    /// @param byte2 Third byte of the address
    /// @param byte3 Fourth byte of the address
    ///
    ////////////////////////////////////////////////////////////
    IPAddress(Uint8 byte0, Uint8 byte1, Uint8 byte2, Uint8 byte3);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the address from a 32-bits integer
    ///
    /// This constructor uses the internal representation of
    /// the address directly. It should be used for optimization
    /// purposes, and only if you got that representation from
    /// IPAddress::toInteger().
    ///
    /// @param address 4 bytes of the address packed into a 32-bits integer
    ///
    /// \see toInteger
    ///
    ////////////////////////////////////////////////////////////
    explicit IPAddress(uint32_t address);

    ////////////////////////////////////////////////////////////
    /// \brief Get a string representation of the address
    ///
    /// The returned string is the decimal representation of the
    /// IP address (like "192.168.1.56"), even if it was constructed
    /// from a host name.
    ///
    /// @return String representation of the address
    ///
    /// \see toInteger
    ///
    ////////////////////////////////////////////////////////////
    std::string toString() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get an integer representation of the address
    ///
    /// The returned number is the internal representation of the
    /// address, and should be used for optimization purposes only
    /// (like sending the address through a socket).
    /// The integer produced by this function can then be converted
    /// back to a sf::IPAddress with the proper constructor.
    ///
    /// @return 32-bits unsigned integer representation of the address
    ///
    /// \see toString
    ///
    ////////////////////////////////////////////////////////////
    uint32_t toInteger() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the computer's local address
    ///
    /// The local address is the address of the computer from the
    /// LAN point of view, i.e. something like 192.168.1.56. It is
    /// meaningful only for communications over the local network.
    /// Unlike getPublicAddress, this function is fast and may be
    /// used safely anywhere.
    ///
    /// @return Local IP address of the computer
    ///
    /// \see getPublicAddress
    ///
    ////////////////////////////////////////////////////////////
    static IPAddress getLocalAddress();

    ////////////////////////////////////////////////////////////
    /// \brief Get the computer's public address
    ///
    /// The public address is the address of the computer from the
    /// internet point of view, i.e. something like 89.54.1.169.
    /// It is necessary for communications over the world wide web.
    /// The only way to get a public address is to ask it to a
    /// distant website; as a consequence, this function depends on
    /// both your network connection and the server, and may be
    /// very slow. You should use it as few as possible. Because
    /// this function depends on the network connection and on a distant
    /// server, you may use a time limit if you don't want your program
    /// to be possibly stuck waiting in case there is a problem; this
    /// limit is deactivated by default.
    ///
    /// @param timeout Maximum time to wait
    ///
    /// @return Public IP address of the computer
    ///
    /// \see getLocalAddress
    ///
    ////////////////////////////////////////////////////////////
    static IPAddress getPublicAddress(Time timeout = Time::Zero);

    ////////////////////////////////////////////////////////////
    // Static member data
    ////////////////////////////////////////////////////////////
    static const IPAddress None;      ///< Value representing an empty/invalid address
    static const IPAddress Any;       ///< Value representing any address (0.0.0.0)
    static const IPAddress LocalHost; ///< The "localhost" address (for connecting a computer to itself locally)
    static const IPAddress Broadcast; ///< The "broadcast" address (for sending UDP messages to everyone on a local network)

private:

    friend SFML_NETWORK_API bool operator <(const IPAddress& left, const IPAddress& right);

    ////////////////////////////////////////////////////////////
    /// \brief Resolve the given address string
    ///
    /// @param address Address string
    ///
    ////////////////////////////////////////////////////////////
    void resolve(const std::string& address);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    uint32_t m_address; ///< Address stored as an unsigned 32 bits integer
    bool   m_valid;   ///< Is the address valid?
};
/// \brief Overload of == operator to compare two IP addresses
///
/// @param left  Left operand (a IP address)
/// @param right Right operand (a IP address)
///
/// @return True if both addresses are equal
///
////////////////////////////////////////////////////////////
SFML_NETWORK_API bool operator ==(const IPAddress& left, const IPAddress& right);
/// \brief Overload of != operator to compare two IP addresses
///
/// @param left  Left operand (a IP address)
/// @param right Right operand (a IP address)
///
/// @return True if both addresses are different
///
////////////////////////////////////////////////////////////
SFML_NETWORK_API bool operator !=(const IPAddress& left, const IPAddress& right);
/// \brief Overload of < operator to compare two IP addresses
///
/// @param left  Left operand (a IP address)
/// @param right Right operand (a IP address)
///
/// @return True if \a left is lesser than \a right
///
////////////////////////////////////////////////////////////
SFML_NETWORK_API bool operator <(const IPAddress& left, const IPAddress& right);
/// \brief Overload of > operator to compare two IP addresses
///
/// @param left  Left operand (a IP address)
/// @param right Right operand (a IP address)
///
/// @return True if \a left is greater than \a right
///
////////////////////////////////////////////////////////////
SFML_NETWORK_API bool operator >(const IPAddress& left, const IPAddress& right);
/// \brief Overload of <= operator to compare two IP addresses
///
/// @param left  Left operand (a IP address)
/// @param right Right operand (a IP address)
///
/// @return True if \a left is lesser or equal than \a right
///
////////////////////////////////////////////////////////////
SFML_NETWORK_API bool operator <=(const IPAddress& left, const IPAddress& right);
/// \brief Overload of >= operator to compare two IP addresses
///
/// @param left  Left operand (a IP address)
/// @param right Right operand (a IP address)
///
/// @return True if \a left is greater or equal than \a right
///
////////////////////////////////////////////////////////////
SFML_NETWORK_API bool operator >=(const IPAddress& left, const IPAddress& right);
/// \brief Overload of >> operator to extract an IP address from an input stream
///
/// @param stream  Input stream
/// @param address IP address to extract
///
/// @return Reference to the input stream
///
////////////////////////////////////////////////////////////
SFML_NETWORK_API std::istream& operator >>(std::istream& stream, IPAddress& address);
/// \brief Overload of << operator to print an IP address to an output stream
///
/// @param stream  Output stream
/// @param address IP address to print
///
/// @return Reference to the output stream
///
////////////////////////////////////////////////////////////
SFML_NETWORK_API std::ostream& operator <<(std::ostream& stream, const IPAddress& address);

} // namespace _Net


#endif // SFML_IPADDRESS_HPP

/// \class sf::IPAddress
/// \ingroup network
///
/// sf::IPAddress is a utility class for manipulating network
/// addresses. It provides a set a implicit constructors and
/// conversion functions to easily build or transform an IP
/// address from/to various representations.
///
/// Usage example:
/// \code
/// sf::IPAddress a0;                                     // an invalid address
/// sf::IPAddress a1 = sf::IPAddress::None;               // an invalid address (same as a0)
/// sf::IPAddress a2("127.0.0.1");                        // the local host address
/// sf::IPAddress a3 = sf::IPAddress::Broadcast;          // the broadcast address
/// sf::IPAddress a4(192, 168, 1, 56);                    // a local address
/// sf::IPAddress a5("my_computer");                      // a local address created from a network name
/// sf::IPAddress a6("89.54.1.169");                      // a distant address
/// sf::IPAddress a7("www.google.com");                   // a distant address created from a network name
/// sf::IPAddress a8 = sf::IPAddress::getLocalAddress();  // my address on the local network
/// sf::IPAddress a9 = sf::IPAddress::getPublicAddress(); // my address on the internet
/// \endcode
///
/// Note that sf::IPAddress currently doesn't support IPv6
/// nor other types of network addresses.
///
////////////////////////////////////////////////////////////
