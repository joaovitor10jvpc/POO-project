cmake_minimum_required(VERSION 3.16)
project(AgendaPlanner LANGUAGES CXX)

# Using C++20 for <=> operator and concepts later
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options for future stages
option(AGENDA_BUILD_TESTS "Build unit tests" ON)
option(AGENDA_BUILD_CLI "Build CLI demo" OFF)
option(AGENDA_BUILD_GUI "Build GUI (Qt)" OFF)

add_library(agenda_headers INTERFACE)
target_include_directories(agenda_headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Common warning flags (Stage 1 pre-check)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(agenda_headers INTERFACE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
elseif (MSVC)
    target_compile_options(agenda_headers INTERFACE /W4 /permissive-)
endif()

# Dummy target to force compilation of headers in stage 1 (uses one TU that includes all)
add_library(agenda_header_check STATIC design/header_check.cpp)
target_link_libraries(agenda_header_check PRIVATE agenda_headers)
set_target_properties(agenda_header_check PROPERTIES OUTPUT_NAME "precheck_headers")

# Placeholder for future CLI / sources
if(AGENDA_BUILD_CLI)
    add_executable(agenda_cli src/main_cli.cpp)
    target_link_libraries(agenda_cli PRIVATE agenda_headers)
endif()

# Qt integration (Stage 3)
if(AGENDA_BUILD_GUI)
    find_package(Qt6 COMPONENTS Widgets REQUIRED)
    add_executable(agenda_gui src/main_gui.cpp)
    target_link_libraries(agenda_gui PRIVATE Qt6::Widgets agenda_headers)
endif()

install(DIRECTORY include/ DESTINATION include)
