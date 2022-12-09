// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <iostream>
#include <string>
#include <AmjuTime.h>
#include <ConfigFile.h>
#include <iOSUtils.h>
#include <UrlUtils.h>
#include "Consts.h"
#include "NetSend.h"
#include "SerialReqManager.h"
#include "Version.h"

namespace Amju
{
static const std::string URL_ROOT = "http://www.amju.com/cgi-bin/amt/";
static const std::string LOG_DEVICE_SCRIPT_NAME = "amt_log_device.pl";
static const std::string LOG_SESSION_SCRIPT_NAME = "amt_log_play_session.pl";

std::string DeviceManufacturer()
{
#if defined(AMJU_IOS) || defined(MACOSX)
  return "Apple";
#elif defined(WIN32)
  return "Windows";
#elif defined(GEKKO)
  return "Nintendo";
#else
  return "Unknown";
#endif
}
  
class NetSendDeviceInfoReq : public OnlineReq
{
public:
  NetSendDeviceInfoReq(
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
  
  virtual void HandleResult()
  {
    HttpResult res = GetResult();
    if (res.GetSuccess())
    {
      // We got a response of some kind. That doesn't mean it succeeded though!
      // Check the HTTP response code.
      int httpCode = res.GetHttpResponseCode();
      std::cout << "HTTP response code " << httpCode << "\n";
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

private:
  std::string m_deviceId;
  std::string m_deviceUserName;
  std::string m_deviceModel;
  std::string m_deviceOsVersion;
};
  
class NetSendReq : public OnlineReq
{
public:
#ifdef WIN32
    // Can't inherit ctor in MSVC 2013 ?
  NetSendReq(
    const std::string& url,
    HttpClient::HttpMethod method,
    const std::string& name) : OnlineReq(url, method, name) {}
#else
  using OnlineReq::OnlineReq;
#endif

  virtual void HandleResult()
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
};
  
static bool NetSendDeviceInfo(
  const std::string& deviceId,
  const std::string& deviceUserName,
  const std::string& deviceModel,
  const std::string& deviceOsVersion)
{
  std::string url = URL_ROOT +
    LOG_DEVICE_SCRIPT_NAME + 
    "?"
    "device_id='" + EncodeStr(deviceId) + "'&"
    "device_user_name='" + EncodeStr(deviceUserName) + "'&"
    "device_model='" + EncodeStr(deviceModel) + "'&"
    "device_os_version='" + EncodeStr(deviceOsVersion) + "'&"
    "device_client_version='" + EncodeStr(GetVersionStr()) + "'&"
    "device_manufacturer='" + EncodeStr(DeviceManufacturer()) + "'";

std::cout << "Sending device info: " << url << "\n";
  
  auto req = new NetSendDeviceInfoReq(url, HttpClient::GET, "send device info",
    deviceId, deviceUserName, deviceModel, deviceOsVersion);
  bool b = TheSerialReqManager::Instance()->AddReq(req);
  return b;
}
  
bool NetSendDeviceInfoFirstRunEver()
{
  // First run: send network message to server identifying this device
  std::string deviceId;
  std::string deviceUserName;
  std::string deviceModel;
  std::string deviceOsVersion;
  
#ifdef AMJU_IOS
  int devId;
  GetDeviceInfo(&devId, &deviceUserName, &deviceModel, &deviceOsVersion);
  deviceId = ToString(devId);
#endif // AMJU_IOS
  
#ifdef _DEBUG
  std::cout << "Sending device info: " <<
  deviceId << ", " <<
  deviceUserName << ", " <<
  deviceModel << ", " <<
  deviceOsVersion << "\n";
#endif // _DEBUG
  
  NetSendDeviceInfo(deviceId, deviceUserName, deviceModel, deviceOsVersion);
  
  return true;
}

bool NetSendUpdateDeviceInfo()
{
  GameConfigFile* gcf = TheGameConfigFile::Instance();
  
  // Check if anything has changed which we should update on the server.
  std::string deviceUserName;
  std::string deviceModel;
  std::string deviceOsVersion;

  int devId = 0;

#ifdef AMJU_IOS
  // Device ID and model should never change, but we get them anyway as that's
  //  what this function gives us. But we SEND the device ID we got when we first
  //  ran the program and saved in the config file. This is just in case the
  //  device ID is not constant over time.
  GetDeviceInfo(&devId, &deviceUserName, &deviceModel, &deviceOsVersion);
#endif // AMJU_IOS

  // Handle empty device ID, which you get upgrading from v.1.0
  std::string deviceId = gcf->GetValue(DEVICE_ID);
  if (deviceId.empty())
  {
    deviceId = ToString(devId);
    gcf->Set(DEVICE_ID, deviceId);
  }
  
  std::string prevDeviceOsVersion = gcf->GetValue(DEVICE_OS_VERSION);
  std::string prevDeviceUserName = gcf->GetValue(DEVICE_USER_NAME);
  std::string prevVersion = gcf->GetValue(CLIENT_VERSION);
  
  if (GetVersionStr() != prevVersion ||
      deviceOsVersion != prevDeviceOsVersion ||
      deviceUserName != prevDeviceUserName)
  {
    // Something has changed, so send update as for a new install.
    NetSendDeviceInfo(deviceId, deviceUserName, deviceModel, deviceOsVersion);
  }
  else
  {
std::cout << "NetSend: Nothing has changed, so not resending device info.\n";
  }

  return true;
}
  
static std::string s_sessionStart;
  
void NetSendMarkSessionStart()
{
  s_sessionStart = ToString(Time::Now().ToSeconds());
}
  
bool NetSendPlaySession(int flags)
{
  if (s_sessionStart.empty())
  {
    // Already sent session
    return false;
  }
  
  GameConfigFile* gcf = TheGameConfigFile::Instance();

  std::string now = ToString(Time::Now().ToSeconds());
  std::string flagStr = ToString(flags);
  
  std::string url = URL_ROOT +
    LOG_SESSION_SCRIPT_NAME + 
    "?"
    "device_id='" + EncodeStr(gcf->GetValue(DEVICE_ID)) + "'&"
    "session_start='" + EncodeStr(s_sessionStart) + "'&"
    "session_end='" + EncodeStr(now) + "'&"
    "session_flags='" + EncodeStr(flagStr) + "'";
  
  s_sessionStart.clear();

std::cout << "Sending play session info: " << url << "\n";
  
  auto req = new NetSendReq(url, HttpClient::GET, "send play session");
  bool b = TheSerialReqManager::Instance()->AddReq(req);
  return b;
}
  
}

