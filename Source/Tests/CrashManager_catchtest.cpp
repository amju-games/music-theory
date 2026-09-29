#include <cstdio>
#include <fstream>
#include "catch.hpp"
#include "CrashManager.h"

// Helper to manually create a fake leftover crash marker file before the test runs
void CreateFakeCrashMarker() 
{
  std::ofstream file("run_marker.txt");
  file << "active";
  file.close();
}

// Helper to ensure the marker is manually wiped between test cases
void ClearMarkerFile() 
{
  std::remove("run_marker.txt");
}

TEST_CASE("CrashManager Life-Cycle and Marker File Handling", "[CrashManager]") 
{
  // Ensure we start with a completely clean slate
  ClearMarkerFile();

  SECTION("Clean Startup Loop") 
  {
    {
      CrashManager manager;

      // 2. Assert: It should report that the previous run did NOT crash
      REQUIRE(manager.DidPreviousRunCrash() == false);

      // 3. Assert: The manager should have created a new active run marker file
      std::ifstream file("run_marker.txt");
      REQUIRE(file.is_open() == true);
      file.close();
    } 
    // <- The 'manager' object goes out of scope here, 
    //  firing its Destructor (~CrashManager)

    // Destructor should have deleted the marker file safely on clean exit
    std::ifstream file("run_marker.txt");
    REQUIRE(file.is_open() == false);
  }

  SECTION("Simulated Crash Recovery Loop") 
  {
    // Fake a crash by manually dumping a leftover file onto the disk
    CreateFakeCrashMarker();

    // Initialise the manager. It should notice the file!
    CrashManager manager;

    // Assert: It correctly flags that the previous session crashed
    REQUIRE(manager.DidPreviousRunCrash() == true);

    // Clean up our sandbox state
    ClearMarkerFile();
  }
}

