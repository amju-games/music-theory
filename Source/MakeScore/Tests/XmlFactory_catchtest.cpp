// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman
//
// Unit tests for XmlFactory

#include "catch.hpp"
#include "../XmlFactory.h" 
#include "../tinyxml2.h"

TEST_CASE("Test XmlFactory", "XmlFactory")
{
  using namespace tinyxml2;

  // Our Xml node handler factory should create a new object of a 
  //  type derived from XmlNodeHandler. This object will know how
  //  to handle a tinyxml2 node of the added type.
  // We use this to handle each node type without a chain of if/elses
  //  and dynamic casts.

  XmlFactory f;

  // Add an XmlTextNodeHandler factory function, which will create a
  //  handler when we get an XMLText node type.
  f.AddType<XmlTextHandler>(std::type_index(typeid(tinxml2::XMLText)));
}

