// * Amjula music theory *
// (c) Copyright 2018 Juliet Colman

#pragma once

#include <string>
#include "SerialReqManager.h"

// Classes to set up requests, and handle the response.
namespace Amju
{
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
    const std::string& deviceOsVersion_);
  
  void HandleResult() override;

private:
  std::string m_deviceId;
  std::string m_deviceUserName;
  std::string m_deviceModel;
  std::string m_deviceOsVersion;
};
  
class NetSendReq : public OnlineReq
{
public:
  NetSendReq(
    const std::string& url,
    HttpClient::HttpMethod method,
    const std::string& name) : OnlineReq(url, method, name) {}

  void HandleResult() override;
};
 
}


