# Chinese Room Abstract Stack Machine (Crabs) API Overview

All Kabuki Toolkit (KT) modules are broken up into seams indexed with contiguous positive integers. The unit testing order is identical to the seam index.

# Minor Seams

## SEAM_1
SEAM_1 is a working minimum viable product of the Chinese Room Abstract Stack Machine (Crabs), a light-weight VM core you may add to your application to control it with Script via TCP and UART.

### Modules

1. crabs
2. kabuki
    1. serial
    2. web


## SEAM_2

SEAM_2 is a working MIDI controller firmware and software that runs over UART and TCP.

### Modules

1. kabuki
    1. tek
    2. hmi

## SEAM_3

SEAM_3 is a functioning application framework built on top of kabuki::hmi, SDL, NanoVG, MyGUI, and NanoGUI. Demo app is a digital mixer.

### Modules

1 projects
    1. Digital Mixer
2. kabuki
    1. app
    2. mg
    3. gl
    4. gui

## SEAM_4

SEAM_4 is a further continuation of the Abstract Mixer from SEAM_3 to add audio and MIDI functionality.

### Modules

1. projects
    1. Digital Mixer
2. kabuki
    1. al
    2. midi

## SEAM_5

SEAM_5 adds a simple project manager with contact list and file compression.

### Modules

1. projects
    1. Kabuki Project
2. kabuki
    1. project
    2. id
    3. data
    4. file

## SEAM_6

SEAM_6 adds a generic store and inventory system that can be used for in-app purchases, web stores, game inventory, etc.

### Modules

1. projects
    1. Kabuki Store Server
    2. Kabuki Store Client
2. kabuki
    1. store

## SEAM_7

SEAM_7 adds The Sloth, a tool for downloading and refactoring SDKs into a common style guide. At this point all the 3rd party libraries shall get Slothed.

### Modules

1. projects
    1. The Sloth
2. kabuki
    1. sloth

## SEAM_8

SEAM_8 adds computer vision and a video player.

### Modules

1. projects
    1. Kabuki Cam
    2. Kabuki Player
2. kabuki
    1. cv
    2. video

