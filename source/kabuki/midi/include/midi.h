/** Kabuki Starship
    @file    /.../Source/_Theater/MIDI/MIDI.h
    @author  
    @license 

        Licensed under the Apache License, Version 2.0  (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

// Notice: This code originally came from an open-source Windows MIDI project, but is getting completely changed.

#pragma once

#include <KabukiTheater-Config.h>

namespace _Theater { namespace MIDI {
    
/*
    Note: This seciton is here for referance only. These structs in in the Win.h header.
        This code is not a part of Symmetry Station and belongs to Microsoft.
*/


typedef struct MIDIEvent
{
    uint32_t dwDeltaTime;
    //< Time  (in MIDI ticks) between the previous event and the current event.
    uint32_t dwStreamID;   //< Reserved. Must be zero.
    uint32_t dwEvent;      //< Event code and event parameters or length.
                        /**< To parse this information, use the MEVT_EVENTTYPE and MEVT_EVENTPARM macros. */

    uint32_t dwParms[];    //< Event parameters.
                        /**< If dwEvent specifies MEVT_F_LONG and the length of the buffer, this member contains
                             parameters for the event. This parameter data must be padded with zeros so that an
                             integral number of uint32_t values are stored. For example, if the event data is five
                             bytes long, three pad bytes must follow the data for a total of eight bytes. In this
                             case, the low 24 bits of dwEvent would contain the value 5. If dwEvent specifies
                             MEVT_F_int16_t, do not use this member in the stream buffer. */
};

typedef struct MIDIEvent MIDIEVENT;

struct MIDIHeader
{
    LPSTR       lpData;          //< Pointer to MIDI data.
    uint32_t    dwBufferLength;  //< Size of the buffer.
    uint32_t    dwBytesRecorded; //< Actual amount of data in the buffer.
                                        /**< This value should be less than or equal to the value given in the
                                             dwBufferLength member. */
    uint32_t*   dwUser;          //< Custom user data.
    uint32_t    dwFlags;         //< Flags giving information about the buffer.
    struct      midihdr_tag  *lpNext;        //< Reserved
                                        /**< Do not use. */
    uint32_t*   reserved;        //< Reserved
                                        /**< Do not use. */
    uint32_t    dwOffset;        //< Offset into the buffer when a callback is performed.
                                        /**< This callback is generated because the MEVT_F_CALLBACK flag is set
                                             in the dwEvent member of the MIDIEVENT structure. This offset
                                             enables an application to determine which event caused the callback. */
    uint32_t*   dwReserved[4];   //< Reserved
    
    virtual const char* op (_::Terminal* io, byte index);
    /*< Inter-process subroutines. */
                                        /**< Do not use. */
};

struct MIDIHeader MIDIHDR;  //*LPMIDIHDR


struct MIDIINCAPS
{
    uint16_t  wMid;                 //< Manufacturer identifier.
    uint16_t  wPid;                 //< Product identifier.
    MMVERSION vDriverVersion;       //< Version number of the device driver for the MIDI input device.
                                    //< MSB = { version number }, LSB = { minor version number }
    TCHAR  szPname[MAXPNAMELEN];    //< Null-terminated product name string.
    uint32_t     dwSupport;         //< Reserved. Must be zero.
    
    virtual const char* op (_::Terminal* io, byte index);
    /*< Inter-process subroutines. */
};




typedef uint32_t MMVERSION;
typedef int16_t TCHAR;

struct MIDIOutCaps
{
    uint16_t      wMid;                 //< Manufacturer identifier of the device driver for the MIDI output device.
                                    /**< Manufacturer identifiers are defined in Manufacturer and Product Identifiers. */
    uint16_t      wPid;                 //< Product identifier of the MIDI output device.
                                    /**< Product identifiers are defined in Manufacturer and Product Identifiers. */
    MMVERSION vDriverVersion;       //< Version number of the device driver for the MIDI output device.
                                    /**< The high-order byte is the major version number, and the low-order byte is
                                         the minor version number. */

    TCHAR     szPname[MAXPNAMELEN]; //< Product name in a null-terminated string.
    uint16_t      wTechnology;          //< Type of the MIDI output device.
    uint16_t      wVoices;              //< Number of voices supported by an internal synthesizer device.
                                    /**< If the device is a port, this member is not meaningful and is set to 0. */
    uint16_t      wNotes;               //< Maximum number of simultaneous notes that can be played by an internal synthesizer device.
                                    /**< If the device is a port, this member is not meaningful and is set to 0. */
    uint16_t      wChannelMask;         //< Channels that an internal synthesizer device responds to.
                                    /**< The least significant bit refers to channel 0 and the most significant bit to
                                         channel 15. Port devices that transmit on all channels set this member to 0xFFFF. */
    uint32_t     dwSupport;            //< Optional functionality supported by the device.

