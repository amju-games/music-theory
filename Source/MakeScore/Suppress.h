#pragma once

// Meta data we can output or suppress.
enum Metadata
{
  MD_TIME         = 0x01,
  MD_BEAT         = 0x02,
  MD_NOTE         = 0x04,
  MD_NOTENAME     = 0x08,
  MD_COMMENT      = 0x10,
  MD_REST         = 0x20,
  MD_BAR_NUMBERS  = 0x40,
};

static const unsigned int SUPPRESS_ALL = 0xffffffff;

// These flags just add up (bitwise OR) -- so you can call multiple
//  times to set different suppress flags.
void SetSuppressFlags(unsigned int suppressFlags);

unsigned int GetSuppressFlags();

