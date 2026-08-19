#include <fstream>
#include <iostream>
#include "catch.hpp"
#include "CurvedPath.h"
#include "GenerateCurvedPathConfig.h"

using namespace Amju;

bool SavePathToCsv(
  const std::string& filename, 
  const std::vector<Vec2f>& data) 
{
    // Open an output file stream
    std::ofstream file(filename);
    
    // Check if the file stream opened successfully
    if (!file.is_open()) 
    {
        std::cerr << "Error: Could not open or create file " << filename << std::endl;
        return false;
    }
    
    // Optional: Write CSV column headers
    //file << "Column1,Column2\n";
    
    // Write each pair on a new line separated by a comma
    for (const auto& p : data) {
        file << p.x << "," << p.y << "\n";
    }
    
    return true;
}

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

TEST_CASE("Generate curved path, extravagance .1", "[CurvedPath]")
{
  PathConfig config;
  config.startPos = Vec2f(0, 0);
  config.endPos = Vec2f(100.f, 100.f);

  GenerateCurvedPathConfig(config, .1f);
  CurvedPathGenerator gen(config);
  const auto path = gen.Generate(1000);
  SavePathToCsv("extravagance_0.1.csv", path);
}

TEST_CASE("Generate curved path, extravagance .99", "[CurvedPath]")
{
  PathConfig config;
  config.startPos = Vec2f(0, 0);
  config.endPos = Vec2f(100.f, 100.f);

  GenerateCurvedPathConfig(config, .99f);
  REQUIRE(config.spirals.size() == 2);
  REQUIRE(config.loops.size() > 0);
  REQUIRE(config.figureEights.size() == 1);

  CurvedPathGenerator gen(config);
  const auto path = gen.Generate(1000);

  SavePathToCsv("extravagance_0.99.csv", path);

//  PrintPath(path);
}

TEST_CASE("Generate curved path, extravagance .5", "[CurvedPath]")
{
  PathConfig config;
  config.startPos = Vec2f(0, 0);
  config.endPos = Vec2f(100.f, 100.f);

  GenerateCurvedPathConfig(config, .5f);
  REQUIRE(config.spirals.size() == 2);
  REQUIRE(config.loops.size() > 0);
  REQUIRE(config.figureEights.size() == 0); // not extravagant enough

  CurvedPathGenerator gen(config);
  const auto path = gen.Generate(1000);

  SavePathToCsv("extravagance_0.5.csv", path);

//  PrintPath(path);
}

