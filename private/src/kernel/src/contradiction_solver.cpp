// SPDX-License-Identifier: Apache-2.0
// Copyright 2026, Amlal El Mahrouss.

/// (C) Amlal El Mahrouss

#include <sdk/ll_solver.hpp>

namespace ll {

bool contradiction_solver::find_contradiction(const theorem_params &thm,
                                              proof_params         &out) const {
  const std::string negated = "¬(" + thm.conclusion + ")";
  out.add_step("Assume: " + negated + ".");

  if (m_zfc.validate(negated)) {
    out.add_step("No contradiction reachable from: " + negated + ".");
    return false;
  }

  out.add_step("Contradiction: " + negated + " is inconsistent with the axioms.");
  out.add_step("Therefore '" + thm.conclusion + "' holds. QED.");
  return true;
}

std::optional<proof_params>
contradiction_solver::solve(const theorem_params &thm) {
  proof_params out;
  out.theorem = thm;
  out.add_step("Strategy: contradiction.");

  if (!find_contradiction(thm, out)) {
    out.status = proof_status::inconclusive;
    return std::nullopt;
  }

  out.status = proof_status::proven;
  return out;
}

} // namespace ll
