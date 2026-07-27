#include <iostream>
#include "catch.hpp"
#include "CurvedPath.h"

using namespace Amju;

void PrintPath(const std::vector<Vec2f>& path)
{
  std::cout << "X coords:\n";
  for (const auto& pos : path)
  {
    std::cout << pos.x << "\n";
  }

  std::cout << "Y coords:\n";
  for (const auto& pos : path)
  {
    std::cout << pos.y << "\n";
  }
}

TEST_CASE("Curved path 1", "[CurvedPath]")
{
  PathConfig config;
  config.endPos = { 100, 100 };

  CurvedPathGenerator gen(config);
  auto path = gen.Generate(100);

//  PrintPath(path);
}

TEST_CASE("Curved path with a loop", "[CurvedPath]")
{
  PathConfig config;
  config.endPos = { 100, 100 };

  LoopConfig loop = { .4f, .6f, 10.f, 1, true };
  config.loops.emplace_back(loop);

  CurvedPathGenerator gen(config);
  auto path = gen.Generate(50);

//  PrintPath(path);
}

TEST_CASE("Curved path with a fig-8", "[CurvedPath]")
{
  PathConfig config;
  config.endPos = { 100, 100 };

  Figure8Config fig8 { .4f, .8f, 60.f, 1 };
  config.figureEights.emplace_back(fig8);

  CurvedPathGenerator gen(config);
  auto path = gen.Generate(50);

//  PrintPath(path);
}

