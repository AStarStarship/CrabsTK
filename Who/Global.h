// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_WHO_GLOBAL_H
#define CRABSTK_WHO_GLOBAL_H
/* Global Who configuration values. Override before including _Config.h or
@link _Seams.h to tune the Who API for your deployment. */
#include "_Config.h"
/* Default max sessions the ASessionTable preallocates. */
#ifndef CRABSTK_WHO_SESSION_TOTAL_DEFAULT
#define CRABSTK_WHO_SESSION_TOTAL_DEFAULT 1024
#endif
/* Default capacity of an ARoom before it must grow. */
#ifndef CRABSTK_WHO_ROOM_TOTAL_DEFAULT
#define CRABSTK_WHO_ROOM_TOTAL_DEFAULT 64
#endif
/* Default max length of a chat handle (bytes, UTF-8). */
#ifndef CRABSTK_WHO_HANDLE_LENGTH_MAX
#define CRABSTK_WHO_HANDLE_LENGTH_MAX 255
#endif
/* Default max length of a chat message (bytes, UTF-8). */
#ifndef CRABSTK_WHO_MESSAGE_LENGTH_MAX
#define CRABSTK_WHO_MESSAGE_LENGTH_MAX 512
#endif
/* Session table shrink threshold: when occupancy drops below half the
total, the next prune reclaims half the total. */
#ifndef CRABSTK_WHO_SESSION_SHRINK_FACTOR
#define CRABSTK_WHO_SESSION_SHRINK_FACTOR 2
#endif
#endif
