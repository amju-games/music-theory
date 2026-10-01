#include "DeviceChangeEvent.h"
#include "GSBase.h"

namespace Amju
{
void DeviceChangeMsg::Execute()
{
  GameState* gs = TheGame::Instance()->GetState();
  GSBase* gsb = dynamic_cast<GSBase*>(gs);
  if (gsb)
  {
    gsb->OnDeviceChangeEvent(m_event);
  }
}
}

