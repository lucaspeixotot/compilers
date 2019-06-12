cmake_minimum_required(VERSION 3.13)

project(lexic_and_syntax_analizer)

add_custom_target(run
    COMMAND "./app"
)

list(APPEND HEADERS
            "${PROJECT_SOURCE_DIR}/include")

list(APPEND MAIN_SOURCES
            "${PROJECT_SOURCE_DIR}/src/main.c")

include_directories(${HEADERS})

add_executable(app ${MAIN_SOURCES})
