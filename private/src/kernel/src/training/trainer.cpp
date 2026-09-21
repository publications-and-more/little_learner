// SPDX-License-Identifier: Apache-2.0
// Copyright 2026, Amlal El Mahrouss.

/// (C) Amlal El Mahrouss

#include <sdk/ll_trainer.hpp>

namespace ll {

/// @note this should have more granularity.
static double status_to_loss(const proof_status &s) {
  switch (s) {
  case proof_status::proven:
    return 0.0;
  case proof_status::inconclusive:
    return 0.5;
  case proof_status::refuted:
    return 1.0;
  case proof_status::pending:
    return 1.0;
  }
  return 1.0;
}

std::vector<double> tensor_param::fold() const {
  std::vector<double> out;
  out.reserve(T_i.size() * T_ij.size() * T_ijk.size());

  for (double a : T_ijk)
    for (double b : T_ij)
      for (double c : T_i)
        out.push_back(a * b * c);

  return out;
}

void tensor_param::backprop(const std::vector<double> &flat_grad,
                            std::vector<double> &grad_i,
                            std::vector<double> &grad_ij,
                            std::vector<double> &grad_ijk) const {
  const std::size_t ni = T_i.size();
  const std::size_t nij = T_ij.size();
  const std::size_t nijk = T_ijk.size();

  if (flat_grad.size() != ni * nij * nijk)
    throw std::invalid_argument(
        "flat_grad size does not match tensor dimensions");

  grad_i.assign(ni, 0.0);
  grad_ij.assign(nij, 0.0);
  grad_ijk.assign(nijk, 0.0);

  std::size_t idx = 0;
  for (std::size_t a = 0; a < nijk; ++a) {
    for (std::size_t b = 0; b < nij; ++b) {
      for (std::size_t c = 0; c < ni; ++c, ++idx) {
        const double g = flat_grad[idx];
        grad_i[c] += g * T_ijk[a] * T_ij[b];
        grad_ij[b] += g * T_ijk[a] * T_i[c];
        grad_ijk[a] += g * T_ij[b] * T_i[c];
      }
    }
  }
}

void cauchy_optimizer::step(std::vector<double> &theta,
                            const std::vector<double> &grad,
                            const std::vector<double> &fisher_diag) {
  if (m_velocity_.size() != theta.size())
    m_velocity_.assign(theta.size(), 0.0);

  for (std::size_t i = 0; i < theta.size(); ++i) {
    const double nat_grad = grad[i] / (fisher_diag[i] + 1e-8);

    m_velocity_[i] = m_gamma_ * m_velocity_[i] - m_eta_ * nat_grad;
    theta[i] += m_velocity_[i];
  }
}

trainer::trainer(machine_model_priv &model, solver_base &solver)
    : m_model_(model), m_solver_(solver) {
  m_params_.T_i.assign(2, 0.1);
  m_params_.T_ij.assign(4, 0.1);
  m_params_.T_ijk.assign(4, 0.1);
}

void trainer::add_theorem(theorem_params thm) {
  m_batch_.push_back(std::move(thm));
}

double trainer::loss_() const {
  if (m_batch_.empty())
    return 1.0;

  double total = 0.0;

  for (const auto &thm : m_batch_) {
    auto result = m_solver_.solve(thm);
    total += result.has_value() ? status_to_loss(result->status) : 1.0;
  }

  return total / static_cast<double>(m_batch_.size());
}

factor_grads_t trainer::factor_grads_(double base_loss) const {
  constexpr double eps = 1e-5;

  factor_grads_t g;
  
  g.d_i.resize(m_params_.T_i.size(), 0.0);
  g.d_ij.resize(m_params_.T_ij.size(), 0.0);
  g.d_ijk.resize(m_params_.T_ijk.size(), 0.0);

  // Cast away const to perturb — each perturbation is restored before
  // returning.
  auto &p = const_cast<tensor_param &>(m_params_);

  for (std::size_t c = 0; c < p.T_i.size(); ++c) {
    p.T_i[c] += eps;
    g.d_i[c] = (loss_() - base_loss) / eps;
    p.T_i[c] -= eps;
  }

  for (std::size_t b = 0; b < p.T_ij.size(); ++b) {
    p.T_ij[b] += eps;
    g.d_ij[b] = (loss_() - base_loss) / eps;
    p.T_ij[b] -= eps;
  }

  for (std::size_t a = 0; a < p.T_ijk.size(); ++a) {
    p.T_ijk[a] += eps;
    g.d_ijk[a] = (loss_() - base_loss) / eps;
    p.T_ijk[a] -= eps;
  }

  return g;
}

double trainer::step() {
  const double loss = loss_();

  (void)loss;

  const auto grads = factor_grads_(loss);

  auto make_fisher = [](const std::vector<double> &g) {
    if (g.empty())
      return std::vector<double>{};

    std::vector<double> f(g.size());

    for (std::size_t i = 0; i < g.size(); ++i)
      f[i] = g[i] * g[i];

    return f;
  };

  m_optim_.step(m_params_.T_i, grads.d_i, make_fisher(grads.d_i));
  m_optim_.step(m_params_.T_ij, grads.d_ij, make_fisher(grads.d_ij));
  m_optim_.step(m_params_.T_ijk, grads.d_ijk, make_fisher(grads.d_ijk));

  return loss;
}

void trainer::train(int epochs) {
  for (int e = 0; e < epochs; ++e) {
    (void)step();
  }
}

} // namespace ll
