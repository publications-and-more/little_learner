// SPDX-License-Identifier: Apache-2.0
// Copyright 2026, Amlal El Mahrouss.

/// (C) Amlal El Mahrouss

#pragma once

#include <sdk/ll_model.hpp>
#include <sdk/ll_solver.hpp>
#include <vector>

namespace ll {

// Preprint 1: three-level tensor product that reproduces the foliated integral
// ⟨P⟩ = ∫_{P_{S,F}} F_t dV = T_i ⊗ T_ij ⊗ T_ijk  (Fubini decomposition).
// Each level corresponds to one depth of machine_param_tree (leaf → mid → root).
struct tensor_param final {
  tensor_param() = default;

  std::vector<double> T_i{};    // leaf factor
  std::vector<double> T_ij{};   // mid factor
  std::vector<double> T_ijk{};  // root factor

  // Returns the Fubini outer product as a flat weight vector.
  std::vector<double> fold() const;

  // Backpropagates a flat gradient through the outer product to factor gradients.
  // ∂L/∂T_i[c]   = Σ_{a,b} flat[a,b,c] · T_ijk[a] · T_ij[b]
  // ∂L/∂T_ij[b]  = Σ_{a,c} flat[a,b,c] · T_ijk[a] · T_i[c]
  // ∂L/∂T_ijk[a] = Σ_{b,c} flat[a,b,c] · T_ij[b]  · T_i[c]
  void backprop(const std::vector<double> &flat_grad,
                std::vector<double>       &grad_i,
                std::vector<double>       &grad_ij,
                std::vector<double>       &grad_ijk) const;
};

// Preprint 2: Cauchy momentum update on the Riemannian manifold.
// Integrating the convective term over [ℝ⁺, +∞) gives the damped update:
//   v_{t+1} = γ·v_t − η · G⁻¹(θ) · ∇L(θ)
//   θ_{t+1} = θ_t + v_{t+1}
// G⁻¹ is the inverse Fisher metric supplied by the caller (diagonal approximation).
struct cauchy_optimizer final {
  cauchy_optimizer() = default;

  double m_eta_{1e-3};    // learning rate
  double m_gamma_{0.9};   // Cauchy damping coefficient

  std::vector<double> m_velocity_{};

  void step(std::vector<double>       &theta,
            const std::vector<double> &grad,
            const std::vector<double> &fisher_diag);
};

// Factor-level gradients produced by finite difference on the solver signal.
struct factor_grads_t final {
  std::vector<double> d_i{};
  std::vector<double> d_ij{};
  std::vector<double> d_ijk{};
};

// Drives one training epoch: evaluate solver loss → factor gradients →
// natural-gradient Cauchy step on each factor vector.
class trainer final {
public:
  explicit trainer(machine_model_priv &model, solver_base &solver);

  ~trainer() = default;

  trainer(const trainer &) = delete;
  trainer &operator=(const trainer &) = delete;

  void add_theorem(theorem_params thm);

  // Runs one gradient step. Returns the current loss value.
  double step();

  void train(int epochs);

private:
  // Proof-status loss: proven → 0, inconclusive → 0.5, refuted/pending → 1.
  // Averaged over the stored theorem batch.
  double loss_() const;

  // Perturbs each factor element in turn, calls loss_(), finite-differences.
  factor_grads_t factor_grads_(double base_loss) const;

  machine_model_priv         &m_model_;
  solver_base                &m_solver_;
  tensor_param                m_params_{};
  cauchy_optimizer            m_optim_{};
  std::vector<theorem_params> m_batch_{};
};

} // namespace ll
