#pragma once

namespace Amju
{
struct PathConfig;

// Set up a procedually generate path config, with extravagance 0..1.
// NB The start and end positions should already be set in the config.
void GenerateCurvedPathConfig(PathConfig& config, float extravagance);
}

