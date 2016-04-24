# Core - Chimera Design Implementation
add_subdirectory(Core)

# Tooling - LLVM/Clang-related
#add_subdirectory(Tooling)

# Test Framework
add_subdirectory(Testing)

# Operators
add_subdirectory(Operators)

add_library(executable
            Tooling/ChimeraTool.cpp
            Tooling/CompilationDatabaseUtils.cpp
            Tooling/FrontendActions.cpp
            Log.cpp
            Utils.cpp
            )
target_include_directories(executable
                           PRIVATE ${CMAKE_SOURCE_DIR}/include
                           )
target_link_libraries(executable operators testing core)
