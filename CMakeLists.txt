cmake_minimum_required(VERSION 3.10.0)

project(topk_words)

set(PROJECT_SOURCE
    src/topk_words.cpp
)

set(PROJECT_INCLUDES
    ${PROJECT_SOURCE_DIR}/src
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
