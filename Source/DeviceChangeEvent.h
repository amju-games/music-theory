// * Amju PIANO FEST *
// (c) Copyright Juliet Colman 2000-2026

#pragma once

#include <MessageQueue.h>

namespace Amju
{
// Device Change Event
// Fired off when we detect that a device has been added or removed.
// When we get one of these, we recheck our MIDI devices.
struct DeviceChangeEvent
{
  // TODO Add any useful info here 
};

// Message wrapper around a DeviceChangeEvent so we can queue them
//  in the message queue.
class DeviceChangeMsg : public Message
{
public:
  DeviceChangeMsg(DeviceChangeEvent dce) : m_event(dce) {}
  void Execute() override;

  DeviceChangeEvent m_event;
};
}
