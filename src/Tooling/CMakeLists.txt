add_library(tooling
            ChimeraTool.cpp
            CompilationDatabaseUtils.cpp
            FrontendActions.cpp
            ../Log.cpp
            ../Utils.cpp
            )

target_include_directories(tooling
                           PRIVATE ${CMAKE_SOURCE_DIR}/include
                           )
