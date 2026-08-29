#include "GuiTextFile.h"

namespace Amju
{
const char* GuiTextFile::NAME = "text-file";

void GuiTextFile::SetText(const std::string& text) 
{
  // The text set here is treated as a filename; we read the 
  //  file contents then call the base class impl to actually
  //  set the text.

  File f(File::NO_VERSION);
  if (!f.OpenRead(text))
  {
    f.ReportError("Expected text file for text-file element.");
    return; 
  }

  // Multi-line, localised text we will append to.
  std::string allText;

  // Read each line and localise it separately.
  // We don't want to skip blank lines, so get every line.
  // File::More() doesn't seem reliable so we just keep going until
  //  GetLocalisedString fails.
  while (f.More()) // TODO Fix this
  {
    std::string str;
    if (!f.GetLocalisedString(&str))
    {
      break;
    }
    allText += str + "\n";
  } 
  
  m_isMulti = true;
  GuiText2::SetText(allText);
}
}

