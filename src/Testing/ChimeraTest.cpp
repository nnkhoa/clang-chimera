/******************************************************************************
 * @file   ChimeraTest.cpp
 * @author Federico Iannucci
 * @date   28 nov 2015
 * @brief  Using the Google C++ Test Framework this file provides testing
 *utility functions
 ******************************************************************************/

#include "Testing/ChimeraTest.h"

#include "Log.h"
#include "Utils.h"
#include "clang/AST/AST.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "llvm/Support/FileSystem.h"

#include "lib/csv.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

//#include <boost/filesystem.hpp>

using namespace std;
using namespace clang::ast_matchers;
using namespace chimera::fs;
using namespace chimera::mutator;
using namespace chimera::log;

#define LOG_TEST_(msg)                                                         \
  if (options.verbose) {                                                       \
    ::std::cout << "[ CHIMERA  ] " << msg << ::std::endl;                      \
  }

static ::chimera::testing::TestingOptions options;
static std::string testDirectory;

int chimera::testing::runAllTest(int argc, const char **argv,
                                 const std::string &testDir, TestingOptions o) {
  ::testing::InitGoogleTest(&argc, const_cast<char **>(argv));
  testDirectory = testDir;
  options = o;
  ChimeraLogger::info("Searching tests in " + testDirectory);
  return RUN_ALL_TESTS();
}

///////////////////////////////////////////////////////////////////////////////
/// MatchCallback
using TestCallbackResultType = ::std::vector<::std::vector<::std::string>>;
using TestCallbackResultEntry = ::std::vector<::std::string>;
class MutatorMatchingTestCallback : public MatchFinder::MatchCallback {
public:
  MutatorMatchingTestCallback(::llvm::raw_ostream &out, Mutator &m)
      : out(out), mutator(m) {}
  /**
   * @brief Run implementation.
   */
  virtual void run(const MatchFinder::MatchResult &Result) {
    ::clang::ast_type_traits::DynTypedNode matched_node;
    if (mutator.getMatchedNode(Result, matched_node)) {
      // Create a rewriter
      clang::Rewriter rw(*Result.SourceManager, Result.Context->getLangOpts());
      // Create a full resource
      clang::FullSourceLoc matchedNodeLoc(
          matched_node.getSourceRange().getBegin(), rw.getSourceMgr());
      LOG_TEST_("\tCoarse grain match : " +
                rw.getRewrittenText(matched_node.getSourceRange()) + " at " +
                ::to_string(matchedNodeLoc.getSpellingLineNumber()) + ":" +
                ::to_string(matchedNodeLoc.getSpellingColumnNumber()));
      // Apply fine grain matching rule
      if (mutator.match(Result)) {
        LOG_TEST_("\t\tFine grain match : PASS");

        // Save result entry
        TestCallbackResultEntry resultEntry{
            ::to_string(matchedNodeLoc.getSpellingLineNumber()),
            ::to_string(matchedNodeLoc.getSpellingColumnNumber())};
        results.push_back(resultEntry);

        bool syntaxCheckResult = false;
        for (MutatorType type = 0; type < this->mutator.getTypes(); ++type) {
          // Create a rewriter
          clang::Rewriter rw2(*Result.SourceManager,
                              Result.Context->getLangOpts());

          this->mutator.mutate(Result, type, rw2);
          // Put the mutants on out
          out << "////////////////////////////// START MUTANT "
                 "////////////////////////////\n";
          out << "// MUTATION_LOCATION: " << resultEntry[0] << ":"
              << resultEntry[1] << "\n";
          out << "// MUTATION_TYPE: " << type << "\n";
          ::std::string mutant;
          ::llvm::raw_string_ostream mutantStream(mutant);
          rw2.getEditBuffer(rw2.getSourceMgr().getMainFileID())
              .write(mutantStream);
          out << mutantStream.str();

          clang::FrontendAction *syntaxCheck = new clang::SyntaxOnlyAction;
          out << "//SYNTAX_CHECK: ";
          syntaxCheckResult =
              ::clang::tooling::runToolOnCode(syntaxCheck, mutantStream.str());
          if (syntaxCheckResult) {
            out << "PASS";
          } else {
            out << "FAIL";
          }
          ASSERT_TRUE(syntaxCheckResult)
              << "Actual mutant doesn't pass the syntax check";
          out << "\n////////////////////////////// END MUTANT "
                 "//////////////////////////////\n";
        }
      } else {
        LOG_TEST_("\t\tFine grain match : FAILED");
      }
    }
  }

  const TestCallbackResultType &getResults() { return this->results; }

private:
  ::llvm::raw_ostream &out;
  Mutator &mutator;
  TestCallbackResultType results;
};

