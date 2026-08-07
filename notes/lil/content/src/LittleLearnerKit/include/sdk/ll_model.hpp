// SPDX-License-Identifier: Apache-2.0
// Copyright 2026, Amlal El Mahrouss.

/// (C) Amlal El Mahrouss

#pragma once

#include <filesystem>
#include <ocl/math_fwd.hpp>
#include <ocl/scientific_fwd.hpp>
#include <ocl/asio_fwd.hpp>
#include <ocl/asio.hpp>

struct machine_model_priv;
struct machine_param_tree;

struct machine_model_priv final
{
    machine_model_priv() = default;
    ~machine_model_priv() = default;

    machine_model_priv& operator=(const machine_model_priv&) = default;
    machine_model_priv(const machine_model_priv&) = default;

    using string_type = std::string;
    using path_type = std::filesystem::path;

    string_type m_tag_name_{};
    path_type m_training_path{};

    machine_param_tree* m_param_tree_high{};
    machine_param_tree* m_param_tree_low{};
    machine_param_tree* m_param_tree_left{};
    machine_param_tree* m_param_tree_right{};
};
