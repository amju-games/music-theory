// * Amjula Music Theory * (c) Copyright Juliet Colman 2024

#pragma once

// Scores have one or more parts; a part has a sequence of bars.
// Every part has the same number of bars.
//
// Example: 
// Part 1  |=====|=========|=====|====|
//         |     |         |     |    |
// Part 2 {|=====|=========|=====|====|
//        {|=====|=========|=====|====|
//
// In this example, part 1 has one stave, part 2 has two staves.
// When we render a line (a range or bar numbers) the verticals in
//  each bar line up.

class Part
{
public:
  Part(const std::string& name) { m_name = name; }

private:
  std::string m_name; // e.g. "Piano", is printed on line 1 to left of staves, vertically
   // centred 
};

