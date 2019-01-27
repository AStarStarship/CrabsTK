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
#include "../../source/kabuki/id/global.h"

using boost::asio::ip::tcp;

using namespace _;
using namespace kabuki::cards;
using namespace kabuki::id;


typedef std::deque<std::string> ExpressionQueue;

class ChatParticipant {
    public:
    
    virtual ~ChatParticipant () {}

    virtual void Deliver (const std::string& message) = 0;
};

typedef std::shared_ptr<ChatParticipant> chat_participant_ptr;

class ChatRoom {
    public:
    enum { kMaxRecentMessages = 100 };

    void Join (chat_participant_ptr participant) {
        participants_.insert (participant);
        for (auto message : recent_msgs_)
            participant->Deliver (message);
    }

    void Leave (chat_participant_ptr participant) {
        participants_.erase (participant);
    }

    void Deliver (const std::string& message) {
        recent_msgs_.push_back (message);
        while (recent_msgs_.size () > kMaxRecentMessages)
            recent_msgs_.pop_front ();

        for (auto participant : participants_)
            participant->Deliver (message);
    }

    private:

    std::set<chat_participant_ptr> participants_;
    ExpressionQueue             recent_msgs_;
};

class ChatSession : public ChatParticipant, 
                    public std::enable_shared_from_this<ChatSession> {
    public:

    ChatSession (tcp::socket socket, ChatRoom& room) :
        socket_ (std::move (socket)), 
        room_ (room) {
    }

    void Start () {
        room_.Join (shared_from_this ());
        ReadHeader ();
    }

    void Deliver (const std::string& message) {
        BOL write_in_progress = !expression_queue.empty ();
        expression_queue.push_back (message);
        if (!write_in_progress) {
            Write ();
        }
    }

    private:

    void ReadHeader () {
        auto self (shared_from_this ());
        boost::asio::async_read (socket_, boost::asio::buffer (read_message_.Data (),
                                 std::string::kHeaderLength),
                                 [this, self] (boost::system::error_code ec,
                                               std::size_t /*length*/) {
            if (!ec && read_message_.DecodeHeader ()) {
                ReadBody ();
            } else {
                room_.Leave (shared_from_this ());
            }
        });
    }

    void ReadBody () {
        auto self (shared_from_this ());
        boost::asio::async_read (socket_,
                                 boost::asio::buffer (read_message_.Body (),
                                 read_message_.BodyLength ()),
                                 [this, self] (boost::system::error_code ec,
                                               std::size_t /*length*/) {
            if (!ec) {
                room_.Deliver (read_message_);
                ReadHeader ();
            } else {
                room_.Leave (shared_from_this ());
            }
        });
    }

    void Write () {
        auto self (shared_from_this ());
        boost::asio::async_write (socket_,
                                  boost::asio::buffer (expression_queue.front ().Data (),
                                  expression_queue.front ().Length ()),
                                  [this, self] (boost::system::error_code ec,
                                                std::size_t /*length*/) {
            if (!ec) {
                expression_queue.pop_front ();
                if (!expression_queue.empty ()) {
                    Write ();
                }
            } else {
                room_.Leave (shared_from_this ());
            }
        });
    }

    tcp::socket socket_;
    ChatRoom& room_;
    std::string read_message_;
    ExpressionQueue expression_queue;
};

class KabukiCardsServer : public ::kabuki::cards::Server {
    public:
    enum {
        kMaxNumUsers = 1024
    };
    KabukiCardsServer (boost::asio::io_service& io_service,
                 const tcp::endpoint& endpoint) :
        acceptor_ (io_service, endpoint),
        socket_ (io_service){
        AcceptConnection ();
    }

    private:
    void AcceptConnection () {
        acceptor_.async_accept (socket_, [this](boost::system::error_code ec) {
            if (!ec) {
                std::make_shared<ChatSession> (std::move (socket_), 
                                               room_)->Start ();
            }

            AcceptConnection ();
        });
    }

    tcp::acceptor acceptor_;
    tcp::socket   socket_;
    ChatRoom      room_;
};

SI4 main (SI4 argc, CH1* argv[]) {
    try {
        if (argc < 2) {
            std::cerr << "\n| Welcome to Kabuki Cards Usage: cards_server <port> [<port> ...]";
            return 1;
        }

        boost::asio::io_service io_service;

        std::list<KabukiCardsServer> servers;
        for (SI4 i = 1; i < argc; ++i) {
            tcp::endpoint endpoint (tcp::v4 (), std::atoi (argv[i]));
            servers.emplace_back (io_service, endpoint);
        }

        io_service.run ();
    } catch (std::exception& e) {
        std::cerr << "\n| Exception: " << e.what ();
    }

    return 0;
}
