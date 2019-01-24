/** TypeCraft - Teaching Typing through Mining and Crafting.
@link    https://github.com/kabuki-starship/kabuki-projects/typecraft
@file    /projects/typecraft/typecraft_server.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include "cserver.h"

namespace typecraft {

/** Key	Type	Default Value	Description
allow-flight	boolean	false	Allows users to use flight on your server while
in Survival mode, if they have a mod that provides flight installed. With
allow-flight enabled, griefers will possibly be more common, because it will
make their work easier. In Creative mode this has no effect.

false - Flight is not allowed (players in air for at least 5 seconds will be
kicked). true - Flight is allowed, and used if the player has a fly mod
installed. allow-nether	boolean	true	Allows players to travel to the Nether.
false - Nether portals will not work.
true - The server will allow portals to send players to the Nether.
difficulty	integer (0-3)	1	Defines the difficulty (such as damage
dealt by mobs and the way hunger and poison affects players) of the server. 0 -
Peaceful 1 - Easy 2 - Normal 3 - Hard enable-query	boolean	false	Enables
GameSpy4 protocol server listener. Used to get information about server.
enable-rcon	boolean	false	Enables remote access to the server console.
enable-command-block	boolean	false	Enables command blocks
force-gamemode	boolean	false	Force players to join in the default game mode.
false - Players will join in the gamemode they left in.
true - Players will always join in the default gamemode.
gamemode	integer (0-3)	0	Defines the mode of gameplay.
0 - Survival
1 - Creative
2 - Adventure
3 - Spectator
generate-structures	boolean	true	Defines whether structures (such as
villages) will be generated. false - Structures will not be generated in new
chunks. true - Structures will be generated in new chunks. Note: Dungeons will
still generate if this is set to false.

generator-settings	string	blank	The settings used to customize world
generation. See Superflat and Customized for possible settings and examples.
hardcore	boolean	false	If set to true, players will be set to spectator
mode if they die. level-name	string	world	The "level-name" value will be
used as the world name and its folder name. You may also copy your saved game
folder here, and change the name to the same as that folder's to load it
instead. Characters such as ' (apostrophe) may need to be escaped by adding a
backslash before them.
level-seed	string	blank	Add a seed for your world, as in Singleplayer.
Some examples are: minecraft, 404, 1a2b3c.
level-type	string	DEFAULT	Determines the type of map that is generated.
DEFAULT - Standard world with hills, valleys, water, etc.
FLAT - A flat world with no features, meant for building.
LARGEBIOMES - Same as default but all biomes are larger.
AMPLIFIED - Same as default but world-generation height limit is increased.
CUSTOMIZED - Same as default unless generator-settings is set to a preset.
max-build-height	integer	256	The maximum height in which building is
allowed. Terrain may still naturally generate above a low height limit.
max-players	integer (0-2147483647)	20	The maximum number of players
that can play on the server at the same time. Note that if more players are on
the server it will use more resources. Note also, op player connections are not
supposed to count against the max players, but ops currently cannot join a full
server. Extremely large values for this field result in the client-side user
list being broken. max-tick-time integer (0–(2^63 - 1))	60000	The maximum
number of milliseconds a single tick may take before the server watchdog stops
the server with the message, A single server tick took 60.00 seconds (should be
max 0.05); Considering it to be crashed, server will forcibly shutdown. Once
this criterion is met, it calls System.exit(1). -1 - disable watchdog entirely
(this disable option was added in 14w32a) max-world-size integer (1-29999984)
29999984	This sets the maximum possible size in blocks, expressed as a
radius, that the world border can obtain. Setting the world border bigger causes
the commands to complete successfully but the actual border will not move past
this block limit. Setting the max-world-size higher than the default doesn't
appear to do anything. Examples:

Setting max-world-size to 1000 will allow you to have a 2000x2000 world border.
Setting max-world-size to 4000 will give you an 8000 x 8000 world border.
motd	string	A Minecraft Server	This is the message that is displayed in
the server list of the client, below the name. The MOTD does support color and
formatting codes. The MOTD supports special characters, such as "♥". However,
such characters must be converted to escaped Unicode form. An online converter
can be found here If the MOTD is over 59 characters, the server list will likely
report a communication error. network-compression-threshold
integer	256	By default it allows packets that are n-1 bytes big to go
normally, but a packet that n bytes or more will be compressed down. So, lower
number means more compression but compressing small amounts of bytes might
actually end up with a larger result than what went in. -1 - disable compression
entirely 0 - compress everything Note: The Ethernet spec requires that packets
less than 64 bytes become padded to 64 bytes. Thus, setting a value lower than
64 may not be beneficial. It is also not recommended to exceed the MTU,
typically 1500 bytes.

online-mode	boolean	true	Server checks connecting players against
Minecraft's account database. Only set this to false if your server is not
connected to the Internet. Hackers with fake accounts can connect if this is set
to false! If minecraft.net is down or inaccessible, no players will be able to
connect if this is set to true. Setting this variable to off purposely is called
"cracking" a server, and servers that are presently with online mode off are
called "cracked" servers, allowing players with unlicensed copies of Minecraft
to join. true - Enabled. The server will assume it has an Internet connection
and check every connecting player. false - Disabled. The server will not attempt
to check connecting players.
op-permission-level	integer (1-4)	4	Sets the default permission
level for ops when using /op. All levels inherit abilities and commands from
levels before them. 1 - Ops can bypass spawn protection. 2 - Ops can use all
singleplayer cheats commands (except /publish, as it is not on servers; along
with /debug and /reload) and use command blocks. Command blocks, along with
Realms owners/operators, have the same permissions as this level. 3 - Ops can
use most multiplayer-exclusive commands, including /debug and commands that
manage players (/ban, /op, etc). 4 - Ops can use all commands including /stop,
/save-all, /save-on, /save-off, and /reload.
player-idle-timeout	integer	0	If non-zero, players are kicked from the
server if they are idle for more than that many minutes. Note: Idle time is
reset when the server receives one of the following packets: Click Window
Enchant Item
Update Sign
Player Digging
Player Block Placement
Held Item Change
Animation (swing arm)
Entity Action
Client Status
Chat Message
Use Entity
prevent-proxy-connections	boolean	false	If the ISP/AS sent from the
server is different from the one from Mojang's authentication server, the player
is kicked true - Enabled. Server prevents users from using vpns or proxies.

false - Disabled. The server doesn't prevent users from using vpns or proxies.

pvp	boolean	true	Enable PvP on the server. Players shooting themselves
with arrows will only receive damage if PvP is enabled. true - Players will be
able to kill each other. false - Players cannot kill other players (also known
as Player versus Environment (PvE)). Note: Indirect damage sources spawned by
players (such as lava, fire, TNT and to some extent water, sand and gravel) will
still deal damage to other players.

query.port	integer (1-65534)	25565	Sets the port for the query
server (see enable-query). rcon.password	string	blank	Sets the
password to rcon. rcon.port	integer (1-65534)	25575	Sets the port to
rcon.
resource-pack	string	blank	Optional URI to a resource pack. The player may
choose to use it. resource-pack-sha1 string	blank	Optional SHA-1 digest of
the resource pack, in lowercase hexadecimal. It's recommended to specify this.
This is not yet used to verify the integrity of the resource pack, but improves
the effectiveness and reliability of caching. server-ip	string	blank	Set this
if you want the server to bind to a particular IP. It is strongly recommended
that you leave server-ip blank! Set to blank, or the IP you want your server to
run (listen) on. server-port	integer (1-65534)	25565	Changes the port
the server is hosting (listening) on. This port must be forwarded if the server
is hosted in a network using NAT (If you have a home router/firewall).
snooper-enabled	boolean	true	Sets whether the server sends snoop data
regularly to http://snoop.minecraft.net. false - disable snooping. true - enable
snooping. spawn-animals	boolean	true	Determines if animals will be able to
spawn. true - Animals spawn as normal. false - Animals will immediately vanish.
Tip: if you have major lag, turn this off/set to false.

spawn-monsters	boolean	true	Determines if monsters will be spawned.
true - Enabled. Monsters will appear at night and in the dark.
false - Disabled. No monsters.
This setting has no effect if difficulty = 0 (peaceful). If difficulty is not =
0, a monster can still spawn from a Monster Spawner.

Tip: if you have major lag, turn this off/set to false.

spawn-npcs	boolean	true	Determines whether villagers will be spawned.
true - Enabled. Villagers will spawn.
false - Disabled. No villagers.
spawn-protection	integer	16	Determines the radius of the spawn
protection as (x*2)+1. Setting this to 0 will not disable spawn protection. 0
will protect the single block at the spawn point. 1 will protect a 3x3 area
centered on the spawn point. 2 will protect 5x5, 3 will protect 7x7, etc. This
option is not generated on the first server start and appears when the first
player joins. If there are no ops set on the server, the spawn protection will
be disabled automatically. use-native-transport	boolean	true	Linux server
performance improvements: optimized packet sending/receiving on Linux true -
Enabled. Enable Linux packet sending/receiving optimization false - Disabled.
Disable Linux packet sending/receiving optimization view-distance	integer
(2-32)	10	Sets the amount of world data the server sends the client,
measured in chunks in each direction of the player (radius, not diameter). It
determines the server-side viewing distance. (see Render distance) 10 is the
default/recommended. If you have major lag, reduce this value.

white-list	boolean	false	Enables a whitelist on the server.
With a whitelist enabled, users not on the whitelist will be unable to connect.
Intended for private servers, such as those for real-life friends or strangers
carefully selected via an application process, for example.

false - No white list is used.
true - The file whitelist.json is used to generate the white list.
Note: Ops are automatically white listed, and there is no need to add them to
the whitelist.

Minecraft Classic server properties
Key	Type	Default Value	Description
verify-names	boolean	true	If enabled the server will make sure that the
client is logged in with the same IP on Minecraft.net. This has caused problems
for people trying to play on the same computer they are hosting the server on as
the server will see the local IP (127.0.0.1) while Minecraft.net will see the
external IP. It is recommended that this is enabled unless you want to play on
your server from the same computer you are hosting it on true - Enabled. The
server will check all names with Minecraft.net false - Disabled. The server will
make sure the IP matches admin-slot	boolean	false	Allow ops to join even
if the server is full.
public	boolean		Whether the server should be displayed in the server
list, or not. true - The server will be displayed in the server list - anyone
can see it. false - Only people with the URL which can be found in
externalurl.txt and people who know the IP and port can join the server
server-name	string		The name of the server. This is displayed in the
server list and when someone is joining the server A single line of text.
Characters such as ' (apostrophe) may need to be escaped by adding a backslash
before them.
max-players	integer (0-256)		The max numbers of players that can play
on the server at the same time. Note that if more players are on the server it
will use more resources. Note also, admin connections are not counted against
the max players. max-connections	integer (1-3)		The max number
of connections the server will accept from the same IP. Note: If you try to set
it higher than 3 it will be changed back to 3.

motd	string		MOTD is short for Message of the day, though you do not
need to change it every day. The MOTD is displayed when people join the server.
grow-trees	boolean		Whether or not the server will allow planted
saplings to grow into trees. true - the server will allow saplings to grow into
trees. false - the server will not allow saplings to grow into trees.

*/

Server::Server() {}

}  // namespace typecraft
