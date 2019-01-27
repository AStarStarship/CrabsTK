/** Kabuki Toolkit
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

class TextOperation :public _::Text<1024>, public Operand {
    public:

    TextOperation () {

    }

    /** Handles Script Commands.
        @param text     Beginning of the Text buffer. 
        @param strand_end End of the Text buffer.
        @return Returns nil upon success and an error string upon failure. */
    virtual const CH1* Sudo (const CH1* text,
                                    const CH1* strand_end) {
        return nullptr
    }

    /** Script Operations. */
    virtual const Operation* Star (uint index, _::Expression* expr) {
        return OperationInvalid;
    }
};

class CardsClient: public Client {
    public:

    typedef enum States {
        kStateDisconnected       = 0,
        kStateAwaitingConnection = 1,
    } State;

    enum {
        kMaxUsernameLength = 64,
        kMaxPasswordLength = 32,
        kBinSizeBytes      = 2048,
        kBinBinSizeWords   = kBinSizeBytes / sizeof (uintptr_t),
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
            BOL write_in_progress = !text_out_queue_.empty ();
            text_out_queue_.push_back (message);
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
                                    [this] (boost::system::error_code error_code,
                                    tcp::resolver::iterator) {
            if (!error_code) {
                ReadHeader ();
            }
        });
    }

    //void ReadHeader () {
    //    boost::asio::async_read (socket_,
    //                             boost::asio::buffer (read_msg_.Data (),
    //                             std::string::kHeaderLength),
    //                             [this] (boost::system::error_code error_code,
    //                                     std::size_t /*length*/) {
    //        if (!error_code && read_msg_.DecodeHeader ()) {
    //            ReadOperation ();
    //        } else {
    //            socket_.close ();
    //        }
    //    });
    //}

    void ReadOperation () {
        boost::asio::async_read (socket_,
                                 boost::asio::buffer (bin_buffer, kBinSizeBytes,
                                 [this] (boost::system::error_code error_code,
                                         std::size_t /*length*/) {
            if (!error_code) {
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
                                  boost::asio::buffer (text_out_queue_.front ().Data (),
                                  text_out_queue_.front ().Length ()),
                                  [this] (boost::system::error_code error_code,
                                  std::size_t /*length*/) {
            if (!error_code) {
                text_out_queue_.pop_front ();
                if (!text_out_queue_.empty ()) {
                    WriteToSocket ();
                }
            } else {
                socket_.close ();
            }
        });
    }

    private:

    uintptr_t bin_buffer[2048 / sizeof (uintptr_t)];
    boost::asio::io_service& io_service_;
    tcp::socket socket_;
    text_operation_queue text_out_queue_;
};

SI4 main (SI4 argc, CH1* argv[]) {

    enum {
        kKeyboardBufferSize = 80,
    };
    
    CH1 input[kKeyboardBufferSize];

    if (argc != 3) {
        std::cerr << "Usage: cards <host> <port>\n";
        return 1;
    }

    Client client;
    cout << 
    KeyboardText ("\n| Enter your handle:", input,
                  kKeyboardBufferSize);
    try {

        boost::asio::io_service io_service;

        tcp::resolver resolver (io_service);
        auto endpoint_iterator = resolver.resolve ({ argv[1], argv[2] });
        CardsClient cards_client (io_service, endpoint_iterator);

        std::thread client_thread ([&io_service] () { io_service.run (); });

        while (std::cin.getline (input, kKeyboardBufferSize)) {

           Text<23> text;
            text.SetBodyLength (TextLength (input, input + kKeyboardBufferSize));

            std::memcpy (text.Body (), input, text.BodyLength ());
            text.EncodeHeader ();
            cards_client.Write (text);
        }

        cards_client.Close ();
        client_thread.join ();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what () << "\n";
    }

    return 0;
}
