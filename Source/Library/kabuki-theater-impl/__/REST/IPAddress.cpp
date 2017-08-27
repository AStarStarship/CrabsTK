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
// Headers
////////////////////////////////////////////////////////////
#include <_Net/IPAddress.hpp>
#include <_Net/HTTP.hpp>
#include <_Net/SocketImpl.hpp>
#include <cstring>
#include <utility>


namespace _Net
{
////////////////////////////////////////////////////////////
const IPAddress IPAddress::None;
const IPAddress IPAddress::Any(0, 0, 0, 0);
const IPAddress IPAddress::LocalHost(127, 0, 0, 1);
const IPAddress IPAddress::Broadcast(255, 255, 255, 255);

IPAddress::IPAddress() :
m_address(0),
m_valid  (false)
{
}

IPAddress::IPAddress(const std::string& address) :
m_address(0),
m_valid  (false)
{
    resolve(address);
}

IPAddress::IPAddress(const char* address) :
m_address(0),
m_valid  (false)
{
    resolve(address);
}

IPAddress::IPAddress(Uint8 byte0, Uint8 byte1, Uint8 byte2, Uint8 byte3) :
m_address(htonl((byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3)),
m_valid  (true)
{
}

IPAddress::IPAddress(uint32_t address) :
m_address(htonl(address)),
m_valid  (true)
{
}

std::string IPAddress::toString() const
{
    in_addr address;
    address.s_addr = m_address;

    return inet_ntoa(address);
}

uint32_t IPAddress::toInteger() const
{
    return ntohl(m_address);
}

IPAddress IPAddress::getLocalAddress()
{
    // The method here is to connect a UDP socket to anyone (here to localhost),
    // and get the local socket address with the getsockname function.
    // UDP connection will not send anything to the network, so this function won't cause any overhead.

    IPAddress localAddress;

    // Create the socket
    SocketHandle sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == priv::SocketImpl::invalidSocket())
        return localAddress;

    // Connect the socket to localhost on any port
    sockaddr_in address = priv::SocketImpl::createAddress(ntohl(INADDR_LOOPBACK), 9);
    if (connect(sock, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1)
    {
        priv::SocketImpl::close(sock);
        return localAddress;
    }

    // Get the local address of the socket connection
    priv::SocketImpl::AddrLength size = sizeof(address);
    if (getsockname(sock, reinterpret_cast<sockaddr*>(&address), &size) == -1)
    {
        priv::SocketImpl::close(sock);
        return localAddress;
    }

    // Close the socket
    priv::SocketImpl::close(sock);

    // Finally build the IP address
    localAddress = IPAddress(ntohl(address.sin_addr.s_addr));

    return localAddress;
}

IPAddress IPAddress::getPublicAddress(Time timeout)
{
    // The trick here is more complicated, because the only way
    // to get our public IP address is to get it from a distant computer.
    // Here we get the web page from http://www.sfml-dev.org/ip-provider.php
    // and parse the result to extract our IP address
    // (not very hard: the web page contains only our IP address).

    HTTP server("www.sfml-dev.org");
    HTTP::Request request("/ip-provider.php", HTTP::Request::Get);
    HTTP::Response page = server.sendRequest(request, timeout);
    if (page.getStatus() == HTTP::Response::Ok)
        return IPAddress(page.getBody());

    // Something failed: return an invalid address
    return IPAddress();
}

void IPAddress::resolve(const std::string& address)
{
    m_address = 0;
    m_valid = false;

    if (address == "255.255.255.255")
    {
        // The broadcast address needs to be handled explicitly,
        // because it is also the value returned by inet_addr on error
        m_address = INADDR_BROADCAST;
        m_valid = true;
    }
    else if (address == "0.0.0.0")
    {
        m_address = INADDR_ANY;
        m_valid = true;
    }
    else
    {
        // Try to convert the address as a byte representation ("xxx.xxx.xxx.xxx")
        uint32_t ip = inet_addr(address.c_str());
        if (ip != INADDR_NONE)
        {
            m_address = ip;
            m_valid = true;
        }
        else
        {
            // Not a valid address, try to convert it as a host name
            addrinfo hints;
            std::memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_INET;
            addrinfo* result = NULL;
            if (getaddrinfo(address.c_str(), NULL, &hints, &result) == 0)
            {
                if (result)
                {
                    ip = reinterpret_cast<sockaddr_in*>(result->ai_addr)->sin_addr.s_addr;
                    freeaddrinfo(result);
                    m_address = ip;
                    m_valid = true;
                }
            }
        }
    }
}

bool operator ==(const IPAddress& left, const IPAddress& right)
{
    return !(left < right) && !(right < left);
}

bool operator !=(const IPAddress& left, const IPAddress& right)
{
    return !(left == right);
}

bool operator <(const IPAddress& left, const IPAddress& right)
{
    return std::make_pair(left.m_valid, left.m_address) < std::make_pair(right.m_valid, right.m_address);
}

bool operator >(const IPAddress& left, const IPAddress& right)
{
    return right < left;
}

bool operator <=(const IPAddress& left, const IPAddress& right)
{
    return !(right < left);
}

bool operator >=(const IPAddress& left, const IPAddress& right)
{
    return !(left < right);
}

std::istream& operator >>(std::istream& stream, IPAddress& address)
{
    std::string str;
    stream >> str;
    address = IPAddress(str);

    return stream;
}

std::ostream& operator <<(std::ostream& stream, const IPAddress& address)
{
    return stream << address.toString();
}

} // namespace _Net
