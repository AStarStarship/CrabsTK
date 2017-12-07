/** kabuki::cards
    @file    ~/source/kabuki/id/user_hist.h
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

typedef std::deque<ChatMessage> chat_message_queue;

class CardsClient: public Room {
    public:

    enum {
        kMaxUsernameLength = 64,
        kMaxPasswordLength = 32
    };

    CardsClient (boost::asio::io_service& io_service,
                 tcp::resolver::iterator endpoint_iterator) :
        Room         ("Kabuki Cards"),
        io_service_ (io_service),
        socket_     (io_service) {
        Connect     (endpoint_iterator);
    }

    void Write (const ChatMessage& msg) {
        io_service_.post (
            [this, msg] () {
            bool write_in_progress = !write_msgs_.empty ();
            write_msgs_.push_back (msg);
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
                                 ChatMessage::kHeaderLength),
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

    /** Script operations. */
    virtual const Operation* Star (uint index, Expression* expr) {
        static const Operation This = { "kabuki::cards::client",
            NumOperations (3), FirstOperation ('A'),
            "kabuki::script and kabuki::cards Client demo.", 0 };

        void* args[2];

        switch (index) {
            case '?': return &This;
            case 'A': {
                static const Operation OpA = { "Connect",
                    Params<1, UI8> (), Params <0> (),
                    "Adds a player to the game.", 0
                };
                if (!expr) return &OpA;

                if (state_ != kStateAwaitingConnection) {

                }

                uid_t player_uid;
                if (ExprArgs (expr, Params<1, UI8> (), Args (args, &player_uid))) {
                    return expr->result;
                }
                User* user;
                players_.Push (new BlackjackPlayer ());

                return nullptr;
            }
            case 'B': {
                static const Operation OpA = { "Ping",
                    Params<0> (), Params<0> (),
                    "Pings the client to see if it's still connected.", 0 };
                if (!expr) return &OpA;

                return ExprResult (expr, Params<1, ADR> (), Args (args, "A"));
            }
            case 'B': {
                static const Operation OpA = { "Print",
                    Params<1, STX> (), Params<0> (),
                    "Attempts to login in the user with #username and "
                    "#password.", 0 };
                if (!expr) return &OpA;

                return ExprResult (expr, Params<1, ADR> (), Args (args, "A"));
            }
            case 'C': {
            }
        }
        return nullptr;
    }

    private:

    char username_[kMaxUsernameLength],
        password_[kMaxPasswordLength];

    boost::asio::io_service& io_service_;
    tcp::socket socket_;
    ChatMessage read_msg_;
    chat_message_queue write_msgs_;
};

int main (int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: cards <host> <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        tcp::resolver resolver (io_service);
        auto endpoint_iterator = resolver.resolve ({ argv[1], argv[2] });
        CardsClient c (io_service, endpoint_iterator);

        std::thread t ([&io_service] () { io_service.run (); });

        char line[ChatMessage::kMaxBodyLength + 1];
        while (std::cin.getline (line, ChatMessage::kMaxBodyLength + 1)) {
            ChatMessage msg;
            msg.BodyLength (std::strlen (line));
            std::memcpy (msg.Body (), line, msg.BodyLength ());
            msg.EncodeHeader ();
            c.Write (msg);
        }

        c.Close ();
        t.join ();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what () << "\n";
    }

    return 0;
}
