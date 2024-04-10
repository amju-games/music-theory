// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include "Attachment.h"

// * Hairpin *
// Show crescendo/decrescendo mark
class Hairpin : public Attachment
{
public:
  virtual std::string ToString() const;
  void SetCrescendo(bool crescNotDecresc) { m_isCresc = crescNotDecresc; }

private:
  bool m_isCresc = true;
};
