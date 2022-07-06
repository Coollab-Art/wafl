cmake_minimum_required(VERSION 3.8)

# ## IF( header-only library )
add_library(wafl INTERFACE)
add_library(wafl::wafl ALIAS wafl)
target_compile_features(wafl INTERFACE cxx_std_20)
target_include_directories(wafl INTERFACE include)

# ## ELSEIF( library with .cpp files )
add_library(wafl)
add_library(wafl::wafl ALIAS wafl)
target_compile_features(wafl PUBLIC cxx_std_20)

# ---Add source files---
target_include_directories(wafl PUBLIC include)
target_sources(wafl PRIVATE
    src/wafl.cpp
    src/other_file.cpp
)

if(WAFL_ENABLE_WARNINGS_AS_ERRORS)
    message("-- [wafl] Enabling warnings as errors for wafl")

    if(MSVC)
        target_compile_options(wafl PRIVATE /WX /W4)
    else()
        target_compile_options(wafl PRIVATE -Werror -Wall -Wextra -Wpedantic -pedantic-errors -Wconversion -Wsign-conversion)
    endif()
else()
    message("-- [wafl] Not using warnings as errors for wafl")
endif()

# ## ENDIF