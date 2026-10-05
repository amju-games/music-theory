// * Amju PIANO FEST *
// (c) Copyright Juliet Colman 2000-2026

#pragma once

#include "MidiInput.h"
#include <CoreMIDI/CoreMIDI.h>
#include <vector>

namespace Amju
{
class MacMidiInput : public MidiInput
{
public:
    MacMidiInput();
    virtual ~MacMidiInput();

    int GetNumConnections() const override;
    bool Connect() override;
    void OnDeviceChange() override;

private:
    MIDIClientRef m_client = 0;
    MIDIPortRef m_inputPort = 0;
    std::vector<MIDIEndpointRef> m_connectedSources;

    static void ReadProc(const MIDIPacketList *pktlist, void *readProcRefCon, void *srcConnRefCon);
    static void NotifyProc(const MIDINotification *message, void *refCon);
    
    void DisconnectAll();
};
}

