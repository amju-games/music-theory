#include "ObscureConfigFile.h"

namespace Amju
{
ObscureConfigFile& GetObscureConfigFile()
{
  static ObscureConfigFile obs;
  return obs;
}
}

