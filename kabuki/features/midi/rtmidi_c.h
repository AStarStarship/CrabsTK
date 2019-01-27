
#include <stdbool.h>

#ifndef RTMIDI_C_H
#define RTMIDI_C_H

#if defined(RTMIDI_EXPORT)
#define RTMIDIAPI __declspec(dllexport)
#else
#define RTMIDIAPI //__declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void* RtMidiPtr;
typedef void* RtMidiInPtr;
typedef void* RtMidiOutPtr;

  enum RtMidiApi {
    RT_MIDI_API_UNSPECIFIED,    /*< Search for a working compiled SDK. */
    RT_MIDI_API_MACOSX_CORE,    /*< Macintosh OS-X Core Midi SDK. */
    RT_MIDI_API_LINUX_ALSA,     /*< The Advanced Linux Sound Architecture SDK. */
    RT_MIDI_API_UNIX_JACK,      /*< The Jack Low-Latency MIDI Server SDK. */
    RT_MIDI_API_WINDOWS_MM,     /*< The Microsoft Multimedia MIDI SDK. */
    RT_MIDI_API_WINDOWS_KS,     /*< The Microsoft Kernel Streaming MIDI SDK. */
    RT_MIDI_API_RTMIDI_DUMMY    /*< A compilable but non-functional SDK. */
  };

enum RtMidiErrorType {
  RT_ERROR_WARNING, RT_ERROR_DEBUG_WARNING, RT_ERROR_UNSPECIFIED, RT_ERROR_NO_DEVICES_FOUND,
  RT_ERROR_INVALID_DEVICE, RT_ERROR_MEMORY_ERROR, RT_ERROR_INVALID_PARAMETER, RT_ERROR_INVALID_USE,
  RT_ERROR_DRIVER_ERROR, RT_ERROR_SYSTEM_ERROR, RT_ERROR_THREAD_ERROR
};

typedef void(* RtMidiCCallback) (double timeStamp, const unsigned CH1* message, void *userData);

RTMIDIAPI SI4 rtmidi_sizeof_rtmidi_api ();

/* RtMidi SDK */
RTMIDIAPI SI4 rtmidi_get_compiled_api (enum RtMidiApi **apis); // return length for NULL argument.
RTMIDIAPI void rtmidi_error (enum RtMidiErrorType type, const CH1* errorString);

RTMIDIAPI void rtmidi_open_port (RtMidiPtr device, unsigned SI4 portNumber, const CH1 *portName);
RTMIDIAPI void rtmidi_open_virtual_port (RtMidiPtr device, const CH1 *portName);
RTMIDIAPI void rtmidi_close_port (RtMidiPtr device);
RTMIDIAPI unsigned SI4 rtmidi_get_port_count (RtMidiPtr device);
RTMIDIAPI const CH1* rtmidi_get_port_name (RtMidiPtr device, unsigned SI4 portNumber);

/* RtMidiIn SDK */
RTMIDIAPI RtMidiInPtr rtmidi_in_create_default ();
RTMIDIAPI RtMidiInPtr rtmidi_in_create (enum RtMidiApi api, const CH1 *clientName, unsigned SI4 queueSizeLimit);
RTMIDIAPI void rtmidi_in_free (RtMidiInPtr device);
RTMIDIAPI enum RtMidiApi rtmidi_in_get_current_api (RtMidiPtr device);
RTMIDIAPI void rtmidi_in_set_callback (RtMidiInPtr device, RtMidiCCallback callback, void *userData);
RTMIDIAPI void rtmidi_in_cancel_callback (RtMidiInPtr device);
RTMIDIAPI void rtmidi_in_ignore_types (RtMidiInPtr device, BOL midiSysex, BOL midiTime, BOL midiSense);
RTMIDIAPI double rtmidi_in_get_message (RtMidiInPtr device, unsigned CH1 **message);

/* RtMidiOut SDK */
RTMIDIAPI RtMidiOutPtr rtmidi_out_create_default ();
RTMIDIAPI RtMidiOutPtr rtmidi_out_create (enum RtMidiApi api, const CH1 *clientName);
RTMIDIAPI void rtmidi_out_free (RtMidiOutPtr device);
RTMIDIAPI enum RtMidiApi rtmidi_out_get_current_api (RtMidiPtr device);
RTMIDIAPI SI4 rtmidi_out_send_message (RtMidiOutPtr device, const unsigned CH1 *message, SI4 length);


#ifdef __cplusplus
}
#endif
#endif
