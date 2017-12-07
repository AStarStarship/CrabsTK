/** kabuki::cards
    @file    ~/projects/kabuki_cards_client.cc
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
 

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "../../source/kabuki/cards/global.h"

using namespace _;
using namespace kabuki::data;
using namespace kabuki::cards;
using boost::asio::ip::tcp;

class CardsClient: public Client {
    public:

    typedef enum States {
        kStateDisconnected       = 0,
        kStateAwaitingConnection = 1,
    } State;

    enum {
        kMaxUsernameLength = 64,
        kMaxPasswordLength = 32,
    };

    CardsClient (boost::asio::io_service& io_service,
                 tcp::resolver::iterator endpoint_iterator) :
        Client      (),
        io_service_ (io_service),
        socket_     (io_service) {
        Connect     (endpoint_iterator);
    }

    void Write (const std::string& message) {
        io_service_.post (
            [this, message] () {
            bool write_in_progress = !write_msgs_.empty ();
            write_msgs_.push_back (message);
            if (!write_in_progress) {
                WriteToSocket ();
            }
        });
    }

    void Close () {
        io_service_.post ([this] () { socket_.close (); });
    }

    private:

    void Connect (tcp::resolver::iterator endpoint_iterator) {
        boost::asio::async_connect (socket_, endpoint_iterator,
                                    [this] (boost::system::error_code ec,
                                    tcp::resolver::iterator) {
            if (!ec) {
                ReadHeader ();
            }
        });
    }

    void ReadHeader () {
        boost::asio::async_read (socket_,
                                 boost::asio::buffer (read_msg_.Data (),
                                 std::string::kHeaderLength),
                                 [this] (boost::system::error_code ec,
                                         std::size_t /*length*/) {
            if (!ec && read_msg_.DecodeHeader ()) {
                ReadBody ();
            } else {
                socket_.close ();
            }
        });
    }

    void ReadBody () {
        boost::asio::async_read (socket_,
                                 boost::asio::buffer (read_msg_.Body (),
                                 read_msg_.BodyLength ()),
                                 [this] (boost::system::error_code ec,
                                         std::size_t /*length*/) {
            if (!ec) {
                std::cout.write (read_msg_.Body (), read_msg_.BodyLength ());
                std::cout << "\n";
                ReadHeader ();
            } else {
                socket_.close ();
            }
        });
    }

    void WriteToSocket () {
        boost::asio::async_write (socket_,
                                  boost::asio::buffer (write_msgs_.front ().Data (),
                                  write_msgs_.front ().Length ()),
                                  [this] (boost::system::error_code ec,
                                  std::size_t /*length*/) {
            if (!ec) {
                write_msgs_.pop_front ();
                if (!write_msgs_.empty ()) {
                    WriteToSocket ();
                }
            } else {
                socket_.close ();
            }
        });
    }

    private:

    boost::asio::io_service& io_service_;
    tcp::socket socket_;
    chat_message_queue write_msgs_;
};

int main (int argc, char* argv[]) {

    enum {
        kKeyboardBufferSize = 80,
    };
    
    char keyboard_buffer[kKeyboardBufferSize];

    if (argc != 3) {
        std::cerr << "Usage: cards <host> <port>\n";
        return 1;
    }

    ::_::KeyboardString ("\n| Welcome to Kabuki Cards Console."
                    "\n|"
                    "\n| Enter your handle:", keyboard_buffer,
                    kKeyboardBufferSize);
    try {

        boost::asio::io_service io_service;

        tcp::resolver resolver (io_service);
        auto endpoint_iterator = resolver.resolve ({ argv[1], argv[2] });
        CardsClient cards_client (io_service, endpoint_iterator);

        std::thread client_thread ([&io_service] () { io_service.run (); });

        char line[std::string::kMaxBodyLength + 1];
        while (std::cin.getline (line, std::string::kMaxBodyLength + 1)) {
            std::string message;
            message.BodyLength (std::strlen (line));
            std::memcpy (message.Body (), line, message.BodyLength ());
            message.EncodeHeader ();
            cards_client.Write (message);
        }

        cards_client.Close ();
        client_thread.join ();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what () << "\n";
    }

    return 0;
}
