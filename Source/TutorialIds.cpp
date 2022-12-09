// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Localise.h>
#include "TutorialIds.h"

namespace Amju
{
// Look up localised string from ID
std::string LookupTutorialString(int id)
{
  static const std::string STRS[] = {
    "$$$1" /* welcome to amjula music school! */,
    "$$$2" /* swipe left and right to move along the corridor */,
    "@@@The blue dot indicates Middle C.",
    "@@@Swipe left and right above the keys to change octave."
  };
  return Lookup(STRS[id]); 
}

}

