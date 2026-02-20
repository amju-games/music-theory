// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#include <cassert>
#include <iostream>
#include "KeySig.h"

namespace MidiScore
{
KeySig IntToKeySig(int k)
{
  if (k > 0)
  {
    return static_cast<KeySig>(
      static_cast<int>(KeySig::KS_SHARP_0) + k);
  }
  else if (k < 0)
  {
    return static_cast<KeySig>(
      static_cast<int>(KeySig::KS_FLAT_0) - k);
  }
  return KeySig::KS_SHARP_0;
}

std::string KeySigString(KeySig ks)
{
  if (static_cast<int>(ks) < static_cast<int>(KeySig::KS_FLAT_0))
  {
    // sharp
    return "key-s-" + std::to_string(static_cast<int>(ks));
  }
  // Flat
  return "key-f-" + 
    std::to_string(static_cast<int>(ks) - static_cast<int>(KeySig::KS_FLAT_0));
}

static float Dot(const std::vector<float>& v1, const std::vector<float>& v2)
{
  assert(v1.size() == v2.size());
  float sum = 0.0f;
  for (size_t i = 0; i < v1.size(); ++i) sum += v1[i] * v2[i];
  return sum;
}
 
KeySig GuessKeySig(const Events& events, bool preferFlatKey)
{
  if (events.empty()) return KeySig::KS_SHARP_0;

  std::vector<float> pitchDistribution = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  // Make frequency distribution of pitches, all shifted to the same octave
  for (const Event& e : events)
  {
    if (e.IsRest()) continue;

    int p = e.m_pitch % 12;
    pitchDistribution[p]++;
  }
  // Analyse sharps/flats in distribution... prob simplest to hard code
  //  perfect distribution for each key, then check our distribution against
  //  each one, and pick the closest match. 
  const std::vector<std::vector<float>> DISTRIBS =
  {
    //KS_SHARP_0,
    // c  c# d  d# e  f  f# g  g# a  a# b
    {  1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1 },
    //KS_SHARP_1,
    {  1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1 }, // g maj
    //KS_SHARP_2,
    {  0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1 }, // d maj
    //KS_SHARP_3,
    {  0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1 }, // a maj
    //KS_SHARP_4,
    {  0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1 }, // e maj
    //KS_SHARP_5,
    {  0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1 }, // b maj
    //KS_SHARP_6,
    {  0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1 }, // f# maj!
    //KS_SHARP_7,
    {  1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0 }, // c# maj!
    //KS_FLAT_0,
    // c  db d  eb e  f  gb g  ab a  bb b
    {  1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1 },
    //KS_FLAT_1,
    {  1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0 }, // f maj
    //KS_FLAT_2,
    {  1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0 }, // bb maj
    //KS_FLAT_3,
    {  1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0 }, // eb maj
    //KS_FLAT_4,
    {  1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0 }, // ab maj
    //KS_FLAT_5,
    {  1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0 }, // db maj
    //KS_FLAT_6,
    {  0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1 }, // gb maj
    //KS_FLAT_7,
    {  0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1 }, // cb maj
  };

  [[maybe_unused]]const std::vector<std::string> NAMES = 
  {
    "0 sharps", "1 sharp", "2 sharps", "3 sharps", "4 sharps",
    "5 sharps", "6 sharps", "7 sharps",
    "0 flats", "1 flat", "2 flats", "3 flats", "4 flats",
    "5 flats", "6 flats", "7 flats",
  };
  // Get the dot product of the piece distrib with each scale.
  // Pick the scale with the highest dot prod.
  int bestI = 0;
  float bestDot = 0;
  for (int i = 0; i < DISTRIBS.size(); i++)
  {
    const auto& scale = DISTRIBS[i];
    float d = Dot(scale, pitchDistribution);

    // Decide if this dot product is better than any previously -- and for
    //  same as best values, it depends if we prefer sharps or flats.
    // Only use >= for keys with 5+ flats, so we don't make bad choices
    //  with a small sample size (number of notes).
    bool foundBetter = 
      preferFlatKey && i > static_cast<int>(KeySig::KS_FLAT_4) ? 
        (d >= bestDot) : (d > bestDot);

    if (foundBetter)
    {
      bestDot = d;
      bestI = i;
    }
  }

  return static_cast<KeySig>(bestI);
}
}

