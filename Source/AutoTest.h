#pragma once

namespace Amju
{
// *** Auto Testing ***
// Auto testing is enabled with command-line param,
//  --autotest or --smoketest.
// If enabled, each Game State performs tests on itself
//  when activated, or proceeds to another state, as
//  appropriate.
// If a test detects that all testing has been completed,
//  it calls EndTest(), with success or failure flag.
enum class AutoTestLevel
{
  AMJU_NO_TEST,
  AMJU_SMOKE_TEST,
  AMJU_FULL_TEST,
};

// Get auto test level
AutoTestLevel GetAutoTestLevel();

// End the process, because all tests are complete, or
//  there has been such a catastrophic failure that we
//  had better just quit.
void EndTest(bool success);
}

