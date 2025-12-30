// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
//
// Sub project to convert easily-authorable music content
//  into list of glyphs.
//
// E.g. use: 
//    (Mac) echo "4/4 c c mr" | ./makescore 
// Same for Win, although we need double quotes and internally we need to 
// strip off the quotes:
//    (Win) echo "4/4 c c mr" | MakeScore.exe

#include "Consts.h"
#include "MakeScore.h"

bool CommandLineParams(int argc, char** argv, MakeScore& ms)
{
  for (int i = 1; i < argc; i++)
  {
    std::string param = argv[i];

    if (param == "--oneline")
    {
      // All on one line
      ms.SetOutputOneLine(true);
    }

    else if (param == "--stave-single")
    {
      ms.SetStaveType(StaveType::STAVE_TYPE_SINGLE);
    }

    else if (param == "--transpose")
    {
      i++;
      int tr = atoi(argv[i]);
      std::cout << "// Transpose: " << tr << "\n";
      ms.SetTranspose(tr);
    }

    else if (param == "--page-width")
    {
      i++;
      // Normalised: i.e. page width of 1 means the default width.
      float pageWidth = DEFAULT_PAGE_WIDTH;
      pageWidth = static_cast<float>(atof(argv[i])) * pageWidth;
      ms.SetPageWidth(pageWidth);
    }

    else if (param == "--scale")
    {
      i++;
      // Normalised: i.e. scale of 1 means the default scale.
      float scale = DEFAULT_SCALE;
      scale = static_cast<float>(atof(argv[i])) * scale;
      ms.SetScale(scale);
    }

    else
    {
      // None of the above: treat as input string
      // Chop off whitespace and quotes - needed for Win, not Mac
      Trim(param);
      StripQuotes(param);

      // Do we need this? TODO
      std::cout << "// " << param << "\n";
      ms.SetInputString(param);

      // For single line rhythm, centre vertically
      // TODO Auto centre 1 or more lines
      ms.SetY(0.5f);

      // TODO Transform input:
      // Add beam groupings
      // Replace beamed quaver/semiquaver glyphs with crotchet glyphs
      ms.Preprocess();

      ms.MakeInternal();
    }
  }
  return true;
}

#ifndef CATCH
// Don't build this main function for unit test exe

int main(int argc, char** argv)
{
  MakeScore ms;

  if (CommandLineParams(argc, argv, ms) == false)
  {
    return 1;
  }

  // Output final string.
  // Don't append a newline char, so we can add more to this line, in
  //  enclosing script.
  std::cout << ms.ToString();

  return 0;
}

#endif // CATCH

