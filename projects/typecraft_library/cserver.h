/* TypeCraft - Teaching Typing through Mining and Crafting.
@link    https://github.com/kabuki-starship/kabuki-toolkit
@file    /project/typecraft/cserver.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#ifndef INCLUDED_TYPECRAFT_SERVER
#define INCLUDED_TYPECRAFT_SERVER 1

#include "cblock.h"
#include "centity.h"

namespace typecraft {

// @cite: Comments stolen from
// https://minecraft.gamepedia.com/Server.properties. @todo Rewrite me!

class Server {
 public:
  /* Creates a default server. */
  Server();

  /* Allows users to use flight on your server while in Survival mode, if they
     have a mod that provides flight installed. With allow-flight enabled,
     griefers will possibly be more common, because it will make their work
     easier. In Creative mode this has no effect. default: false false  : Flight
     is not allowed (players in air for at least 5 seconds will be kicked).
      true   : Flight is allowed, and used if the player has a fly mod
     installed. */
  bool allow_flight;

  /* boolean true Allows players to travel to the Nether.
      false - Nether portals will not work.
      true  - The server will allow portals to send players to the Nether. */

  bool allow_nether;
  /* boolean false Enables remote access to the server console.
      enable-rcon,

  /* boolean false Enables GameSpy4 protocol server listener. Used to get
  information about server. */
  bool enable_query;

  /*(0-3) 1 Defines the difficulty (such as damage dealt by mobs and the way
     hunger and poison affects players) of the server. 0 - Peaceful 1 - Easy 2 -
     Normal 3 - Hard */
  int difficulty;

  /* integer (0-3) 0 Defines the mode of gameplay.
      Default: 0
      0 - Survival
      1 - Creative
      2 - Adventure
      3 - Spectator */
  int game_mode;

  /* The maximum height in which building is allowed.
  Terrain may still naturally generate above a low height limit. */
  int max_build_height;

  /* The maximum number of players that can play on the server at the same
     time. Note that if more players are on the server it will use more
     resources. Note also, op player connections are not supposed to count
     against the max players, but ops currently cannot join a full server.
     Extremely large values for this field result in the client-side user list
     being broken. Range  : 0-2147483647 Default: 20 */
  int max_players;

  /* integer (0–(2^63 - 1)) 60000 The maximum number of milliseconds a single
     tick may take before the server watchdog stops the server with the message,
     A single server tick took 60.00 seconds (should be max 0.05); Considering
     it to be crashed, server will forcibly shutdown. Once this criterion is
     met, it calls System.exit(1).
      -1 - disable watchdog entirely (this disable option was added in 14w32a)
   */
  int max_tick_time;

  /* integer (1-29999984) 29999984 This sets the maximum possible size in
     blocks, expressed as a radius, that the world border can obtain. Setting
     the world border bigger causes the commands to complete successfully but
     the actual border will not move past this block limit. Setting the
     max-world-size higher than the default doesn't appear to do anything.
      Examples:

      Setting max-world-size to 1000 will allow you to have a 2000x2000 world
     border. Setting max-world-size to 4000 will give you an 8000 x 8000 world
     border. */
  int max_world_size;

  /* By default it allows packets that are n-1 bytes big to go normally, but a
     packet that n bytes or more will be compressed down. So, lower number means
     more compression but compressing small amounts of bytes might actually end
     up with a larger result than what went in. Default: 256 -1 - disable
     compression entirely 0 - compress everything Note: The Ethernet spec
     requires that packets less than 64 bytes become padded to 64 bytes. Thus,
     setting a value lower than 64 may not be beneficial. It is also not
     recommended to exceed the MTU, typically 1500 bytes. */
  int network_compression_threshold;

  /* integer (1-4) 4 Sets the default permission level for ops when using /op.
     All levels inherit abilities and commands from levels before them. 1 - Ops
     can bypass spawn protection. 2 - Ops can use all singleplayer cheats
     commands (except /publish, as it is not on servers; along with /debug and
     /reload) and use command blocks. Command blocks, along with Realms
     owners/operators, have the same permissions as this level. 3 - Ops can use
     most multiplayer-exclusive commands, including /debug and commands that
     manage players (/ban, /op, etc). 4 - Ops can use all commands including
     /stop, /save-all, /save-on, /save-off, and /reload. */

  int op_permission_level;

  /* integer (1-65534) 25565 Changes the port the server is hosting (listening)
   * on. This port must be forwarded if the server is hosted in a network using
   * NAT (If you have a home router/firewall). */
  int server_port;

  /* Determines the radius of the spawn protection as (x*2)+1.
      Setting this to 0 will not disable spawn protection. 0 will protect the
     single block at the spawn point. 1 will protect a 3x3 area centered on the
     spawn point. 2 will protect 5x5, 3 will protect 7x7, etc. This option is
     not generated on the first server start and appears when the first player
     joins. If there are no ops set on the server, the spawn protection will be
     disabled automatically. Default: 16 */
  int spawn_protection;

  /* Sets the amount of world data the server sends the client, measured in
     chunks in each direction of the player (radius, not diameter). It
     determines the server-side viewing distance. (see Render distance) 10 is
     the default/recommended. If you have major lag, reduce this value. Range  :
     2-32 Default: 10    */
  int view_distance;

  /* The max numbers of players that can play on the server at the same time.
  Note that if more players are on the server it will use more resources. Note
  also, admin connections are not counted against the max players.
  Range: 0-256 */
  int max_players;

  /* integer (1-3)  The max number of connections the server will accept from
  the same IP.
  Note: If you try to set it higher than 3 it will be changed back to 3. */
  int max_connections;

  /*  integer 0 If non-zero, players are kicked from the server if they are
     idle for more than that many minutes. Note: Idle time is reset when the
     server receives one of the following packets: Click Window Enchant Item
      Update Sign
      Player Digging
      Player Block Placement
      Held Item Change
      Animation (swing arm)
      Entity Action
      Client Status
      Chat Message
      Use Entity */
  int player_idle_timeout;

  bool enable_command_block, /* boolean false Enables command blocks. */
      /* boolean false Force players to join in the default game mode.
          false - Players will join in the gamemode they left in.
          true - Players will always join in the default gamemode. */
      bool force_gamemode,

      /* boolean true Defines whether structures (such as villages) will be
         generated. false - Structures will not be generated in new chunks. true
         - Structures will be generated in new chunks.
          Note: Dungeons will still generate if this is set to false. */
      bool generate_structures;

  /* Whether or not the server will allow planted saplings to grow into trees.
      true - the server will allow saplings to grow into trees.
      false - the server will not allow saplings to grow into trees. */
  bool grow_trees;

  /* string blank The settings used to customize world generation.
      See Superflat and Customized for possible settings and examples. hardcore
     boolean false If set to true, players will be set to spectator mode if they
     die. */
  const char* generator_settings;

  /*  The "level-name" value will be used as the world name and its folder
     name. You may also copy your saved game folder here, and change the name to
     the same as that folder's to load it instead. Characters such as '
     (apostrophe) may need to be escaped by adding a backslash before them.
      Default: world */
  const char* level_name;

  /* Add a seed for your world, as in Singleplayer.
      Some examples are: minecraft, 404, 1a2b3c.
      Default: blank */
  const char* level_seed;

  /* string DEFAULT Determines the type of map that is generated.
      DEFAULT - Standard world with hills, valleys, water, etc.
      FLAT - A flat world with no features, meant for building.
      LARGEBIOMES - Same as default but all biomes are larger.
      AMPLIFIED - Same as default but world-generation height limit is
     increased.
      CUSTOMIZED - Same as default unless generator-settings is set to a preset.
   */
  const char* level_type;

  /* string A Minecraft Server This is the message that is displayed in the
     server list of the client, below the name. The MOTD does support color and
     formatting codes. The MOTD supports special characters, such as "♥".
     However, such characters must be converted to escaped Unicode form. An
     online converter can be found here If the MOTD is over 59 characters, the
     server list will likely report a communication error. */
  const char* motd;

  /* integer (1-65534) 25565 Sets the port for the query server (see
   * enable-query). */
  const char* query_port;

  /* Sets the password to rcon.
      Default: blank    */
  const char* rcon_password;

  /* Sets the port to rcon.
      Range   : 1-65534
      Default : 25575
  */
  const char* rcon_port;

  /* string DEFAULT Determines the type of map that is generated.
      DEFAULT - Standard world with hills, valleys, water, etc.
      FLAT - A flat world with no features, meant for building.
      LARGEBIOMES - Same as default but all biomes are larger.
      AMPLIFIED - Same as default but world-generation height limit is
     increased.
      CUSTOMIZED - Same as default unless generator-settings is set to a preset.
   */

  /* string blank Optional URI to a resource pack. The player may choose to use
   * it. */
  const char* resource_pack;

  const char
      *resource_pack_sha1, /* string blank Optional SHA-1 digest of the resource
                              pack, in lowercase hexadecimal. It's recommended
                              to specify this. This is not yet used to verify
                              the integrity of the resource pack, but improves
                              the effectiveness and reliability of caching. */

      /* Set this if you want the server to bind to a particular IP.
          It is strongly recommended that you leave server-ip blank!
          Set to blank, or the IP you want your server to run (listen) on. */
      const char *server_ip;

  const char* server_name; /* string  The name of the server. This is displayed
      in the server list and when someone is joining the server
      A single line of text. Characters such as ' (apostrophe) may need to be
      escaped by adding a backslash before them. */

  /* string  MOTD is short for Message of the day, though you do not need to
   * change it every day. The MOTD is displayed when people join the server.*/
  const char* motd;

 private:
};

}  // namespace typecraft
#endif  //< TYPECRAFT_SERVER_H
