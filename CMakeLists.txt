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
# Biblioteca principal
# ---------------------------------------------------------------------------
add_library(erebus_core STATIC
    src/domain/dice.cpp
    src/handlers/dice_handler.cpp
    src/services/dice_service.cpp
    src/adapters/event_bus_impl.cpp
)

target_include_directories(erebus_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(erebus_core PUBLIC cxx_std_17)

# ---------------------------------------------------------------------------
# Testes
# ---------------------------------------------------------------------------
enable_testing()
add_subdirectory(tests/unit)
add_subdirectory(tests/integration)
