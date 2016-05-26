//===- ChimeraTest.h --------------------------------------------*- C++ -*-===//
//
//  Copyright (C) 2015, 2016  Federico Iannucci (fed.iannucci@gmail.com)
//
//  This file is part of Clang-Chimera.
//
//  Clang-Chimera is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Affero General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Clang-Chimera is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Affero General Public License for more details.
//
//  You should have received a copy of the GNU Affero General Public License
//  along with Clang-Chimera. If not, see <http://www.gnu.org/licenses/>.
//
//===----------------------------------------------------------------------===//
/// \file ChimeraTest.h
/// \author Federico Iannucci
/// \brief This file contains the testing utility functions based on
///        Google C++ Test Framework
//===----------------------------------------------------------------------===//

#ifndef INCLUDE_TEST_CHIMERATEST_H_
#define INCLUDE_TEST_CHIMERATEST_H_

#include "lib/gtest/gtest.h" ///< Include gtest.h to use Google Test Framework

#include <string>

namespace chimera
{
namespace mutator
{
class Mutator;
}
}

namespace chimera
{
namespace testing
{

/// @defgroup CHIMERA_TEST_TYPES Chimera Test Types
/// \{
struct TestingOptions {
    TestingOptions() : verbose ( false ) {}
    bool verbose : 1; // Enable verbose output
};
/// \}

// Helper Macro
#define CHIMERA_MUTATOR_MATCH_TEST(mutator_class_name, mutator_identifier)     \
  TEST(mutator_identifier, mutator_test) {                                     \
    ::chimera::testing::testMutatorMatch<mutator_class_name>();                \
  }

/// @brief Test a mutator
/// @details  To test a mutator, its matching rules and mutation rules should be tested
///           To run the test it must be created test_N.cpp files with from 0
///           onwards.
///           For each test_N.cpp a test_N.csv should be created to compare
///           automatically the tests.
///           Matching rules - ASTMatcher + match :
///           - The test_N.csv to testing the matching rules has this format :
///             LINE,COL
///             where LINE is the line in which the match occurred, COL is the
///             column. LINE e COL are calculated
///             using the space NOT the tab.
///             The entries in the .csv must have the same order as in the
///             source code.
///             This test is robust because EACH ASTNode has a specific
///             line:col,
///             that identify the first token.
///           Visually can be verified the mutation rules seeing the
///           test_N_mutants.cpp files in which
///           the generated mutants are reported.
/// @param Mutator to test
void testMutatorMatch ( chimera::mutator::Mutator * );

template <class MutatorClass> void testMutatorMatch()
{
    MutatorClass mutator;
    testMutatorMatch ( &mutator );
}

/// @brief Run all tests
/// @param argc Like main's argc
/// @param argv Like main's argv, to configure gtest
/// @param The directory path that contains the tests
/// @return Running result: 0 OK
int runAllTest ( int argc, const char **argv, const std::string &,
                 TestingOptions o = TestingOptions() );

} // End chimera::test namespace
} // End chimera namespace

#endif /* INCLUDE_TEST_CHIMERATEST_H_ */
