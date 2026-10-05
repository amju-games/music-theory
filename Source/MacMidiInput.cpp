#include <iostream>
#include <CoreFoundation/CoreFoundation.h>
#include <MessageQueue.h>
#include "DeviceChangeEvent.h"
#include "MacMidiInput.h"
#include "MusicEvent.h"

namespace Amju
{
static void OnNoteOff(
  unsigned char channel, unsigned char pitch, unsigned char velocity)
{
  TheMessageQueue::Instance()->Add(
    new MusicKbMsg(MusicKbEvent(pitch, 0)));
}

static void OnNoteOn(
  unsigned char channel, unsigned char pitch, unsigned char velocity)
{
  TheMessageQueue::Instance()->Add(
    new MusicKbMsg(MusicKbEvent(pitch, velocity)));
}

MacMidiInput::MacMidiInput() = default;

MacMidiInput::~MacMidiInput()
{
    DisconnectAll();

    if (m_inputPort != 0)
    {
        MIDIPortDispose(m_inputPort);
        m_inputPort = 0;
    }

    if (m_client != 0)
    {
        MIDIClientDispose(m_client);
        m_client = 0;
    }
}

int MacMidiInput::GetNumConnections() const
{
    return static_cast<int>(m_connectedSources.size());
}

bool MacMidiInput::Connect()
{
    OSStatus status = noErr;

    // 1. Create MIDI Client with a notification callback for device changes
    if (m_client == 0)
    {
        CFStringRef clientName = CFSTR("Amju MIDI Client");
        status = MIDIClientCreate(clientName, NotifyProc, this, &m_client);
        if (status != noErr)
        {
            std::cerr << "Failed to create MIDI client. Status: " << status << std::endl;
            return false;
        }
    }

    // 2. Create Input Port with a read callback for incoming MIDI messages
    if (m_inputPort == 0)
    {
        CFStringRef portName = CFSTR("Amju MIDI Input Port");
        status = MIDIInputPortCreate(m_client, portName, ReadProc, this, &m_inputPort);
        if (status != noErr)
        {
            std::cerr << "Failed to create MIDI input port. Status: " << status << std::endl;
            return false;
        }
    }

    // 3. Connect to all currently available MIDI sources
    OnDeviceChange();

    return true;
}

void MacMidiInput::OnDeviceChange()
{
    // Disconnect existing sources before rescanning
    DisconnectAll();

    ItemCount numSources = MIDIGetNumberOfSources();
    for (ItemCount i = 0; i < numSources; ++i)
    {
        MIDIEndpointRef source = MIDIGetSource(i);
        if (source != 0)
        {
            OSStatus status = MIDIPortConnectSource(m_inputPort, source, nullptr);
            if (status == noErr)
            {
                m_connectedSources.push_back(source);
            }
        }
    }
}

void MacMidiInput::DisconnectAll()
{
    for (MIDIEndpointRef source : m_connectedSources)
    {
        if (m_inputPort != 0 && source != 0)
        {
            MIDIPortDisconnectSource(m_inputPort, source);
        }
    }
    m_connectedSources.clear();
}

void MacMidiInput::ReadProc(const MIDIPacketList *pktlist, void *readProcRefCon, void *srcConnRefCon)
{
    const MIDIPacket *packet = &pktlist->packet[0];

    for (UInt32 p = 0; p < pktlist->numPackets; ++p)
    {
        UInt16 i = 0;
        while (i < packet->length)
        {
            unsigned char status = packet->data[i];

            // Status bytes always have the high bit set (>= 0x80)
            if (status & 0x80)
            {
                unsigned char messageType = status & 0xF0; // High nibble (command)
                unsigned char channel     = status & 0x0F; // Low nibble (0-15)

                if (messageType == 0x90 && (i + 2 < packet->length)) // Note On
                {
                    unsigned char pitch    = packet->data[i + 1];
                    unsigned char velocity = packet->data[i + 2];

                    if (velocity == 0)
                    {
                        // Handled as Note Off per MIDI spec
                        OnNoteOff(channel, pitch, velocity);
                    }
                    else
                    {
                        OnNoteOn(channel, pitch, velocity);
                    }

                    i += 3; // Advance past status, pitch, velocity
                    continue;
                }
                else if (messageType == 0x80 && (i + 2 < packet->length)) // Note Off
                {
                    unsigned char pitch    = packet->data[i + 1];
                    unsigned char velocity = packet->data[i + 2];

                    OnNoteOff(channel, pitch, velocity);

                    i += 3; // Advance past status, pitch, velocity
                    continue;
                }
            }

            // Move to next byte for unhandled or 1/2-byte status messages
            i++;
        }

        packet = MIDIPacketNext(packet);
    }
}

void MacMidiInput::NotifyProc(const MIDINotification *message, void *refCon)
{
    auto* self = static_cast<MacMidiInput*>(refCon);
    if (!self) return;

    // kMIDIMsgSetupChanged is sent when a MIDI device is plugged in or removed
    if (message->messageID == kMIDIMsgSetupChanged)
    {
        TheMessageQueue::Instance()->Add(new DeviceChangeMsg({}));
    }
}
} // namespace Amju