    virtual byte getState ();
    /*< Gets the Inter-process state. */
    
    virtual const char* setState  (byte Value);
    /*< Sets the Inter-process state.
        @return returns 0 upon success, and an I2P::Errror upon failure. */
    
    virtual const char* op (_::Terminal* io, byte index);
    /*< Inter-process subroutines. */
};



struct MIDIPropTempo
{
    uint32_t cbStruct;  /*< Length, in bytes, of this structure.
                            This member must be filled in for both the MIDIPROP_SET and MIDIPROP_GET operations of
                            the midiStreamProperty function. */
    uint32_t dwTempo;   /*< Tempo of the stream, in microseconds per quarter note.
                            The tempo is honored only if the time division for the stream is specified in quarter
                            note format. This member is set in a MIDIPROP_SET operation and is filled on return
                            from a MIDIPROP_GET operation. */
    
    virtual const char* op (_::Terminal* io, byte index);
    /*< Inter-process subroutines. */
};

typedef MIDIPropTempo MIDIPROPTEMPO;

struct MIDIPropTimeDiv
    uint32_t cbStruct;
    /*< Length, in bytes, of this structure.
        This member must be filled in for both the MIDIPROP_SET and MIDIPROP_GET operations
        of the midiStreamProperty function. */
    uint32_t dwTimeDiv;
    /*< Time division for this stream.
        Format specified in the Standard MIDI Files 1.0 specification. The low 16 bits of this
        uint32_t value contain the time division. This member is set in a MIDIPROP_SET operation
        and is filled on return from a MIDIPROP_GET operation. */

    virtual byte getState ();
    /*< Gets the Inter-process state. */
    
    virtual const char* setState  (byte Value);
    /*< Sets the Inter-process state.
        @return returns 0 upon success, and an I2P::Errror upon failure. */
    
    virtual const char* op (_::Terminal* io, byte index);
    /*< Inter-process subroutines. */
};
typedef struct MIDIPropTimeDiv MIDIPROPTIMEDIV;


struct MIDIStrMBufVer
{
    uint32_t dwVersion;
    /*< Version of the stream.
        The high 16 bits contain the major version, and the low 16 bits contain the minor
        version. The version number for the first implementation of MIDI streams should be 1.0. */

    uint32_t dwMid;
    /*< Manufacturer identifier.
        Manufacturer identifiers are defined in Manufacturer and Product Identifiers. */

    uint32_t dwOEMVersion;
    /*< OEM version of the stream.
        Original equipment manufacturers can use this field to version-stamp any custom events
        they have specified. If a custom event is specified, it must be the first event sent
        after the stream is opened. */
    
    virtual const char* op (_::Terminal* io, byte index);
    /*< Inter-process subroutines. */
};

typedef struct MIDISTRMBUFFVER MIDIStrMBufVer;

enum MMRESULT
{
  MMSYSERR_NOERROR        = 0,
  MMSYSERR_ERROR          = 1,
  MMSYSERR_BADDEVICEID    = 2,
  MMSYSERR_NOTENABLED     = 3,
  MMSYSERR_ALLOCATED      = 4,
  MMSYSERR_INVALHANDLE    = 5,
  MMSYSERR_NODRIVER       = 6,
  MMSYSERR_NOMEM          = 7,
  MMSYSERR_NOTSUPPORTED   = 8,
  MMSYSERR_BADERRNUM      = 9,
  MMSYSERR_INVALFLAG      = 10,
  MMSYSERR_INVALPARAM     = 11,
  MMSYSERR_HANDLEBUSY     = 12,
  MMSYSERR_INVALIDALIAS   = 13,
  MMSYSERR_BADDB          = 14,
  MMSYSERR_KEYNOTFOUND    = 15,
  MMSYSERR_READERROR      = 16,
  MMSYSERR_WRITEERROR     = 17,
  MMSYSERR_DELETEERROR    = 18,
  MMSYSERR_VALNOTFOUND    = 19,
  MMSYSERR_NODRIVERCB     = 20,
  WAVERR_BADFORMAT        = 32,
  WAVERR_STILLPLAYING     = 33,
  WAVERR_UNPREPARED       = 34
};



