# Core - Chimera Design Implementation
add_subdirectory(Core)

# Tooling - LLVM/Clang-related
add_subdirectory(Tooling)

# Test Framework
add_subdirectory(Testing)

# Operators
add_subdirectory(Operators)

add_library(utils
            Log.cpp
            Utils.cpp
            )
target_include_directories(utils 
                           PRIVATE ${CMAKE_SOURCE_DIR}/include
                           )
