add_library(core
            MutationOperator.cpp
            MutationTemplate.cpp
            )
target_include_directories(core
                           PRIVATE ${CMAKE_SOURCE_DIR}/include
                           )