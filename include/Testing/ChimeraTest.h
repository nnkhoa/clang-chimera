/******************************************************************************
 * @file   ChimeraTest.h
 * @author Federico Iannucci
 * @date   28 nov 2015
 * @brief  Using the Google C++ Test Framework this file provides testing
 *         utility functions.
 ******************************************************************************/
#ifndef INCLUDE_TEST_CHIMERATEST_H_
#define INCLUDE_TEST_CHIMERATEST_H_

#include "Core/Mutator.h"

#include "lib/gtest/gtest.h" ///< Include gtest.h to use Google Test Framework

namespace chimera {
namespace testing {

/// @defgroup CHIMERA_TEST_TYPES Chimera Test Types
/// \{
struct TestingOptions {
  TestingOptions() : verbose(false) {}
  bool verbose : 1; // Enable verbose output
};
/// \}

#define CHIMERA_MUTATOR_MATCH_TEST(mutator_class_name, mutator_identifier)     \
  TEST(mutator_identifier, mutator_test) {                                     \
    ::chimera::testing::testMutatorMatch<mutator_class_name>();                \
  }

/// @brief Test a mutator
/// @details  To test a mutator it should be tested the matching rules and the
/// mutation rules.
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
void testMutatorMatch(chimera::mutator::Mutator *);

template <class MutatorClass> void testMutatorMatch() {
  MutatorClass mutator;
  testMutatorMatch(&mutator);
}

/// @brief Run all tests
/// @param argc Like main's argc
/// @param argv Like main's argv, to configure gtest
/// @param The directory path that contains the tests
/// @return Running result: 0 OK
int runAllTest(int argc, const char **argv, const std::string &,
               TestingOptions o = TestingOptions());

} // End chimera::test namespace
} // End chimera namespace

#endif /* INCLUDE_TEST_CHIMERATEST_H_ */