/*

    Common Return codes

    MMSYSERR_NOERROR  (0)
        The function completed normally.

    MMSYSERR_BADDEVICEID  (2)
        An out-of-range device ID was specified.

    MMSYSERR_ALLOCATED  (4)
        Device is already allocated and is not available.

    MMSYSERR_INVALHANDLE  (5)
        An illegal device handle was specified.

    MMSYSERR_NODRIVER  (6)
        No device driver is present for this device.

    MMSYSERR_NOMEM  (7)
        Driver memory allocation error.

    MIDIERR_UNPREPARED  (64)
        Header is not prepared.

    MIDIERR_STILLPLAYING  (65)
        Attempt to close device while still playing.

    MIDIERR_NOTREADY  (67)
        Hardware is busy.

MIDI Input Functions

    UINT midiInGetNumDevs ();
        Gets the number of input devices available on the system.

    UINT midiInGetDevCaps  (UINT wDeviceID, LPMIDIINCAPS lpCaps, UINT wSize);
        Fills the MIDIINCAPS structure, pointed to by lpCaps, with the capabilities for the specified device.

    UINT midiInOpen  (LPHMIDIIN lphMIDIIn, UINT wDeviceID, uint32_t dwCallback, uint32_t dwCallbackInstance, uint32_t dwFlags);
        Opens the specified device for input. A handle to the device is returned in lphMIDIIn. dwFlags specifies the type of callback that is used to alert the application to events. Common callback types are CALLBACK_WINDOW and CALLBACK_FUNCTION. dwCallback specifies the callback function address or window handle, while dwCallbackInstance specifies user data that are passed to callback functions  (but not to window callbacks).

    UINT midiInClose  (HMIDIIn hMIDIIn);
        Closes the specified input device.

    UINT midiInPrepareHeader  (HMIDIIn hMIDIIn, LPMIDIHDR lpMIDIInHdr, UINT wSize);
        Prepares a header, normally used for sysex messages, for processing by the device. The header structure and data buffer  (lpData field of MIDIHDR structure) must be allocated using GlobalAlloc () using the GMEM_MOVEABLE and GMEM_SHARE flags.

    UINT midiInUnprepareHeader  (HMIDIIn hMIDIIn, LPMIDIHDR lpMIDIInHdr, UINT wSize);
        Cleans up the preparation done by a previous call to midiInPrepareHeader ().

    UINT midiInAddBuffer  (HMIDIIn hMIDIIn, LPMIDIHDR lpMIDIInHdr, UINT wSize);
        Sends the buffer managed by the specified MIDIHDR structure to the device. Received sysex data are stored in the buffer which is returned to the application when filled.

    UINT midiInStart  (HMIDIIn hMIDIIn);
        Enables MIDI input from the already-opened device.

    UINT midiInStop  (HMIDIIn hMIDIIn);
        Stops MIDI input from the device.

    UINT midiInReset  (HMIDIIn hMIDIIn);
        Flushes any pending input buffers and stops input.


MIDI Output Functions

    UINT midiOutGetNumDevs ();
        Gets the number of output devices available on the system.

    UINT midiOutGetDevCaps  (UINT wDeviceID, LPMIDIOUTCAPS lpCaps, UINT wSize);
        Fills the MIDIOutCaps structure, pointed to by lpCaps, with the capabilities for the specified device.

    UINT midiOutOpen  (LPHMIDIOUT lphMIDIOut, UINT wDeviceID, uint32_t dwCallback, uint32_t dwCallbackInstance, uint32_t dwFlags);
        Opens the specified device for output. A handle to the device is returned in lphMIDIOut. dwFlags specifies the type of callback that is used to alert the application. Common callback types are CALLBACK_WINDOW and CALLBACK_FUNCTION. dwCallback specifies the callback function address or window handle, while dwCallbackInstance specifies user data that are passed to callback functions  (but not to window callbacks).

    UINT midiOutClose  (HMIDIOut hMIDIOut);
        Closes the specified output device.

    UINT midiOutint16_tMsg  (HMIDIOut hMIDIOut, uint32_t dwMsg);
        Sends a int16_t  (non-sysex) message to the specified device. The message is packed into a uint32_t, with the first byte of the message in the least significant byte.

    UINT midiOutLongMsg  (HMIDIOut hMIDIOut, LPMIDIHDR lpMIDIOutHdr, UINT wSize);
        Sends a buffer of data, generally part or all of a sysex, to the specified device. The MIDIHDR structure must be properly initialized and prepared before calling this function.

    UINT midiOutPrepareHeader  (HMIDIOut hMIDIOut, LPMIDIHDR lpMIDIOutHdr, UINT wSize);
        Prepares a header, normally used for sysex messages, for processing by the device. The header structure and data buffer  (lpData field of MIDIHDR structure) must be allocated using GlobalAlloc () using the GMEM_MOVEABLE and GMEM_SHARE flags.

    UINT midiOutUnprepareHeader  (HMIDIOut hMIDIOut, LPMIDIHDR lpMIDIOutHdr, UINT wSize);
        Cleans up the preparation done by a previous call to midiOutPrepareHeader ().

    UINT midiOutReset  (HMIDIOut hMIDIOut);
        Flushes any pending output buffers and, with some drivers, also sends Note Off messages for all notes on all channels.
 */
}   //< MIDI
}   //< _Theater
