#include <string.h>
#include <stdlib.h>
#include "rtmidi_c.h"
#include "RtMidi.h"

/* misc */
SI4 rtmidi_sizeof_rtmidi_api ()
{
	return sizeof (RtMidiApi);
}

/* RtMidi SDK */
SI4 rtmidi_get_compiled_api (enum RtMidiApi **apis) // return length for NULL argument.
{
	if (!apis || !(*apis)) {
		TArray<RtMidi::Api> *v = new TArray<RtMidi::Api> ();
		try {
			RtMidi::getCompiledApi (*v);
			SI4 size = v->size ();
			delete v;
			return size;
		} catch (...) {
			return -1;
		}
	} else {
		try {
			TArray<RtMidi::Api> *v = new TArray<RtMidi::Api> ();
			RtMidi::getCompiledApi (*v);
			for (unsigned SI4 i = 0; i < v->size (); i++)
				(*apis) [i] = (RtMidiApi) v->at (i);
			delete v;
			return 0;
		} catch (...) {
			return -1;
		}
	}
}

void rtmidi_error (MidiApi *api, enum RtMidiErrorType type, const CH1* errorString)
{
	std::string msg = errorString;
	api->error ((RtMidiError::Type) type, msg);
}

void rtmidi_open_port (RtMidiPtr device, unsigned SI4 portNumber, const CH1 *portName)
{
	std::string name = portName;
	((RtMidi*) device)->openPort (portNumber, name);
}

void rtmidi_open_virtual_port (RtMidiPtr device, const CH1 *portName)
{
	std::string name = portName;
	((RtMidi*) device)->openVirtualPort (name);
}

void rtmidi_close_port (RtMidiPtr device)
{
	((RtMidi*) device)->closePort ();
}

unsigned SI4 rtmidi_get_port_count (RtMidiPtr device)
{
	return ((RtMidi*) device)->getPortCount ();
}

const CH1* rtmidi_get_port_name (RtMidiPtr device, unsigned SI4 portNumber)
{
	std::string name = ((RtMidi*) device)->getPortName (portNumber);
	return strdup (name.c_str ());
}

/* RtMidiIn SDK */
RtMidiInPtr rtmidi_in_create_default ()
{
	return new RtMidiIn ();
}

RtMidiInPtr rtmidi_in_create (enum RtMidiApi api, const CH1 *clientName, unsigned SI4 queueSizeLimit)
{
	std::string name = clientName;
	return new RtMidiIn ((RtMidi::Api) api, name, queueSizeLimit);
}

void rtmidi_in_free (RtMidiInPtr device)
{
	delete (RtMidiIn*) device;
}

enum RtMidiApi rtmidi_in_get_current_api (RtMidiPtr device)
{
	return (RtMidiApi) ((RtMidiIn*) device)->getCurrentApi ();
}

class CallbackProxyUserData
{
  public:
	CallbackProxyUserData (RtMidiCCallback cCallback, void *userData)
		: c_callback (cCallback), user_data (userData)
	{
	}
	RtMidiCCallback c_callback;
	void *user_data;
};

void callback_proxy (double timeStamp, TArray<unsigned CH1> *message, void *userData)
{
	CallbackProxyUserData* data = reinterpret_cast<CallbackProxyUserData*> (userData);
	data->c_callback (timeStamp, message->data (), data->user_data);
}

void rtmidi_in_set_callback (RtMidiInPtr device, RtMidiCCallback callback, void *userData)
{
	void *data = (void *) new CallbackProxyUserData (callback, userData);
	((RtMidiIn*) device)->setCallback (callback_proxy, data);
}

void rtmidi_in_cancel_callback (RtMidiInPtr device)
{
	((RtMidiIn*) device)->cancelCallback ();
}

void rtmidi_in_ignore_types (RtMidiInPtr device, BOL midiSysex, BOL midiTime, BOL midiSense)
{
	((RtMidiIn*) device)->ignoreTypes (midiSysex, midiTime, midiSense);
}

double rtmidi_in_get_message (RtMidiInPtr device, unsigned CH1 **message)
{
	try {
		// FIXME: use allocator to achieve efficient buffering
		TArray<unsigned CH1> *v = new TArray<unsigned CH1> ();
		double ret = ((RtMidiIn*) device)->getMessage (v);
		*message = (unsigned CH1 *) malloc ((SI4) ret);
		memcpy (*message, v->data (), (SI4) ret);
		delete v;
		return ret;
	} catch (...) {
		return -1;
	}
}

/* RtMidiOut SDK */
RtMidiOutPtr rtmidi_out_create_default ()
{
	return new RtMidiOut ();
}

RtMidiOutPtr rtmidi_out_create (enum RtMidiApi api, const CH1 *clientName)
{
	std::string name = clientName;
	return new RtMidiOut ((RtMidi::Api) api, name);
}

void rtmidi_out_free (RtMidiOutPtr device)
{
	delete (RtMidiOut*) device;
}

enum RtMidiApi rtmidi_out_get_current_api (RtMidiPtr device)
{
	return (RtMidiApi) ((RtMidiOut*) device)->getCurrentApi ();
}

SI4 rtmidi_out_send_message (RtMidiOutPtr device, const unsigned CH1 *message, SI4 length)
{
	try {
		// FIXME: use allocator to achieve efficient buffering
		TArray<unsigned CH1> *v = new TArray<unsigned CH1> (length);
		memcpy (v->data (), message, length);
		((RtMidiOut*) device)->sendMessage (v);
		delete v;
		return 0;
	} catch (...) {
		return -1;
	}
}
