add_library(testing
            ChimeraTest.cpp
            gtest/gtest-all.cc
            )

target_include_directories(testing
                           PRIVATE ${CMAKE_SOURCE_DIR}/include
                           PRIVATE ${CMAKE_SOURCE_DIR}/include/lib
                           )