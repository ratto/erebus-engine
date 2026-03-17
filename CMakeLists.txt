cmake_minimum_required(VERSION 3.20)
project(erebus_engine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra)

# ---------------------------------------------------------------------------
# Cobertura gcov (opcional) — ativar com: cmake -B build -DENABLE_COVERAGE=ON
# ---------------------------------------------------------------------------
option(ENABLE_COVERAGE "Enable gcov coverage" OFF)
if(ENABLE_COVERAGE)
  add_compile_options(--coverage)
  add_link_options(--coverage)
endif()

# ---------------------------------------------------------------------------
# Google Test via FetchContent
# ---------------------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ---------------------------------------------------------------------------
# Biblioteca principal (src/ ainda vazia — preenchida nas próximas sprints)
# ---------------------------------------------------------------------------
add_library(erebus_core INTERFACE)

# ---------------------------------------------------------------------------
# Testes
# ---------------------------------------------------------------------------
enable_testing()
add_subdirectory(tests/unit)
add_subdirectory(tests/integration)
