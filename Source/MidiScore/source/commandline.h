// * atlas * Texture atlas tool
// (c) Copyright 2026 Juliet Colman

#pragma once

#include <optional>
#include <sstream>
#include <string>
#include <vector>

class commandline
{
public:
  using strvec = std::vector<std::string>;

  commandline(int argc, const char** argv) : 
    m_strings(argv, argv + argc) {}

  const strvec& strings() const { return m_strings; }

  int num_strings() const { return static_cast<int>(m_strings.size()); }

  bool contains(const std::string& s) const 
  { 
    return std::find(m_strings.cbegin(), m_strings.cend(), s) != m_strings.cend(); 
  }

  // Return arg after the given string, if the string exists, the following
  //  arg exists, and it can be converted to the specified type.
  // E.g. args: "my.exe --cats 3"
  // get_value<int>("--cats") -> 3
  // get_value<int>("--dogs") -> nullopt
  template <typename T=std::string>
  std::optional<T> get_value(const std::string& key) const
  {
    auto it = std::find(m_strings.cbegin(), m_strings.cend(), key); 
    if (it == m_strings.cend()) return std::nullopt;
    ++it;
    if (it == m_strings.cend()) return std::nullopt;
    return get<T>(std::distance(m_strings.cbegin(), it)); 
  }
   
  // Return the arg at given index, converted to the specified type.
  // E.g. args: "my.exe 1 2 cat"
  // get<int>(1) -> 1
  // get<string>(1) -> "1"
  // get<string>(3) -> "cat"
  // get<int>(3) -> nullopt
  // get(4) -> nullopt
  template <typename T=std::string>
  std::optional<T> get(int n) const
  {
    if (n >= num_strings()) return std::nullopt;
    std::stringstream ss(m_strings[n]);
    T t;
    try
    {
      ss >> t;
    }
    catch (...)
    {
      return std::nullopt;
    }
    return t;
  }
  
  void remove_string(const std::string& r)
  {
    m_strings.erase(std::remove(m_strings.begin(), m_strings.end(), r), m_strings.end());
  }
 
private:
  strvec m_strings;
};

// Check if a flag exists in the command-line input, then remove it, so
//  the positions and number of the mandatory params don't vary.
inline bool check_flag(commandline& cl, const std::string& flag)
{
  bool ret = cl.contains(flag);
  cl.remove_string(flag);
  return ret;
}

