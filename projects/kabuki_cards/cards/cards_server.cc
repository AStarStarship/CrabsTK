/** Kabuki Arcade
    @file    ~/source/kabuki/arcade/cards/arcade_server.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

Server::Server (const char* password, uint32_t port, int max_games) :
                Room           ("Kabuki_Cards_Server"),
                authenticator_ (),
                password_      (&authenticator_, password),
                state_         (SetState (kStateInitializing)),
                port_          (port),
                users_         (&authenticator_, kDefaultMaxUsers),
                games_         (),
                directions_    (StrandClone ("\n>")) {
    games_.reserve (max_games);
}

Server::~Server () {
}

const char* Server::Exit () {
    SetState (kStateShuttingDown);
    return nullptr;
}

const char* Server::Restart () {
    return nullptr;
}

int Server::GetState () {
    return state_;
}

Server::State Server::SetState (State state) {
    switch (state) {
        case State::kStateShuttingDown: {
            cout << "\n| Exiting server...\n|";
        }
        case State::kStateInitializing: {
            cout << "\n| Initializing server...\n|";
        }
        case State::kStateServingClients: {
            cout << "\n| Initializing completed! ({:-0)+=<\n|";
        }
    }
    system ("PAUSE");
    state_ = state;
    return state;
}

uint32_t Server::GetPort () {
    return port_;
}

bool Server::SetPort (uint32_t port_number) {
    if (port_number <= 1024) {
        // Can't use a well known TCP port.
        return false;
    }
    port_ = port_number;
    return true;
}

id::UserList& Server::Users () {
    return users_;
}

int Server::GetNumTables () {
    return games_.size ();
}

int Server::AddGame (Game* game) {
    games_.push_back (game);
    return GetNumTables () - 1;
}

int Server::AddBlackjackGame () {
    char handle[64];
    StrandWrite (StrandWrite (handle, handle + 64, "Dealer"),
               handle + 64, users_.PeekNextUid ());
    User* user = users_.GetUser (AddAgent (handle));
    if (!user) {
        return -1;
    }
    cout << "\n| Adding new Blackjack table " << user->GetHandleKey ();
    return AddGame (new BlackjackGame (users_, user));
}

bool Server::RemoveGame (int index) {
    if (index < 0) {
        return false;
    }
    if (index >= GetNumTables ()) {
        return false;
    }
    delete games_[index];
    games_.erase (games_.begin () + index);
    return true;
}

const char* Server::GetDirections () {
    return directions_;
}

bool Server::SetDirections (const char* directions) {
    if (!directions) {
        return false;
    }
    delete directions_;
    directions = StrandClone (directions);
    return true;
}

void Server::Print (_::Text& txt) {
    cout << "\n| Server: " << GetRoomName ();
}

int Server::AddAgent (const char* handle_prefix,
                      double balance, uint64_t value) {
    char handle[32];
    char* token;
    if (!(token = StrandWrite (handle, handle + 32, handle_prefix))) {
        return -1;
    }
    if (!StrandWrite (token, handle + 32, users_.Length ())) {
        return -1;
    }
    return users_.Add (handle, password_.GetKey (), balance, value);
}

const char* Server::Sudo (const char* text, const char* strand_end) {
    const char* token_end;
    int         index;
    //char      handle[Handle::kMaxLength + 1],
    //          password[Password::kMaxLength + 1];
    //int32_t   game_number;
    //          session;
    //double    balance;
    //uint64_t  chips;
    //User    * user;

    if (!text) {
        return nullptr;
    }
    if (text > strand_end) {
        return nullptr;
    }
    if (*text == '@') {
        return users_.Sudo (text + 1, strand_end);
    } else if (token_end = TokenEquals (text, strand_end, "exit")) {
        return Exit ();
    } else if (token_end = TokenEquals (text, strand_end, "restart")) {
        return Restart ();
    } else if (token_end = TokenEquals (text, strand_end, "add")) {
        if (token_end = TokenEquals (token_end + 1, strand_end, "blackjack")) {
            AddBlackjackGame ();
            return token_end;
        }
        cout << "\n| Sorry but that game has not been programmed yet."
             << "Your only option is "
             << "\n| \"blackjack\" until you program another game.";

        return nullptr;
    } else if (token_end = TokenEquals (text, strand_end, "remove")) {
        if (!(token_end = TextRead (token_end + 1, strand_end, index))) {
            return nullptr;
        }
        RemoveGame (index);
        return token_end;

    } else if (token_end = TokenEquals (text, strand_end, "?")) {
        cout << Star ('?', nullptr)->description;
    }
    cout << "\n| Invalid input.";
    return nullptr;
}

const Operation* Server::Star (uint index, Expression* expr) {
    static const Operation This { "Kabuki_Cards_Server",
        OperationCount (0), OperationFirst ('A'),
        "\n ________________________________________________________________"
        "\n|                                                               |"
        "\n| kabuki::cards::Server Help                                    |"
        "\n|_______________________________________________________________|"
        "\n| Data Type  | Description                                      |"
        "\n|------------|--------------------------------------------------|"
        "\n|    SI4     | 32-bit signed integer.                           |"
        "\n|    SI8     | 64-bit signed integer.                           |"
        "\n|    STR     | UTF-8 or ASCII String.                           |"
        "\n|____________|__________________________________________________|"
        "\n|  Command   | Description                                      |"
        "\n|------------|--------------------------------------------------|"
        "\n|   exit     | Exits the server without saving.                 |"
        "\n|  restart   | Restarts the server.                             |"
        "\n|  AddAgent  | Adds agent to given SI4:game_number, DBL:money,  |"
        "\n|            | and SI8:num_chips.                               |"
        "\n|  ListUsers | Lists the users.                                 |"
        "\n|  Register  | Adds a user with given STR:handle, STR:password, |"
        "\n|            | DBL:money, and SI8:chips.                        |"
        "\n| DeleteUser | Deletes the given STR:user.                      |"
        "\n|____________|__________________________________________________|"
        "\n|                                                               |"
        "\n| Directions: Server currently only plays the game of Blackjack |"
        "\n| but more games are planned for the future. To play the game,  |"
        "\n| you will need to join a game that has already been created or |"
        "\n| create a new Blackjack game. To Enter a game type $game_name. |"
        "\n|                                                               |"
        "\n| When in you enter a game you are an observer waiting to join  |"
        "\n| a game. Observers join the game in a First-In-First-Out(FIFO) |"
        "\n| queue. You may leave the current game any time by type        |"
        "\n|                                                               |"
        "\n| To join a game you will have to purchase chips for the buy    |"
        "\n| typing \"@dealer buy num_chips\", where num_chips is an         |"
        "\n| integer.                                                      |"
        "\n|                                                               |"
        "\n| To send a message to a another player, type @handle followed  |"
        "\n| by a 140 character or less tweet. You may send tweets to all  |"
        "\n| the players and observers in a game using @all.               |"
        "\n|                                                               |"
        "\n| You may leave the current game at any time by typing \"leave\". |"
        "\n|                                                               |"
        "\n| You may quit at any time by typing \"quit\".                    |"
        "\n|                                                               |"
        "\n| You may exit at any time by typing \"exit\".                    |"
        "\n|_______________________________________________________________|",
        0 
    };

    void* args[2];
    char handle[Handle::kMaxLength + 1],
        password[Password::kMaxLength + 1];
    //buffer[kMaxTextLength + 1];
    User  * user;
    int32_t session;
    uid_t   public_key;

    switch (index) {
        case '?': return &This;
        case 'A': {
            static const Operation OpA = { "LogIn",
                Params<2, STR, Handle::kMaxLength, STR,
                Password::kMaxLength> (),
                Params<2, UI8, UI8> (),
                "Attempts to login with the given #handle and #password and "
                "returns a #session_number and #public_key.", 0
            };
            if (!expr) return &OpA;
            if (ExpressionArgs (expr, Params<2, STR, Handle::kMaxLength, STR,
                Password::kMaxLength> (), Args (args, handle, password)))
            {
                return expr->result;
            }
            public_key = users_.LogIn (handle, password);
            if (!public_key) {
                Result (expr, Bin::kErrorInvalidArgs);
            }
            return ExprResult (expr, Params<2, UI8, UI8> (), Args (args, &session,
                               &public_key));
        }
        case 'B': {
            static const Operation OpB = { "Register",
                Params<2, STR, Handle::kMaxLength,
                STR, Password::kMaxLength> (),
                Params<2, UI8, UI8> (),
                "Attempts to register with the given handle:STR and "
                "password:STR and returns a #session_number and "
                "public_key:STR.", 0 };

            if (!expr) return &OpB;
            if (ExpressionArgs (expr, Params<2, STR, Handle::kMaxLength, STR,
                Password::kMaxLength> (), Args (args, handle, password))) {
                return expr->result;
            }
            session = users_.Add (handle, password);
            if (session < 0) { // Invalid login credentials
                session = 0;
                public_key = 0;
                return ExprResult (expr, Params<2, UI8, UI8> (),
                                   Args (args, &session, &public_key));
            }
            user = users_.GetUser (session);
            //if (!user) { // This will never happen.
            //    return Result (expr, Bin::kErrorRoomError);
            //}
            session     = user->GetSession ();
            public_key = user->GetSessionKey ();
            public_key = Random<uid_t> ();
            return ExprResult (expr, Params<2, UI8, UI8> (),
                               Args (args, &session, &public_key));
        }
        case 'C': {
            static const Operation OpC = { "DeleteUser",
                Params<2, STR, Handle::kMaxLength,
                STR, Password::kMaxLength> (),
                Params<0> (),
                "Attempts to delete with the given #handle and #password.", 0 };

            if (!expr) return &OpC;
            if (ExpressionArgs (expr, Params<2, STR, Handle::kMaxLength, STR,
                Password::kMaxLength> (), Args (args, handle, password))) {
                return expr->result;
            }
            session = users_.Find (handle);
            if (session < 0) { // Invalid login credentials
                session = 0;
                public_key = 0;
                return ExprResult (expr, Params<2, UI8, UI8> (),
                                   Args (args, &session, &public_key));
            }
            user = users_.GetUser (session);
            if (!user->GetPassword ().Equals (password)) {
                return nullptr;
            }
            users_.Remove (session);
            return nullptr;
        }
        case 'D': {
            static const Operation OpD = { "AddGame",
                Params<2, STR, Handle::kMaxLength, STR,
                Password::kMaxLength> (),
                Params<2, UI8, UI8> (),
                "Adds a new game:STR to the server.", 0 };
            if (!expr) return &OpD;
        }
        case 'E': {
            static const Operation OpE = { "DeleteGame",
                Params<2, STR, Handle::kMaxLength, STR,
                Password::kMaxLength> (),
                Params<2, UI8, UI8> (),
                "Deletes the given game:SI4.", 0 };
            if (!expr) return &OpE;
        }
        case 'F': {
            static const Operation OpF = { "ListGames",
                Params<1, STR, Handle::kMaxLength, STR,
                Password::kMaxLength> (),
                Params<1, STR, kMaxTextLength> (),
                "Lists the current games.", 0 };
            if (!expr) return &OpF;
            for (int i = 0; i < GetNumTables (); ++i) {

            }
        }
    }
    if (index < 'A') {
        return Result (expr, Bin::kErrorInvalidOperation);
    }
    if (index > (uint)(GetNumTables () + 'A')) {
        return Result (expr, Bin::kErrorInvalidOperation);
    }
    return Push (expr, dynamic_cast<Operand*> (games_[index - 'A']));
}

}       //< namespace cards
}       //< namespace kabuki
