/** kabuki::cards
    @file    ~/source/kabuki/cards/cards_server.cc
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

#include "server.h"

using namespace _;
using namespace kabuki::id;
using namespace std;

namespace kabuki { namespace cards {

Server::Server (const char* server_name, uint32_t port, int max_games) :
                Room       (server_name),
                authenticator_ (new AuthenticatorDefault ()),
                state_   (0),
                port_    (port),
                users_   (authenticator_, kDefaultMaxUsers),
                games_   (max_games) {
    // Nothing to do :-)
}

Server::~Server () {
    delete authenticator_;
}

int Server::GetState () {
    return state_;
}

bool Server::SetState (int state) {
    if (state < 0) {
        return false;
    }
    state_ = state;
    return true;
}

const char* Server::GetServerName () {
    return Room::GetRoomName ();
}

void Server::Print () {
    cout << "\n| Server: " << GetRoomName ();
}

const Operation* Server::Star (uint index, Expression* expr) {
    static const Operation This = { "kabuki::cards::Server",
        NumOperations (0), FirstOperation ('A'),
        "\n| Welcome to Kabuki Cards Server :-)"
        "\n|"
        "\n| Directions: Server currently only plays the game of Blackjack but "
        "\n| more games are planned for the future. To play the game, you will "
        "\n| need to join a game that has already been created or create a new "
        "\n| Blackjack game. To Enter a game type $game_name."
        "\n|"
        "\n| When in you enter a game you are an observer waiting to join a game."
        "\n| Observers join the game in a First-In-First-Out (FIFO) queue. You may "
        "\n| leave the current game any time by type \"leave\"."
        "\n|"
        "\n| To join a game you will have to purchase chips for the buy typing "
        "\n| \"@dealer_number buy num_chips\", where num_chips is a positive "
        "\n| integer.\n"
        "\n| "
        "\n| To send a message to a another player, type @handle followed by a "
        "\n| 140 character or less tweet. You may send tweets to all the players "
        "\n| and observers in a game using @all. "
        "\n|"
        "\n| You may exit at any time by typing \"exit\".", 0 };
    void* args[2];
    char handle[Handle::kDefaultMaxLength + 1],
         password[Password::kDefaultMaxLength + 1];
         //buffer[kMaxStringLength + 1];
    User  * user;
    int32_t session;
    uid_t   session_key;

    switch (index) {
        case '?': return &This;
        case 'A': {
            static const Operation OpA = { "LogIn",
                Params<2, STX, Handle::kDefaultMaxLength, STX,
                       Password::kDefaultMaxLength> (),
                Params<2, UI8, UI8> (),
                "Attempts to login with the given #handle and #password and "
                "returns a #session_number and #session_key.", 0 };

            if (!expr) return &OpA;
            if (ExprArgs (expr, Params<2, STX, Handle::kDefaultMaxLength, STX,
                Password::kDefaultMaxLength> (), Args (args, handle, password))) {
                return expr->result;
            }
            session = users_.LogIn (handle, password);
            if (!session) {
                Result (expr, Bin::kErrorInvalidArgs);
            }
            session_key = Random<uid_t> ();
            return ExprResult (expr, Params<2, UI8, UI8> (), Args (args, &session,
                                                                   &session_key));
        }
        case 'B': {
            static const Operation OpB = { "Register",
                Params<2, STX, Handle::kDefaultMaxLength, 
                       STX, Password::kDefaultMaxLength> (),
                Params<2, UI8, UI8> (),
                "Attempts to register with the given #handle and #password and "
                "returns a #session_number and #session_key.", 0 };

            if (!expr) return &OpB;
            if (ExprArgs (expr, Params<2, STX, Handle::kDefaultMaxLength, STX,
                Password::kDefaultMaxLength> (), Args (args, handle, password))) {
                return expr->result;
            }
            session = users_.Register (handle, password);
            if (session < 0) { // Invalid login credentials
                session = 0;
                session_key = 0;
                return ExprResult (expr, Params<2, UI8, UI8> (), Args (args, &session,
                                   &session_key));
            }
            user = users_.GetUser (session);
            //if (!user) { // This will never happen.
            //    return Result (expr, Bin::kErrorRoomError);
            //}
            session = user->GetSession ();
            session_key = user->GetSessionKey ();
            session_key = Random<uid_t> ();
            return ExprResult (expr, Params<2, UI8, UI8> (), Args (args, &session,
                               &session_key));
        }
        case 'C': {
            static const Operation OpC = { "DeleteUser",
                Params<2, STX, Handle::kDefaultMaxLength, 
                       STX, Password::kDefaultMaxLength> (),
                Params<0> (),
                "Attempts to delete with the given #handle and #password.", 0 };

            if (!expr) return &OpC;
            if (ExprArgs (expr, Params<2, STX, Handle::kDefaultMaxLength, STX,
                Password::kDefaultMaxLength> (), Args (args, handle, password))) {
                return expr->result;
            }
            session = users_.Find (handle);
            if (session < 0) { // Invalid login credentials
                session = 0;
                session_key = 0;
                return ExprResult (expr, Params<2, UI8, UI8> (),
                                   Args (args, &session, &session_key));
            }
            user = users_.GetUser (session);
            if (!user->GetPassword ().Equals (password)) {
                return nullptr;
            }
            users_.Remove (session);
            return nullptr;
        }
        case 'D': {
            static const Operation OpD = { "CreateBlackjackGame",
                Params<2, STX, Handle::kDefaultMaxLength, STX,
                Password::kDefaultMaxLength> (),
                Params<2, UI8, UI8> (),
                "Attempts to login with the given #handle and #password and "
                "returns a #session_number and #session_key.", 0 };
            if (!expr) return &OpD;
        }
        case 'E': {
            static const Operation OpE = { "DeleteBlackjackGame",
                Params<2, STX, Handle::kDefaultMaxLength, STX,
                Password::kDefaultMaxLength> (),
                Params<2, UI8, UI8> (),
                "Attempts to login with the given #handle and #password and "
                "returns a #session_number and #session_key.", 0 };
            if (!expr) return &OpE;
        }
        case 'F': {
            static const Operation OpF = { "ListGames",
                Params<1, STX, Handle::kDefaultMaxLength, STX,
                Password::kDefaultMaxLength> (),
                Params<1, STX, kMaxStringLength> (),
                "Lists the current games.", 0 };
            if (!expr) return &OpF;
            for (int i = 0; i < games_.GetCount (); ++i) {

            }
        }
    }
    if (index < 'A') {
        return Result (expr, Bin::kErrorInvalidOperation);
    }
    return Push (expr, dynamic_cast<Operand*> (games_[index]));
}

}       //< namespace cards
}       //< namespace kabuki
