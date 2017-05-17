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
#include <_Net/FTP.hpp>
#include <_Net/IPAddress.hpp>
#include <_/Err.hpp>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iterator>
#include <sstream>
#include <cstdio>


namespace _Net
{
////////////////////////////////////////////////////////////
class FTP::DataChannel : NonCopyable
{
public:

    ////////////////////////////////////////////////////////////
    DataChannel(FTP& owner);

    ////////////////////////////////////////////////////////////
    FTP::Response open(FTP::TransferMode mode);

    ////////////////////////////////////////////////////////////
    void send(std::istream& stream);

    ////////////////////////////////////////////////////////////
    void receive(std::ostream& stream);

private:

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    FTP&      m_ftp;        ///< Reference to the owner FTP instance
    TCPSocket m_dataSocket; ///< Socket used for data transfers
};

FTP::Response::Response(Status code, const std::string& message) :
m_status (code),
m_message(message)
{

}

bool FTP::Response::isOk() const
{
    return m_status < 400;
}

FTP::Response::Status FTP::Response::getStatus() const
{
    return m_status;
}

const std::string& FTP::Response::getMessage() const
{
    return m_message;
}

FTP::DirectoryResponse::DirectoryResponse(const FTP::Response& response) :
FTP::Response(response)
{
    if (isOk())
    {
        // Extract the directory from the server response
        std::string::size_type begin = getMessage().find('"', 0);
        std::string::size_type end   = getMessage().find('"', begin + 1);
        m_directory = getMessage().substr(begin + 1, end - begin - 1);
    }
}

const std::string& FTP::DirectoryResponse::getDirectory() const
{
    return m_directory;
}

FTP::ListingResponse::ListingResponse(const FTP::Response& response, const std::string& data) :
FTP::Response(response)
{
    if (isOk())
    {
        // Fill the array of strings
        std::string::size_type lastPos = 0;
        for (std::string::size_type pos = data.find("\r\n"); pos != std::string::npos; pos = data.find("\r\n", lastPos))
        {
            m_listing.push_back(data.substr(lastPos, pos - lastPos));
            lastPos = pos + 2;
        }
    }
}

const std::vector<std::string>& FTP::ListingResponse::getListing() const
{
    return m_listing;
}

FTP::~FTP()
{
    disconnect();
}

FTP::Response FTP::connect(const IPAddress& server, uint16_t port, Time timeout)
{
    // Connect to the server
    if (m_commandSocket.connect(server, port, timeout) != Socket::Done)
        return Response(Response::ConnectionFailed);

    // Get the response to the connection
    return getResponse();
}

FTP::Response FTP::login()
{
    return login("anonymous", "user@sfml-dev.org");
}

FTP::Response FTP::login(const std::string& name, const std::string& password)
{
    Response response = sendCommand("USER", name);
    if (response.isOk())
        response = sendCommand("PASS", password);

    return response;
}

FTP::Response FTP::disconnect()
{
    // Send the exit command
    Response response = sendCommand("QUIT");
    if (response.isOk())
        m_commandSocket.disconnect();

    return response;
}

FTP::Response FTP::keepAlive()
{
    return sendCommand("NOOP");
}

FTP::DirectoryResponse FTP::getWorkingDirectory()
{
    return DirectoryResponse(sendCommand("PWD"));
}

FTP::ListingResponse FTP::getDirectoryListing(const std::string& directory)
{
    // Open a data channel on default port (20) using ASCII transfer mode
    std::ostringstream directoryData;
    DataChannel data(*this);
    Response response = data.open(Ascii);
    if (response.isOk())
    {
        // Tell the server to send us the listing
        response = sendCommand("NLST", directory);
        if (response.isOk())
        {
            // Receive the listing
            data.receive(directoryData);

            // Get the response from the server
            response = getResponse();
        }
    }

    return ListingResponse(response, directoryData.str());
}

FTP::Response FTP::changeDirectory(const std::string& directory)
{
    return sendCommand("CWD", directory);
}

FTP::Response FTP::parentDirectory()
{
    return sendCommand("CDUP");
}

FTP::Response FTP::createDirectory(const std::string& name)
{
    return sendCommand("MKD", name);
}

FTP::Response FTP::deleteDirectory(const std::string& name)
{
    return sendCommand("RMD", name);
}

FTP::Response FTP::renameFile(const std::string& file, const std::string& newName)
{
    Response response = sendCommand("RNFR", file);
    if (response.isOk())
       response = sendCommand("RNTO", newName);

    return response;
}

FTP::Response FTP::deleteFile(const std::string& name)
{
    return sendCommand("DELE", name);
}

FTP::Response FTP::download(const std::string& remoteFile, const std::string& localPath, TransferMode mode)
{
    // Open a data channel using the given transfer mode
    DataChannel data(*this);
    Response response = data.open(mode);
    if (response.isOk())
    {
        // Tell the server to start the transfer
        response = sendCommand("RETR", remoteFile);
        if (response.isOk())
        {
            // Extract the filename from the file path
            std::string filename = remoteFile;
            std::string::size_type pos = filename.find_last_of("/\\");
            if (pos != std::string::npos)
                filename = filename.substr(pos + 1);

            // Make sure the destination path ends with a slash
            std::string path = localPath;
            if (!path.empty() && (path[path.size() - 1] != '\\') && (path[path.size() - 1] != '/'))
                path += "/";

            // Create the file and truncate it if necessary
            std::ofstream file((path + filename).c_str(), std::ios_base::binary | std::ios_base::trunc);
            if (!file)
                return Response(Response::InvalidFile);

            // Receive the file data
            data.receive(file);

            // Close the file
            file.close();

            // Get the response from the server
            response = getResponse();

            // If the download was unsuccessful, delete the partial file
            if (!response.isOk())
                std::remove((path + filename).c_str());
        }
    }

    return response;
}

FTP::Response FTP::upload(const std::string& localFile, const std::string& remotePath, TransferMode mode)
{
    // Get the contents of the file to send
    std::ifstream file(localFile.c_str(), std::ios_base::binary);
    if (!file)
        return Response(Response::InvalidFile);

    // Extract the filename from the file path
    std::string filename = localFile;
    std::string::size_type pos = filename.find_last_of("/\\");
    if (pos != std::string::npos)
        filename = filename.substr(pos + 1);

    // Make sure the destination path ends with a slash
    std::string path = remotePath;
    if (!path.empty() && (path[path.size() - 1] != '\\') && (path[path.size() - 1] != '/'))
        path += "/";

    // Open a data channel using the given transfer mode
    DataChannel data(*this);
    Response response = data.open(mode);
    if (response.isOk())
    {
        // Tell the server to start the transfer
        response = sendCommand("STOR", path + filename);
        if (response.isOk())
        {
            // Send the file data
            data.send(file);

            // Get the response from the server
            response = getResponse();
        }
    }

    return response;
}

FTP::Response FTP::sendCommand(const std::string& command, const std::string& parameter)
{
    // Build the command string
    std::string commandStr;
    if (parameter != "")
        commandStr = command + " " + parameter + "\r\n";
    else
        commandStr = command + "\r\n";

    // Send it to the server
    if (m_commandSocket.send(commandStr.c_str(), commandStr.length()) != Socket::Done)
        return Response(Response::ConnectionClosed);

    // Get the response
    return getResponse();
}

FTP::Response FTP::getResponse()
{
    // We'll use a variable to keep track of the last valid code.
    // It is useful in case of multi-lines responses, because the end of such a response
    // will start by the same code
    uint lastCode  = 0;
    bool isInsideMultiline = false;
    std::string message;

    for (;;)
    {
        // Receive the response from the server
        char buffer[1024];
        std::size_t length;

        if (m_receiveBuffer.empty())
        {
            if (m_commandSocket.receive(buffer, sizeof(buffer), length) != Socket::Done)
                return Response(Response::ConnectionClosed);
        }
        else
        {
            std::copy(m_receiveBuffer.begin(), m_receiveBuffer.end(), buffer);
            length = m_receiveBuffer.size();
            m_receiveBuffer.clear();
        }

        // There can be several lines inside the received buffer, extract them all
        std::istringstream in(std::string(buffer, length), std::ios_base::binary);
        while (in)
        {
            // Try to extract the code
            uint code;
            if (in >> code)
            {
                // Extract the separator
                char separator;
                in.get(separator);

                // The '-' character means a multiline response
                if ((separator == '-') && !isInsideMultiline)
                {
                    // Set the multiline flag
                    isInsideMultiline = true;

                    // Keep track of the code
                    if (lastCode == 0)
                        lastCode = code;

                    // Extract the line
                    std::getline(in, message);

                    // Remove the ending '\r' (all lines are terminated by "\r\n")
                    message.erase(message.length() - 1);
                    message = separator + message + "\n";
                }
                else
                {
                    // We must make sure that the code is the same, otherwise it means
                    // we haven't reached the end of the multiline response
                    if ((separator != '-') && ((code == lastCode) || (lastCode == 0)))
                    {
                        // Extract the line
                        std::string line;
                        std::getline(in, line);

                        // Remove the ending '\r' (all lines are terminated by "\r\n")
                        line.erase(line.length() - 1);

                        // Append it to the message
                        if (code == lastCode)
                        {
                            std::ostringstream out;
                            out << code << separator << line;
                            message += out.str();
                        }
                        else
                        {
                            message = separator + line;
                        }

                        // Save the remaining data for the next time getResponse() is called
                        m_receiveBuffer.assign(buffer + in.tellg(), length - in.tellg());

                        // Return the response code and message
                        return Response(static_cast<Response::Status>(code), message);
                    }
                    else
                    {
                        // The line we just read was actually not a response,
                        // only a new part of the current multiline response

                        // Extract the line
                        std::string line;
                        std::getline(in, line);

                        if (!line.empty())
                        {
                            // Remove the ending '\r' (all lines are terminated by "\r\n")
                            line.erase(line.length() - 1);

                            // Append it to the current message
                            std::ostringstream out;
                            out << code << separator << line << "\n";
                            message += out.str();
                        }
                    }
                }
            }
            else if (lastCode != 0)
            {
                // It seems we are in the middle of a multiline response

                // Clear the error bits of the stream
                in.clear();

                // Extract the line
                std::string line;
                std::getline(in, line);

                if (!line.empty())
                {
                    // Remove the ending '\r' (all lines are terminated by "\r\n")
                    line.erase(line.length() - 1);

                    // Append it to the current message
                    message += line + "\n";
                }
            }
            else
            {
                // Error: cannot extract the code, and we are not in a multiline response
                return Response(Response::InvalidResponse);
            }
        }
    }

    // We never reach there
}

FTP::DataChannel::DataChannel(FTP& owner) :
m_ftp(owner)
{

}

FTP::Response FTP::DataChannel::open(FTP::TransferMode mode)
{
    // Open a data connection in active mode (we connect to the server)
    FTP::Response response = m_ftp.sendCommand("PASV");
    if (response.isOk())
    {
        // Extract the connection address and port from the response
        std::string::size_type begin = response.getMessage().find_first_of("0123456789");
        if (begin != std::string::npos)
        {
            Uint8 data[6] = {0, 0, 0, 0, 0, 0};
            std::string str = response.getMessage().substr(begin);
            std::size_t index = 0;
            for (int i = 0; i < 6; ++i)
            {
                // Extract the current number
                while (isdigit(str[index]))
                {
                    data[i] = data[i] * 10 + (str[index] - '0');
                    index++;
                }

                // Skip separator
                index++;
            }

            // Reconstruct connection port and address
            uint16_t port = data[4] * 256 + data[5];
            IPAddress address(static_cast<Uint8>(data[0]),
                              static_cast<Uint8>(data[1]),
                              static_cast<Uint8>(data[2]),
                              static_cast<Uint8>(data[3]));

            // Connect the data channel to the server
            if (m_dataSocket.connect(address, port) == Socket::Done)
            {
                // Translate the transfer mode to the corresponding FTP parameter
                std::string modeStr;
                switch (mode)
                {
                    case FTP::Binary: modeStr = "I"; break;
                    case FTP::Ascii:  modeStr = "A"; break;
                    case FTP::Ebcdic: modeStr = "E"; break;
                }

                // Set the transfer mode
                response = m_ftp.sendCommand("TYPE", modeStr);
            }
            else
            {
                // Failed to connect to the server
                response = FTP::Response(FTP::Response::ConnectionFailed);
            }
        }
    }

    return response;
}

void FTP::DataChannel::receive(std::ostream& stream)
{
    // Receive data
    char buffer[1024];
    std::size_t received;
    while (m_dataSocket.receive(buffer, sizeof(buffer), received) == Socket::Done)
    {
        stream.write(buffer, static_cast<std::streamsize>(received));

        if (!stream.good())
        {
            err() << "FTP Error: Writing to the file has failed" << std::endl;
            break;
        }
    }

    // Close the data socket
    m_dataSocket.disconnect();
}

void FTP::DataChannel::send(std::istream& stream)
{
    // Send data
    char buffer[1024];
    std::size_t count;

    for (;;)
    {
        // read some data from the stream
        stream.read(buffer, sizeof(buffer));

        if (!stream.good() && !stream.eof())
        {
            err() << "FTP Error: Reading from the file has failed" << std::endl;
            break;
        }

        count = stream.gcount();

        if (count > 0)
        {
            // we could read more data from the stream: send them
            if (m_dataSocket.send(buffer, count) != Socket::Done)
                break;
        }
        else
        {
            // no more data: exit the loop
            break;
        }
    }

    // Close the data socket
    m_dataSocket.disconnect();
}

} // namespace _Net
