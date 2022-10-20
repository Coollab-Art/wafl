cmake_minimum_required(VERSION 3.8)

add_library(wafl)
add_library(wafl::wafl ALIAS wafl)
target_compile_features(wafl PUBLIC cxx_std_20)

# ---Add source files---
target_include_directories(wafl PUBLIC include)
target_sources(wafl PRIVATE
    src/wafl.cpp
)

# Set warning level
if(MSVC)
    target_compile_options(wafl PRIVATE /W4)
else()
    target_compile_options(wafl PRIVATE -Wall -Wextra -Wpedantic -pedantic-errors -Wconversion -Wsign-conversion)
endif()

# Maybe enable warnings as errors
set(WARNINGS_AS_ERRORS_FOR_WAFL OFF CACHE BOOL "ON iff you want to treat warnings as errors")
if(WARNINGS_AS_ERRORS_FOR_WAFL)
    if(MSVC)
        target_compile_options(wafl PRIVATE /WX)
    else()
        target_compile_options(wafl PRIVATE -Werror)
    endif()
endif()