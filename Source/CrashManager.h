#pragma once

#include <string>

class CrashManager 
{
public:
  CrashManager();

  // Automatically cleans up the marker file on exit
  ~CrashManager();

  // Checks if the last run crashed
  bool DidPreviousRunCrash() const;

private:
  bool m_didCrashLastTime = false;
  std::string m_markerPath = "run_marker.txt";
};
