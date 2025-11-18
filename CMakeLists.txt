cmake_minimum_required(VERSION 3.10.0)

project(topk_words)

set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)

set(PROJECT_INCLUDES
    ${PROJECT_SOURCE_DIR}/src
)

# functions.cpp используется в основной программе и в бенчмарке
# сделаем библиотеку
set(TARGET_LIB topk_lib)
add_library(${TARGET_LIB} src/functions.cpp)
target_include_directories(${TARGET_LIB} PUBLIC ${PROJECT_INCLUDES})
set_target_properties(${TARGET_LIB} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# ---------------------------
# Основная программа topk_words
# ---------------------------
add_executable(${PROJECT_NAME}
    src/topk_words.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${TARGET_LIB})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# ============================================================
#                 GOOGLE BENCHMARK
# ============================================================

set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
set(BENCHMARK_ENABLE_TESTING OFF)

add_subdirectory(benchmark)

set(TARGET_BENCH bench_topk)
add_executable(${TARGET_BENCH}
    src/bench_topk.cpp
)
target_link_libraries(${TARGET_BENCH} PRIVATE
    benchmark::benchmark
    ${TARGET_LIB}
)
target_include_directories(${TARGET_BENCH} PRIVATE ${PROJECT_INCLUDES})
set_target_properties(${TARGET_BENCH} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

