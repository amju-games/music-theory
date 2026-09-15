#include <fstream>
#include <iostream>
#include "CrashManager.h"

CrashManager::CrashManager()
{
  // 2. Check for the marker file left behind by a crash
  std::ifstream file(m_markerPath);
  if (file.is_open()) {
    m_didCrashLastTime = true;
    file.close();
  }

  // 3. Create/Overwrite the marker file for the current run
  std::ofstream newFile(m_markerPath);
  if (newFile.is_open()) {
    newFile << "active";
    newFile.close();
  }
}

CrashManager::~CrashManager() {
  // If the program closes normally, this destructor runs and deletes the file
  std::remove(m_markerPath.c_str());
}

bool CrashManager::DidPreviousRunCrash() const {
  return m_didCrashLastTime;
}
