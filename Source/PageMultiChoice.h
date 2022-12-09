// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <string>
#include <vector>
#include <RCPtr.h>
#include <StringUtils.h>
#include "Page.h"

namespace Amju
{


  // HINTS are intensely GUI-based, animations/behaviour rather than 
  //  just data like strings. So an active Hint takes over the GUI, and
  //  can load its own GUI, which it displays in addition to the 
  //  standard GUI.
  // Multi coloured highlighting: this will be much easier if vertices
  //  have colours. Then we don't have multiple scores, say, or snippets
  //  of text. It's all the same tri list, just with different vertex
  //  colours. OK MAKE IT SO.




  // * PageMultiChoice *
  // User selects correct answer. How user selects (drag, button, etc) 
  //  does not affect the core Page definition, and could be randomised.
  class PageMultiChoice : public Page
  {
  public:


  protected:

  };


  // TODO TEMP TEST
  // Sketching out how this would work
  class PageMusicalTerm : public PageMultiChoice
  {
  public:
    

  protected:

  };
}

