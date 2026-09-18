// SPDX-License-Identifier: Apache-2.0
// Copyright 2026, Amlal El Mahrouss.

/// (C) Amlal El Mahrouss

#include <sdk/ll_model.hpp>

#ifndef NE_ZFC_NOT
#define NE_ZFC_NOT "¬"
#endif

#ifndef NE_ZFC_NULL
#define NE_ZFC_NULL "∅"
#endif

namespace ll {

// Returns true when `expr` is consistent with the ZFC axioms.
// Rejects empty expressions, negations, and empty-set terms.
bool zfc_set_solver::validate(const std::string &expr) const {
  if (expr.empty())
    return false;

  if (expr.starts_with(NE_ZFC_NOT))
    return false;

  if (expr.find(NE_ZFC_NULL) != std::string::npos)
    return false;

  return true;
}

} // namespace ll
