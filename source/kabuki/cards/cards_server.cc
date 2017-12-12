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

Server::Server (const char* password, uint32_t port, int max_games) :
                Room           ("Kabuki_Cards_Server"),
                authenticator_ (),
                password_      (&authenticator_, password),
                state_         (0),
                port_          (port),
                users_         (&authenticator_, kDefaultMaxUsers),
                games_         (),
                directions_    (StrandClone ("")) {
    games_.reserve (max_games);
}

Server::~Server () {
}

void Server::RestartServer () {

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

int Server::AddGame (CardGame* game) {
    games_.push_back (game);
    return GetNumTables () - 1;
}

int Server::AddBlackjackGame () {
    User* user = users_.GetUser (AddAgent ("Dealer"));
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

void Server::Print () {
    cout << "\n| Server: " << GetRoomName ();
}

int Server::AddAgent (const char* handle_prefix,
                      double balance, uint64_t value) {
    char handle[32];
    char* token;
    if (!(token = TextWrite (handle, handle + 32, handle_prefix))) {
        return -1;
    }
    if (!TextWrite (token, handle + 32, users_.GetCount ())) {
        return -1;
    }
    return users_.Register (handle, password_.GetKey (), balance, value);
}

const char* Server::HandleText (const char* text, const char* text_end) {
    const char* next_token;
    char        handle[Handle::kDefaultMaxLength + 1],
                password[Password::kDefaultMaxLength + 1];
    int32_t     game_number;
                //session;
    double      money;
    uint64_t    chips;
    //User      * user;

    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }
    if (*text == '@') {
        return users_.HandleText (text + 1, text_end);
    } else if (TextTokenEquals (text, text_end, "Quit")) {
        RestartServer ();
    } else if (TextTokenEquals (text, text_end, "Restart")) {
        RestartServer ();
    } else if (TextTokenEquals (text, text_end, "LogIn")) {
        next_token = TextRead (text, text_end, handle,
                               handle + Handle::kDefaultMaxLength + 1);
        if (!next_token) {
            return nullptr;
        }
        next_token = TextRead (text, text_end, password,
                               password + Password::kDefaultMaxLength + 1);
        if (!next_token) {
            return nullptr;
        }
        users_.LogIn (handle, password);
    } else if (TextTokenEquals (text, text_end, "Register")) {
        next_token = TextRead (text, text_end, handle,
                               handle + Handle::kDefaultMaxLength + 1);
        if (!next_token) {
            return nullptr;
        }
        next_token = TextRead (text, text_end, password,
                               password + Password::kDefaultMaxLength + 1);
        if (!next_token) {
            return nullptr;
        }
        next_token = TextRead (text, text_end, money);
        if (!next_token) {
            return nullptr;
        }
        next_token = TextRead (text, text_end, chips);
        if (!next_token) {
            return nullptr;
        }
        users_.Register (handle, password);
    } else if (TextTokenEquals (text, text_end, "Unregister")) {
        next_token = TextRead (text, text_end, handle,
                               handle + Handle::kDefaultMaxLength + 1);
        if (!next_token) {
            return nullptr;
        }
        next_token = TextRead (text, text_end, password,
                               password + Password::kDefaultMaxLength + 1);
        if (!next_token) {
            return nullptr;
        }
        users_.Unregister (handle, password);
    } else if (TextTokenEquals (text, text_end, "AddAgent")) {
        game_number = -1;
        next_token = TextRead (text, text_end, game_number);
        if (!next_token) {
            return nullptr;
        }
        next_token = TextRead (text, text_end, money);
        if (!next_token) {
            return nullptr;
        }
        next_token = TextRead (text, text_end, chips);
        if (!next_token) {
            return nullptr;
        }
        AddAgent ("Terminator_", money, chips);
    } else if (TextTokenEquals (text, text_end, "?")) {
        cout << Star ('?', nullptr)->metadata;
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
        "\n|   Quit     | Quits the server without saving.                 |"
        "\n|  Restart   | Restarts the server.                             |"
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
        "\n| typing \"@dealer buy num_chips\", where num_chips is an       |"
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
    char handle[Handle::kDefaultMaxLength + 1],
        password[Password::kDefaultMaxLength + 1];
    //buffer[kMaxTextLength + 1];
    User  * user;
    int32_t session;
    uid_t   session_key;

    switch (index) {
        case '?': return &This;
        case 'A': {
            static const Operation OpA = { "LogIn",
                Params<2, STR, Handle::kDefaultMaxLength, STR,
                Password::kDefaultMaxLength> (),
                Params<2, UI8, UI8> (),
                "Attempts to login with the given #handle and #password and "
                "returns a #session_number and #session_key.", 0
            };
            if (!expr) return &OpA;
            if (ExprArgs (expr, Params<2, STR, Handle::kDefaultMaxLength, STR,
                Password::kDefaultMaxLength> (), Args (args, handle, password)))
            {
                return expr->result;
            }
            session_key = users_.LogIn (handle, password);
            if (!session_key) {
                Result (expr, Bin::kErrorInvalidArgs);
            }
            return ExprResult (expr, Params<2, UI8, UI8> (), Args (args, &session,
                               &session_key));
        }
        case 'B': {
            static const Operation OpB = { "Register",
                Params<2, STR, Handle::kDefaultMaxLength,
                STR, Password::kDefaultMaxLength> (),
                Params<2, UI8, UI8> (),
                "Attempts to register with the given handle:STR and "
                "password:STR and returns a #session_number and "
                "session_key:STR.", 0 };

            if (!expr) return &OpB;
            if (ExprArgs (expr, Params<2, STR, Handle::kDefaultMaxLength, STR,
                Password::kDefaultMaxLength> (), Args (args, handle, password))) {
                return expr->result;
            }
            session = users_.Register (handle, password);
            if (session < 0) { // Invalid login credentials
                session = 0;
                session_key = 0;
                return ExprResult (expr, Params<2, UI8, UI8> (),
                                   Args (args, &session, &session_key));
            }
            user = users_.GetUser (session);
            //if (!user) { // This will never happen.
            //    return Result (expr, Bin::kErrorRoomError);
            //}
            session     = user->GetSession ();
            session_key = user->GetSessionKey ();
            session_key = Random<uid_t> ();
            return ExprResult (expr, Params<2, UI8, UI8> (),
                               Args (args, &session, &session_key));
        }
        case 'C': {
            static const Operation OpC = { "DeleteUser",
                Params<2, STR, Handle::kDefaultMaxLength,
                STR, Password::kDefaultMaxLength> (),
                Params<0> (),
                "Attempts to delete with the given #handle and #password.", 0 };

            if (!expr) return &OpC;
            if (ExprArgs (expr, Params<2, STR, Handle::kDefaultMaxLength, STR,
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
            static const Operation OpD = { "AddGame",
                Params<2, STR, Handle::kDefaultMaxLength, STR,
                Password::kDefaultMaxLength> (),
                Params<2, UI8, UI8> (),
                "Adds a new game:STR to the server.", 0 };
            if (!expr) return &OpD;
        }
        case 'E': {
            static const Operation OpE = { "DeleteGame",
                Params<2, STR, Handle::kDefaultMaxLength, STR,
                Password::kDefaultMaxLength> (),
                Params<2, UI8, UI8> (),
                "Deletes the given game:SI4.", 0 };
            if (!expr) return &OpE;
        }
        case 'F': {
            static const Operation OpF = { "ListGames",
                Params<1, STR, Handle::kDefaultMaxLength, STR,
                Password::kDefaultMaxLength> (),
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
