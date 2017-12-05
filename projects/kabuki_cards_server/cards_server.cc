//
// chat_server.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>
#include "../../source/kabuki/cards/global.h"

using boost::asio::ip::tcp;

using namespace _;
using namespace kabuki::cards;


typedef std::deque<ChatMessage> chat_message_queue;

class chat_participant {
    public:
    virtual ~chat_participant () {}
    virtual void deliver (const ChatMessage& msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

class chat_room {
    public:
    void join (chat_participant_ptr participant) {
        participants_.insert (participant);
        for (auto msg : recent_msgs_)
            participant->deliver (msg);
    }

    void leave (chat_participant_ptr participant) {
        participants_.erase (participant);
    }

    void deliver (const ChatMessage& msg) {
        recent_msgs_.push_back (msg);
        while (recent_msgs_.size () > max_recent_msgs)
            recent_msgs_.pop_front ();

        for (auto participant : participants_)
            participant->deliver (msg);
    }

    private:
    std::set<chat_participant_ptr> participants_;
    enum { max_recent_msgs = 100 };
    chat_message_queue recent_msgs_;
};

class chat_session
    : public chat_participant,
    public std::enable_shared_from_this<chat_session> {
    public:
    chat_session (tcp::socket socket, chat_room& room) :
        socket_ (std::move (socket)), 
        room_ (room) {
    }

    void start () {
        room_.join (shared_from_this ());
        do_read_header ();
    }

    void deliver (const ChatMessage& msg) {
        bool write_in_progress = !write_msgs_.empty ();
        write_msgs_.push_back (msg);
        if (!write_in_progress) {
            do_write ();
        }
    }

    private:

    void do_read_header () {
        auto self (shared_from_this ());
        boost::asio::async_read (socket_,
                                 boost::asio::buffer (read_msg_.Data (), ChatMessage::kHeaderLength),
                                 [this, self] (boost::system::error_code ec,
                                               std::size_t /*length*/) {
            if (!ec && read_msg_.DecodeHeader ()) {
                do_read_body ();
            } else {
                room_.leave (shared_from_this ());
            }
        });
    }

    void do_read_body () {
        auto self (shared_from_this ());
        boost::asio::async_read (socket_,
                                 boost::asio::buffer (read_msg_.Body (), read_msg_.BodyLength ()),
                                 [this, self] (boost::system::error_code ec,
                                               std::size_t /*length*/) {
            if (!ec) {
                room_.deliver (read_msg_);
                do_read_header ();
            } else {
                room_.leave (shared_from_this ());
            }
        });
    }

    void do_write () {
        auto self (shared_from_this ());
        boost::asio::async_write (socket_,
                                  boost::asio::buffer (write_msgs_.front ().Data (),
                                  write_msgs_.front ().Length ()),
                                  [this, self] (boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                write_msgs_.pop_front ();
                if (!write_msgs_.empty ()) {
                    do_write ();
                }
            } else {
                room_.leave (shared_from_this ());
            }
        });
    }

    tcp::socket socket_;
    chat_room& room_;
    ChatMessage read_msg_;
    chat_message_queue write_msgs_;
};

class ChatServer {
    public:
    ChatServer (boost::asio::io_service& io_service,
                 const tcp::endpoint& endpoint)
        : acceptor_ (io_service, endpoint),
        socket_ (io_service) {
        DoAccept ();
    }

    private:
    void DoAccept () {
        acceptor_.async_accept (socket_,
                                [this] (boost::system::error_code ec) {
            if (!ec) {
                std::make_shared<chat_session> (std::move (socket_), room_)->start ();
            }

            DoAccept ();
        });
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
    chat_room room_;
};

int main (int argc, char* argv[]) {
    try {
        if (argc < 2) {
            std::cerr << "\n| Usage: chat_server <port> [<port> ...]\n";
            return 1;
        }

        boost::asio::io_service io_service;

        std::list<ChatServer> servers;
        for (int i = 1; i < argc; ++i) {
            tcp::endpoint endpoint (tcp::v4 (), std::atoi (argv[i]));
            servers.emplace_back (io_service, endpoint);
        }

        io_service.run ();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what () << "\n";
    }

    return 0;
}
