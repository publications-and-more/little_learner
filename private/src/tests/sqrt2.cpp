// SPDX-License-Identifier: Apache-2.0
// Copyright 2026, Amlal El Mahrouss.

/// (C) Amlal El Mahrouss

#include <sdk/ll_trainer.hpp>

using namespace ll;

int main() {
  machine_model_priv model;
  induction_solver solver;

  trainer t(model, solver);

  theorem_params thm;
  thm.name = "sqrt2_irrational";
  thm.hypotheses = {"n ∈ ℤ", "m ∈ ℤ", "gcd(n,m) = 1"};
  thm.conclusion = "√2 ≠ n/m";
  thm.strategy = proof_strategy::contradiction;

  t.add_theorem(thm);
  t.train(1000);
}