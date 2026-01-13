#pragma once

// Meta data we can output or suppress.
enum Metadata
{
  META_TIME     = 0x01,
  META_BEAT     = 0x02,
  META_NOTE     = 0x04,
  META_NOTENAME = 0x08,
  META_COMMENT  = 0x10,
};

static const unsigned int SUPPRESS_ALL = 0xffffffff;

// These flags just add up (bitwise OR) -- so you can call multiple
//  times to set different suppress flags.
void SetSuppressFlags(unsigned int suppressFlags);

unsigned int GetSuppressFlags();


