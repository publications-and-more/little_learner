# SPDX-License-Identifier: Apache-2.0
# Copyright 2026, Amlal El Mahrouss.

cmake_minimum_required(VERSION 3.20)

project(LittleLearner
  VERSION 1.0.0
  LANGUAGES CXX
  DESCRIPTION "Little Learner — Ne.app formal reasoning ML model"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set OCL_INCLUDE_DIR to point at the OCL headers if they are not on the
# default system include path (e.g. -DOCL_INCLUDE_DIR=/path/to/ocl/include).
set(OCL_INCLUDE_DIR "" CACHE PATH "Path to OCL include directory")

add_library(ll_kernel STATIC
  private/src/kernel/src/construction_solver.cpp
  private/src/kernel/src/contradiction_solver.cpp
  private/src/kernel/src/dtt_solver.cpp
  private/src/kernel/src/induction_solver.cpp
  private/src/kernel/src/trainer.cpp
  private/src/kernel/src/zfc_solver.cpp
)

# ll_model.hpp includes <ocl/...> in its public interface, so OCL_INCLUDE_DIR
# must propagate to consumers of ll_kernel as well.
target_include_directories(ll_kernel PUBLIC
  private/src/kernel/include
  $<$<BOOL:${OCL_INCLUDE_DIR}>:${OCL_INCLUDE_DIR}>
)

target_compile_options(ll_kernel PRIVATE
  $<$<CXX_COMPILER_ID:Clang,GNU>:-Wall -Wextra -Wpedantic>
)
