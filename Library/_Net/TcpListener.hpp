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

#ifndef SFML_TCPLISTENER_HPP
#define SFML_TCPLISTENER_HPP
// Headers
////////////////////////////////////////////////////////////
#include <_Net/Export.hpp>
#include <_Net/Socket.hpp>
#include <_Net/IPAddress.hpp>


namespace _Net
{
class TCPSocket;
/// \brief Socket that listens to new TCP connections
///
////////////////////////////////////////////////////////////
class SFML_NETWORK_API TCPListener : public Socket
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    TCPListener();

    ////////////////////////////////////////////////////////////
    /// \brief Get the port to which the socket is bound locally
    ///
    /// If the socket is not listening to a port, this function
    /// returns 0.
    ///
    /// @return Port to which the socket is bound
    ///
    /// \see listen
    ///
    ////////////////////////////////////////////////////////////
    uint16_t getLocalPort() const;

    ////////////////////////////////////////////////////////////
    /// \brief Start listening for connections
    ///
    /// This functions makes the socket listen to the specified
    /// port, waiting for new connections.
    /// If the socket was previously listening to another port,
    /// it will be stopped first and bound to the new port.
    ///
    /// @param port    Port to listen for new connections
    /// @param address Address of the interface to listen on
    ///
    /// @return Status code
    ///
    /// \see accept, close
    ///
    ////////////////////////////////////////////////////////////
    Status listen(uint16_t port, const IPAddress& address = IPAddress::Any);

    ////////////////////////////////////////////////////////////
    /// \brief Stop listening and close the socket
    ///
    /// This function gracefully stops the listener. If the
    /// socket is not listening, this function has no effect.
    ///
    /// \see listen
    ///
    ////////////////////////////////////////////////////////////
    void close();

    ////////////////////////////////////////////////////////////
    /// \brief Accept a new connection
    ///
    /// If the socket is in blocking mode, this function will
    /// not return until a connection is actually received.
    ///
    /// @param socket Socket that will hold the new connection
    ///
    /// @return Status code
    ///
    /// \see listen
    ///
    ////////////////////////////////////////////////////////////
    Status accept(TCPSocket& socket);
};


} // namespace _Net


#endif // SFML_TCPLISTENER_HPP

/// \class sf::TCPListener
/// \ingroup network
///
/// A listener socket is a special type of socket that listens to
/// a given port and waits for connections on that port.
/// This is all it can do.
///
/// When a new connection is received, you must call accept and
/// the listener returns a new instance of sf::TCPSocket that
/// is properly initialized and can be used to communicate with
/// the new client.
///
/// Listener sockets are specific to the TCP protocol,
/// UDP sockets are connectionless and can therefore communicate
/// directly. As a consequence, a listener socket will always
/// return the new connections as sf::TCPSocket instances.
///
/// A listener is automatically closed on destruction, like all
/// other types of socket. However if you want to stop listening
/// before the socket is destroyed, you can call its close()
/// function.
///
/// Usage example:
/// \code
/// // Create a listener socket and make it wait for new
/// // connections on port 55001
/// sf::TCPListener listener;
/// listener.listen(55001);
///
/// // Endless loop that waits for new connections
/// while (running)
/// {
///     sf::TCPSocket client;
///     if (listener.accept(client) == sf::Socket::Done)
///     {
///         // A new client just connected!
///         std::cout << "New connection received from " << client.getRemoteAddress() << std::endl;
///         doSomethingWith(client);
///     }
/// }
/// \endcode
///
/// \see sf::TCPSocket, sf::Socket
///
////////////////////////////////////////////////////////////
