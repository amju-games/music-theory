// * Amjula music theory *
// (c) Copyright 2018 Jason Colman

#include <ConfigFile.h>
#include "Consts.h"
#include "NetRequest.h"
#include "SerialReqManager.h"
#include "Version.h"

namespace Amju
{
NetSendDeviceInfoReq::NetSendDeviceInfoReq(
  const std::string& url,
  HttpClient::HttpMethod method,
  const std::string& name,
  const std::string& deviceId_,
  const std::string& deviceUserName_,
  const std::string& deviceModel_,
  const std::string& deviceOsVersion_) :
  OnlineReq(url, method, name),
  m_deviceId(deviceId_),
  m_deviceUserName(deviceUserName_),
  m_deviceModel(deviceModel_),
  m_deviceOsVersion(deviceOsVersion_) {}
 
void NetSendDeviceInfoReq::HandleResult()
{
  HttpResult res = GetResult();
  if (res.GetSuccess())
  {
    // We got a response of some kind. That doesn't mean it succeeded though!
    // Check the HTTP response code.
    int httpCode = res.GetHttpResponseCode();
    std::cout << "Net send: got response for request '" << GetName() << "': HTTP response code " << httpCode << "\n";
    if (httpCode == 200) // i.e. OK
    {
      // We can save device info to config file, which means we won't send again
      //  (unless we need to update, e.g. OS version changes)
      //#ifdef LOG_SUCCESSFUL_REQ
      std::cout << "Request '" << GetName() << "' success!\n";
      //#endif

      // Now we have successfully logged this on the server, it's OK to cache it locally so we don't send it
      //  again. If this request fails, we won't cache the info and will attempt to resend next time.
      GameConfigFile* gcf = TheGameConfigFile::Instance();
      gcf->Set(DEVICE_ID, m_deviceId);
      gcf->Set(DEVICE_OS_VERSION, m_deviceOsVersion);
      gcf->Set(DEVICE_USER_NAME, m_deviceUserName);
      gcf->Set(CLIENT_VERSION, GetVersionStr());
      gcf->Save();
    }
  }
  else
  {
    std::cout << "Request '" << GetName() << "' FAILED!\n";
  }
}

void NetSendReq::HandleResult()
{
  HttpResult res = GetResult();
  if (res.GetSuccess())
  {
//#ifdef LOG_SUCCESSFUL_REQ
    std::cout << "Request '" << GetName() << "' success!\n";
//#endif
  }
  else
  {
    std::cout << "Request '" << GetName() << "' FAILED!\n";
  }
}
 
}