void chimera::testing::testMutatorMatch(chimera::mutator::Mutator *mutator) {
  ::chimera::mutator::Mutator &m = *mutator;
  // Load N sources from .cpp file in mutators/<mutator_identifier>/test_N.cpp
  unsigned int testNum = 0;
  int toolRetVal = -1;
  LOG_TEST_("Start Mutator Testing - " + m.getIdentifier());
  std::string test_file_directory(testDirectory + "mutators" + pathSep +
                                  m.getIdentifier() + pathSep);
  std::string test_file_path;

  do {
    test_file_path = test_file_directory + "test_" + to_string(testNum);
    // Check file existence
    if (::llvm::sys::fs::exists(test_file_path + ".cpp")) {
      LOG_TEST_("Running on file - " + test_file_path);
      // Exists at least one file, open it and load it into a string
      std::ifstream test_file(test_file_path + ".cpp");
      std::string test_file_source(static_cast<std::stringstream const &>(
                                       std::stringstream() << test_file.rdbuf())
                                       .str());
      // Close the file
      test_file.close();

      ///////////////////////////////////////////////////////////////////////////////
      /// Check test file syntax
      clang::FrontendAction *syntaxCheck = new clang::SyntaxOnlyAction();
      if (!clang::tooling::runToolOnCode(syntaxCheck, test_file_source,
                                         "test.cpp")) {
        chimera::log::ChimeraLogger::error(
            "The test file IS NOT syntactically correct. Skipping this file.");

        // FIXME Freeing syntaxCheck produce segfault
      } else {
        // Create mutation output
        ::std::string mutationOutputFilePath = test_file_path + "_mutants.cpp";
        ::std::error_code errorCode;
        ::llvm::raw_fd_ostream mutationOutputStream(
            mutationOutputFilePath, errorCode, ::llvm::sys::fs::F_Text);

        if (mutationOutputStream.has_error()) {
          ::chimera::log::ChimeraLogger::fatal(
              "Error occurred in opening " + mutationOutputFilePath +
              ". Error message: " + errorCode.message());
        }

        // Create a matchFinder, load the mutator on it and run
        MatchFinder finder;
        // Create a MatchCallback
        MatchFinder::MatchCallback *callback =
            new MutatorMatchingTestCallback(mutationOutputStream, m);
        switch (m.getMatcherType()) {
        case StatementMatcherType:
          finder.addMatcher(m.getStatementMatcher(), callback);
          break;
        case DeclarationMatcherType:
          finder.addMatcher(m.getDeclarationMatcher(), callback);
          break;
        case TypeMatcherType:
          finder.addMatcher(m.getTypeMatcher(), callback);
          break;
        case TypeLocMatcherType:
          finder.addMatcher(m.getTypeLocMatcher(), callback);
          break;
        case NestedNameSpecifierMatcherType:
          finder.addMatcher(m.getNestedNameSpecifierMatcher(), callback);
          break;
        case NestedNameSpecifierLocMatcherType:
          finder.addMatcher(m.getNestedNameSpecifierLocMatcher(), callback);
          break;
        default:
          // Error!
          break;
        }

        clang::FrontendAction *finderAction =
            clang::tooling::newFrontendActionFactory(&finder)->create();
        toolRetVal = clang::tooling::runToolOnCode(
            finderAction, test_file_source, "test.cpp");
        ASSERT_TRUE(toolRetVal) << "Running failure";

        // Get results from the callback
        TestCallbackResultType results =
            ((MutatorMatchingTestCallback *)callback)->getResults();

        try {
          // Read from csv file and compare the result
          io::CSVReader<2, io::trim_chars<' '>,
                        io::double_quote_escape<',', '\"'>>
              oracle(test_file_path + "_match.csv");
          ::std::string line;
          ::std::string col;

          TestCallbackResultType oracleResults;
          while (oracle.read_row(line, col)) {
            TestCallbackResultEntry e{line, col};
            oracleResults.push_back(e);
          }
          // Firs check the number of entries
          ASSERT_EQ(oracleResults.size(), results.size())
              << "Number of matching mismatch. More matching than expected.";

          for (size_t i = 0; i < results.size(); ++i) {
            // Compare
            EXPECT_EQ(oracleResults.at(i), results.at(i))
                << "Result's entry mismatch";
          }
        } catch (::io::error::can_not_open_file &e) {
          ChimeraLogger::info("Oracle file NOT FOUND. Skipping.");
        }
      }
      // Increase file test number
      testNum++;
    } else
      testNum = 0;
    // testNum should be increased during the elaboration of test_0 so this
    // condition is met only if
    // a test_N.cpp file isn't found
  } while (testNum != 0);
  LOG_TEST_("Finish Mutator Testing - " + m.getIdentifier());
}
