#pragma once

#include <GuiText2.h>

namespace Amju
{
// * GuiTextFile *
// Gui Text type which loads its text from a file, rather than
//  a hardcoded (but localised) string in the gui file.
// TODO Promote to amjulib
class GuiTextFile : public GuiText2
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  GuiTextFile* Clone() override { return new GuiTextFile(*this); }

  // The text set here is treated as a filename; we read the 
  //  file contents then call the base class impl to actually
  //  set the text.
  // (The pre-localised text is used to save back the filename in
  //  the base class.)
  void SetText(const std::string& text) override;
};
}

