cmake_minimum_required(VERSION 3.13)

# set(CMAKE_BINARY_DIR "${PROJECT_SOURCE_DIR}/build")
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(scanner)

add_custom_target(run
    COMMAND "./app"
)

list(APPEND HEADERS
            "${PROJECT_SOURCE_DIR}/include")

list(APPEND MAIN_SOURCES
  "${PROJECT_SOURCE_DIR}/src/main.c"
  "${PROJECT_SOURCE_DIR}/src/scanner.c"
  )

include_directories(${HEADERS})

add_executable(app ${MAIN_SOURCES})
