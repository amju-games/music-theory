#pragma once

#include "XmlFactory.h"
#include "tinyxml2.h"

// Interface for types that handle a tinyxml2 node type.
class XmlNodeHandler
{
public:
  virtual ~XmlNodeHandler() = default;
};

class XmlTextHandler : public XmlNodeHandler
{
public:
};

// Perform depth-first traversal of tinyxml XML tree. 
// For each node, create a handler and handle the node.
// (If no handler exists, we ignore the node.)
class XmlParser
{
public:
  XmlParser();
  bool Parse(tinyxml2::XMLDocument);

private:
  XmlFactory m_factory;
};

