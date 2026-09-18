// SPDX-License-Identifier: Apache-2.0
// Copyright 2026, Amlal El Mahrouss.

/// (C) Amlal El Mahrouss

#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <ocl/asio.hpp>
#include <ocl/asio_fwd.hpp>
#include <ocl/math_fwd.hpp>
#include <ocl/scientific_fwd.hpp>

namespace ll {

struct machine_param_tree final {
  machine_param_tree() = default;

  std::string label{};
  double      value{0.0};

  machine_param_tree *high{};
  machine_param_tree *low{};
  machine_param_tree *left{};
  machine_param_tree *right{};
};

struct machine_model_priv final {
  machine_model_priv() = default;
  ~machine_model_priv() = default;

  machine_model_priv &operator=(const machine_model_priv &) = default;
  machine_model_priv(const machine_model_priv &) = default;

  using string_type = std::string;
  using path_type   = std::filesystem::path;

  string_type m_tag_name_{};
  path_type   m_training_path{};

  machine_param_tree *param_tree_high{};
  machine_param_tree *param_tree_low{};
  machine_param_tree *param_tree_left{};
  machine_param_tree *param_tree_right{};
};

enum class proof_strategy { induction, construction, contradiction, unknown };
enum class proof_status   { pending, proven, refuted, inconclusive };

class theorem_params final {
public:
  std::string              name{};
  std::vector<std::string> hypotheses{};
  std::string              conclusion{};
  proof_strategy           strategy{ proof_strategy::unknown };
};

class proof_params final {
public:
  theorem_params            theorem{};
  std::vector<std::string>  steps{};
  proof_status              status{ proof_status::pending };

  void add_step(std::string s) { steps.push_back(std::move(s)); }
};

class definition_params final {
public:
  std::string name{};
  std::string body{};
};

class corollary_params final {
public:
  std::string name{};
  std::string derived_from{};   // theorem name this follows from
  std::string statement{};
};

class conjecture_params final {
public:
  std::string name{};
  std::string statement{};
  bool        open{ true };
};

// Dependent Type Theory checker: verifies that a proof term is well-typed
// under the hypotheses (i.e., the conclusion is inhabited).
class dtt_solver final {
public:
  // Returns true when `conclusion` is derivable from `hypotheses` under DTT.
  bool check(const std::vector<std::string> &hypotheses,
             const std::string              &conclusion) const;
};

// ZFC set-theory validator: checks set-membership and subset relations
// declared inside proof steps.
class zfc_set_solver final {
public:
  // Returns true when the set expression `expr` is consistent with the axioms.
  bool validate(const std::string &expr) const;
};

} // namespace ll