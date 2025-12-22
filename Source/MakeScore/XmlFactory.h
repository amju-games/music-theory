#pragma once

#include <functional>
#include <map>
#include <typeindex>

// Completely generic factory 
// (Not using amjulib or other lib here)
template <typename KEY_TYPE, typename CREATED_BASE_TYPE>
class Factory
{
public:
  using CreatorFunc = std::function<CREATED_BASE_TYPE*()>;

  void Add(KEY_TYPE kt, CreatorFunc func)
  {
    m_map[kt] = func;
  }

  template <class DERIVED_TYPE>
  void AddType(KEY_TYPE kt)
  {
    static_assert(std::is_base_of<CREATED_BASE_TYPE, DERIVED_TYPE>::value);
    Add(kt, [](){ return new DERIVED_TYPE; });
  }

  CREATED_BASE_TYPE* Create(KEY_TYPE kt)
  {
    return m_map[kt]();
  }

private:
  std::map<KEY_TYPE, std::function<CREATED_BASE_TYPE*()>> m_map;
};

class XmlNodeHandler;

using XmlFactory = Factory<std::type_index, XmlNodeHandler>;


