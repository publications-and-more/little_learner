// SPDX-License-Identifier: Apache-2.0
// Copyright 2026, Amlal El Mahrouss.

/// (C) Amlal El Mahrouss

#include <sdk/ll_model.hpp>
#include <algorithm>

namespace ll {

// Returns true when `conclusion` is inhabitable given `hypotheses`.
// Checks: direct match, inductive-hypothesis match, and substring coverage.
bool dtt_solver::check(const std::vector<std::string> &hypotheses,
                       const std::string              &conclusion) const {
  if (conclusion.empty())
    return false;

  if (std::find(hypotheses.begin(), hypotheses.end(), conclusion) !=
      hypotheses.end())
    return true;

  const std::string ih = "inductive_hypothesis: ";
  for (const auto &h : hypotheses) {
    if (h.starts_with(ih) && h.substr(ih.size()) == conclusion)
      return true;
  }

  for (const auto &h : hypotheses) {
    if (!h.empty() && conclusion.find(h) != std::string::npos)
      return true;
    if (!h.empty() && h.find(conclusion) != std::string::npos)
      return true;
  }

  return false;
}

} // namespace ll
