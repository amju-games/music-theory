#include <iostream>
#include <CommandLineArgs.h>
#include <MessageQueue.h>
#include "AutoTest.h"

namespace Amju
{
AutoTestLevel GetAutoTestLevelFromCommandLine()
{
  const auto& args = GetCommandLineArgs();
  if (args.Contains("--autotest"))
  {
    std::cout << "*** AUTO TEST MODE ***\n";
    return AutoTestLevel::AMJU_FULL_TEST;
  }
  else if (args.Contains("--smoketest"))
  {
    std::cout << "*** SMOKE TEST MODE ***\n";
    return AutoTestLevel::AMJU_SMOKE_TEST;
  }
  return AutoTestLevel::AMJU_NO_TEST;
}

AutoTestLevel GetAutoTestLevel()
{
  static auto testLevel = GetAutoTestLevelFromCommandLine();
  return testLevel;
}

void EndTest(EndTestReason reason)
{
  std::cout << "**** END OF TEST! ****\n";
  exit(static_cast<int>(reason));
}

void AutoMsg(std::function<void()> f)
{
  TheMessageQueue::Instance()->Add(new FuncMsg(f, SecondsFromNow(1.f)));
}
}

