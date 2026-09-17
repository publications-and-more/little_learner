// SPDX-License-Identifier: Apache-2.0
// Copyright 2026, Amlal El Mahrouss.

/// (C) Amlal El Mahrouss

#pragma once

#include <optional>
#include <sdk/ll_model.hpp>

namespace ll {

// Base interface every proof solver implements.
class solver_base {
public:
  virtual ~solver_base() = default;

  // Attempt to prove `thm`. Returns a completed proof_params on success,
  // or nullopt when the strategy cannot close the goal.
  virtual std::optional<proof_params> solve(const theorem_params &thm) = 0;
};

// Proof by induction.
class induction_solver final : public solver_base {
public:
  std::optional<proof_params> solve(const theorem_params &thm) override;

private:
  bool check_base(const theorem_params &thm, proof_params &out) const;
  bool check_step(const theorem_params &thm, proof_params &out) const;

  dtt_solver m_dtt{};
};

// Proof by construction — finds a witness satisfying the conclusion.
class construction_solver final : public solver_base {
public:
  std::optional<proof_params> solve(const theorem_params &thm) override;

private:
  bool build_witness(const theorem_params &thm, proof_params &out) const;

  dtt_solver     m_dtt{};
  zfc_set_solver m_zfc{};
};

// Proof by contradiction — assumes ¬conclusion and derives a contradiction.
class contradiction_solver final : public solver_base {
public:
  std::optional<proof_params> solve(const theorem_params &thm) override;

private:
  bool find_contradiction(const theorem_params &thm, proof_params &out) const;

  zfc_set_solver m_zfc{};
};

} // namespace ll
