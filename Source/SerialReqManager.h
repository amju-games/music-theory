// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#ifndef SERIAL_REQ_MANAGER_H_INCLUDED
#define SERIAL_REQ_MANAGER_H_INCLUDED

#include <OnlineReqManager.h>
#include <StringUtils.h>

namespace Amju
{
class SerialReqManager
{
public:
  SerialReqManager();

  void Update();

  // Call to add a new request.
  // This will be executed by another thread. When the request has finished,
  // Update() will call HandleResult() on the request then erase it.
  // maxRequests: this is the max number of requests with the same name.
  // If adding the new request would exceed the maximum, the request
  // will not be added. 
  // Returns true if request added to queue; returns false if max number
  // of requests of this type would be exceeded.
  bool AddReq(RCPtr<OnlineReq>, int maxRequestsOfThisType = 1, bool discardNewNotOld = true);

  // Count all queued requests
  int CountAllReqs() const;

  // Return the number of requests with the given name.
  int CountReqsWithName(const std::string& name);

  // Get total number of responses received
  int GetTotalNumResponses() const;

  // Get total number of requests queued
  // Seems to duplicated CountAllReqs() ???
//  int GetTotalNumQueued() const;

  // Clear all requests - but any request in flight is not cancelled
  void Clear();

  // For reporting, debugging etc
  Strings GetRequestNames() const;

private:
  typedef std::deque<RCPtr<OnlineReq> > OnlineReqs;
  OnlineReqs m_reqs;

  // Worker thread which gets the front request using m_hc, then sleeps
  class SerialThread : public Thread
  {
  public:
    virtual void Work();
    void SetReq(RCPtr<OnlineReq> req);
    bool Waiting() const;

  private:
    RCPtr<OnlineReq> m_req;
    HttpClient m_hc;
    mutable Mutex m_mutex;
  };
  SerialThread* m_thread;
  int m_numResponses;
  int m_numQueued;
};
  
using TheSerialReqManager = Singleton<SerialReqManager>;
}

#endif
