/******************************************************************************
 * @file   Chimera.cpp
 * @author Federico Iannucci
 * @date   01 dic 2015
 * @brief  Chimera Implementation file
 ******************************************************************************/

#include "Log.h"
#include "Core/MutationTemplate.h"
#include "Testing/ChimeraTest.h"
#include "Tooling/ChimeraTool.h"
#include "Tooling/CompilationDatabaseUtils.h"
#include "Tooling/FrontendActions.h"

#include "clang/Tooling/CommonOptionsParser.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Debug.h"

#include <iostream>
#include <string>
#include <vector>
//#include "../../include/Operators/FLAP/Operators.h"

using namespace chimera;

/// \addtogroup CHIMERA_CHIMERATOOL_CL_OPTIONS Command Line Options
/// \{
// Overview
const char *overview =
    "Without -generate-mutants the tool does a 'simulation' creating only the "
    "report.csv in <output_dir>/mutants/<source_filename>/ directory\n";

// Categories
::llvm::cl::OptionCategory catChimera("clang-chimera general options");

// Options
::llvm::cl::opt<bool> optDebug("debug", ::llvm::cl::desc("Enable debug output"),
                               ::llvm::cl::ValueDisallowed,
                               ::llvm::cl::cat(catChimera),
                               ::llvm::cl::init(false));
::llvm::cl::opt<::std::string>
    optDebugOnly("debug-only", ::llvm::cl::desc("Enable a debug class"),
                 ::llvm::cl::ValueRequired,
                 ::llvm::cl::value_desc("debug-class"),
                 ::llvm::cl::cat(catChimera), ::llvm::cl::init(""));

// Preprocess
enum PreprocessLevel {
  None,              ///< None preprocess
  ReformatOnly,      ///< Only reformat
  ExpandMacros,      ///< Reformat and expand macros
  CompletePreprocess ///< Complete preprocessing
};
::llvm::cl::opt<PreprocessLevel> optPreprocessLevel(
    "preprocess", ::llvm::cl::desc("Perfom the specified preprocess action to "
                                   "create the input file. The result will be "
                                   "saved in <output_dir>/resources"),
    ::llvm::cl::values(
        clEnumValN(
            ReformatOnly, "reformat",
            "Reformat the code only, to match the line:col of the reports"),
        clEnumValN(ExpandMacros, "expand-macros", "Reformat and expand macros"),
        clEnumValN(CompletePreprocess, "E",
                   "Preprocess as the -E compile option"),
        clEnumValEnd),
    ::llvm::cl::cat(catChimera), ::llvm::cl::init(::PreprocessLevel::None));

// Modifiers
::llvm::cl::opt<bool> optVerbose("v", ::llvm::cl::desc("Enable verbose output"),
                                 ::llvm::cl::ValueDisallowed,
                                 ::llvm::cl::cat(catChimera),
                                 ::llvm::cl::init(false));
::llvm::cl::opt<bool> optShowFunDef(
    "show-fun-def",
    ::llvm::cl::desc("Show the functions definition in the input file"),
    ::llvm::cl::ValueDisallowed, ::llvm::cl::cat(catChimera),
    ::llvm::cl::init(false));
::llvm::cl::opt<bool>
    optGenerateMutants("generate-mutants",
                       ::llvm::cl::desc("Enable the mutants generation"),
                       ::llvm::cl::ValueDisallowed, ::llvm::cl::cat(catChimera),
                       ::llvm::cl::init(false));
::llvm::cl::opt<bool> optNotGenerateReport(
    "no-generate-report",
    ::llvm::cl::desc("Disable the generation of the report"),
    ::llvm::cl::ValueDisallowed, ::llvm::cl::cat(catChimera),
    ::llvm::cl::init(false));
::llvm::cl::opt<::std::string> optFunOpConfFile(
    "fun-op", ::llvm::cl::desc(
                  "The configuration file for functions/operations filtering"),
    ::llvm::cl::ValueRequired, ::llvm::cl::value_desc("file"),
    ::llvm::cl::cat(catChimera), ::llvm::cl::init(""));
::llvm::cl::opt<::std::string> optOutputDir(
    "o", ::llvm::cl::desc("The output directory, default: ./chimera_output/"),
    ::llvm::cl::ValueRequired, ::llvm::cl::value_desc("dir-path"),
    ::llvm::cl::cat(catChimera), ::llvm::cl::init("./chimera_output"));
::llvm::cl::opt<::std::string> optCompilationDatabaseDir(
    "cd-dir", ::llvm::cl::desc("Set the directory in which search for "
                               "compile_commmands.json. Passing this option "
                               "make useless trying to use the -- at the end "
                               "to pass compile commands manually."),
    ::llvm::cl::ValueRequired, ::llvm::cl::value_desc("dir-path"),
    ::llvm::cl::cat(catChimera), ::llvm::cl::init(""));

::llvm::cl::opt<::std::string> optExecuteTest(
    "execute-test",
    ::llvm::cl::desc("Execute tests. This option disables the source input, "
                     "only other options will be accepted."),
    ::llvm::cl::ValueRequired, ::llvm::cl::value_desc("test-dir"),
    ::llvm::cl::cat(catChimera), ::llvm::cl::init(""));

::llvm::cl::opt<bool>
    optShowOperators("show-op",
                     ::llvm::cl::desc("Show the supported Mutation Operators"),
                     ::llvm::cl::ValueDisallowed, ::llvm::cl::cat(catChimera),
                     ::llvm::cl::init(false));

// Utility functions
bool optIsOccured(const ::std::string &optString, int argc, const char **argv) {
  for (int i = 0; i < argc; ++i) {
    if (argv[i] == ("-" + optString)) {
      return true;
    }
  }
  return false;
}
/// \}

/// \{
// Test
// CHIMERA_MUTATOR_MATCH_TEST(::chimera::MutatorIfConditionTrueFalse,
//                           mutator_if_condition_true_false);
// CHIMERA_MUTATOR_MATCH_TEST(::chimera::MutatorMemberExprStuckData,
//                           mutator_member_expr_stuck_data);
// CHIMERA_MUTATOR_MATCH_TEST(::chimera::MutatorArrayRefStuckData,
//                           mutator_array_ref_stuck_data);
// CHIMERA_MUTATOR_MATCH_TEST(::chimera::MutatorIntegerRefStuckData,
//                           mutator_int_ref_stuck_data);
// CHIMERA_MUTATOR_MATCH_TEST(::iideaa::FLAPFloatOperationMutator, prova);
/// \}

bool chimera::ChimeraTool::registerMutationOperator(
    ::chimera::m_operator::MutationOperatorPtr op) {
  std::pair<typename MutationOperatorPtrMap::iterator, bool> retval =
      this->registeredOperatorMap.insert(
          std::pair<m_operator::IdType,
                    ::chimera::m_operator::MutationOperatorPtr>(
              op->getIdentifier(), std::move(op)));
  return retval.second;
}

bool chimera::ChimeraTool::unregisterMutationOperator(
    const m_operator::IdType &id) {
  return this->registeredOperatorMap.erase(id);
}

const MutationOperatorPtrMap &
chimera::ChimeraTool::getRegisteredMutOperators() {
  return this->registeredOperatorMap;
}

int chimera::ChimeraTool::run(int argc, const char **argv) {
  // Initialization
  // Init the ChimeraLogger
  ::chimera::log::ChimeraLogger::init();

  // Arguments Parsing
  // If there aren't, show the help
  if (argc == 1) {
    argc = 2;
    const char *help[2] = {argv[0], "-help"};
    ::clang::tooling::CommonOptionsParser helpOption(argc, help, catChimera,
                                                     overview);
  }
  
  // Arguments that ends the execution
  if (optIsOccured(optShowOperators.ArgStr, argc, argv)){
    ::llvm::outs() << "Supported Operators:\n";
    for (const auto& op : this->getRegisteredMutOperators()){
      ::llvm::outs() << " * " << op.getValue()->getIdentifier() << "\n";
    }
    return 0;
  }

  ///////////////////////////////////////////////////////////////////////////////
  // These options must appear as first argument, and sources aren't needed
  if (optIsOccured(optExecuteTest.ArgStr, argc, argv)) {
    // When -execute-tests occurs the positional argument MUST NOT BE passed.
    llvm::cl::ParseCommandLineOptions(argc, argv, overview);
    chimera::log::ChimeraLogger::info(
        "*** Chimera will now perform the tests [use -v for verbose]***");
    return ::chimera::testing::runAllTest(argc, argv, optExecuteTest);
  }
  ///////////////////////////////////////////////////////////////////////////////
  // From now on the source input is required
  const char **argvv;
  // If compilationDatabaseDir occur, the CommonOptionsParser haven't to search
  // for compile_commands.json
  if (optIsOccured(optCompilationDatabaseDir.ArgStr, argc, argv)) {
    chimera::log::ChimeraLogger::info("A custom compilation database search "
                                      "path has been specified. Skipping "
                                      "default searching.");
    // Copy argv in argvv
    argvv = (const char **)new const char *[argc + 1];
    for (int i = 0; i < argc; ++i) {
      argvv[i] = argv[i];
    }
    // If is passed add a "--" to the end to prevent error from the
    // CommonOptionsParser.
    argvv[argc] =
        "--"; // Avoid to check for compile_commands.json in the directory tree
    argc++;
  } else {
    argvv = argv;
  }

  // In any case call the CommonOptionsParser
  ::clang::tooling::CommonOptionsParser op(argc, argvv, catChimera, overview);

  if (optCompilationDatabaseDir != "") {
    // Free allocated resources
    delete[] argvv;
  }

#ifndef NDEBUG
  // Debug option
  if (optDebug) {
    ::llvm::DebugFlag = true;
  }
  if (optDebugOnly != "") {
    ::llvm::DebugFlag = true;
    ::llvm::setCurrentDebugType((const char *)optDebugOnly.c_str());
  }
#endif

  // Execute test
  if (optExecuteTest != "") {
    return chimera::testing::runAllTest(argc, argv, optExecuteTest);
  }

  // Init the verbose output
  if (optVerbose) {
    chimera::log::ChimeraLogger::initVerbose();
    chimera::log::ChimeraLogger::setVerboseLevel(9);
  }

  // Output directory
  std::string outputPath =
      clang::tooling::getAbsolutePath((::std::string)optOutputDir);

  // Set resources directory
  std::string resourcesOutputDir =
      outputPath + chimera::fs::pathSep + "resources" + chimera::fs::pathSep;

  // Options Specific actions
  ::std::unique_ptr<::clang::tooling::CompilationDatabase> userCDatabase;
  if (optCompilationDatabaseDir != "") {
    std::string errorMsg;
    // Try to load a compilation database from the specified directory
    userCDatabase = ::clang::tooling::CompilationDatabase::loadFromDirectory(
        (::std::string)optCompilationDatabaseDir, errorMsg);
    if (userCDatabase == NULL) {
      // If it's passed MUST BE present a compile_commands.json in the
      // directory, so
      // if it's not found show an error and stop the program.
      chimera::log::ChimeraLogger::error(errorMsg); // Show error message
      return 1;                                     // Error
    }
  }

  // Configuration file
  conf::FunOpConfMap confMap;
  if (optFunOpConfFile != "") {
    chimera::log::ChimeraLogger::verbose("Configuration file : " +
                                         (::std::string)optFunOpConfFile);
    // ifstream confFile((::std::string) optFunOpConfFile);
    // Read conf file
    if (!conf::readFunctionsOperatorsConfFile(optFunOpConfFile, confMap)) {
      chimera::log::ChimeraLogger::error("Cannot open the configuration file");
    }
  }

  // To avoid problems of directory changing during clang operations create a
  // sourceAbsolutePathList
  std::vector<std::string> sourceAbsolutePathList;
  for (auto sourcePath : op.getSourcePathList()) {
    sourceAbsolutePathList.push_back(
        clang::tooling::getAbsolutePath(sourcePath));
  }

  // Loop on SourcePaths
  ::std::vector<::std::string> sourcePaths = op.getSourcePathList();
  for (std::string sourcePath : sourceAbsolutePathList) {
    // Get the compile commands for the sourcePath
    ::chimera::cd_utils::CompileCommandVector commands;
    if (optCompilationDatabaseDir != "") {
      // The previous error check make safe this instruction
      commands = chimera::cd_utils::getCompileCommandsByFilePath(*userCDatabase,
                                                                 sourcePath);
    } else {
      commands = chimera::cd_utils::getCompileCommandsByFilePath(
          op.getCompilations(), sourcePath);
    }
#ifdef _CHIEMERA_DEBUG_
    ::chimera::cd_utils::dump(::std::cout, commands);
#endif
    // Check the emptiness
    if (commands.empty()) {
      chimera::log::ChimeraLogger::warning(
          "Compile command not found. Skipping " + sourcePath);
      continue; // Skip this iteration
    }

    // Prepare inputs for the MutationTemplate
    ::clang::tooling::CompileCommand command = commands[0];

    ///////////////////////////////////////////////////////////////////////////////
    /// Add options/arguments
    // Add -w to suppress warning
    command.CommandLine.push_back("-w");
    command.CommandLine.push_back("-fsyntax-only");
    command.CommandLine.push_back(
        "-Qunused-arguments"); // suppress warnings on command line arguments

    // FIXME Some Bug, could not find stddef.h
    command.CommandLine.push_back("-I/usr/lib/clang/3.7.0/include/");

    ///////////////////////////////////////////////////////////////////////////////
    // The command for the sourcePath is ready!
    // Check source preprocessing
    if (optPreprocessLevel != PreprocessLevel::None) {
      PreprocessLevel l = optPreprocessLevel;
      ::chimera::log::ChimeraLogger::verboseAndIncr(
          "[ RUN  ] Preprocessing source file");
      // For sure will be saved a preprocessed file version in resources
      // directory

      // Variable needed to create a raw_fd_ostream
      ::std::error_code errorCode;
      ::std::string filepath =
          resourcesOutputDir + llvm::sys::path::filename(sourcePath).data();

      // Create output directory
      if (::chimera::fs::createDirectories(resourcesOutputDir)) {
        // Open raw_fd_stream for the preprocessed-version fo the file
        ::llvm::raw_fd_ostream preprocessSourceFileStream(
            filepath, errorCode, llvm::sys::fs::F_Text);
        // Check which type of preprocessing
        if (l == PreprocessLevel::CompletePreprocess) {
          ::chimera::log::ChimeraLogger::verbose(
              "Applying complete preprocessing");
          ::chimera::preprocessIncludeAction(preprocessSourceFileStream,
                                             command, sourcePath);
        } else if (l == PreprocessLevel::ExpandMacros) {
          ::chimera::log::ChimeraLogger::verbose("Applying macro expansion");
          ::chimera::expandMacrosAction(preprocessSourceFileStream, command,
                                        sourcePath);
        } else {
          assert(l == PreprocessLevel::ReformatOnly);
          ::chimera::log::ChimeraLogger::verbose("Applying reformatting");
          ::chimera::reformatAction(preprocessSourceFileStream, command,
                                    sourcePath);
        }
        // Close file stream
        preprocessSourceFileStream.close();
        chimera::log::ChimeraLogger::verbosePreDecr(
            "[ DONE ] Preprocessing source file");

        // A different version for the sourcePath has been created, modify
        // command and sourcePath
        ::chimera::cd_utils::changeCompileCommandTarget(command, sourcePath,
                                                        filepath);
        // Modify the sourcePath
        sourcePath = filepath;

        chimera::log::ChimeraLogger::verbose(
            "[ RUN  ] Performing syntax check on preprocessed file");
        // Some times the Macro Expander corrupt the file so check the syntax
        int syntaxCheckResult =
            ::chimera::checkSyntaxAction(command, sourcePath);
        if (syntaxCheckResult != 0) {
          chimera::log::ChimeraLogger::fatal(
              "[ FAIL ] Performing syntax check on preprocessed file\nThis "
              "could happen for apparently no reason with the macro-expansion, "
              "the macro-expander sometimes could not properly manage "
              "comments, see the the first error message, if this is the case, "
              "modify the source file in order to use this option.\nSorry for "
              "the inconvenient.");
          return 1;
        } else {
          chimera::log::ChimeraLogger::verbose(
              "[ PASS ] Performing syntax check on preprocessed file");
        }
        chimera::log::ChimeraLogger::decrActualVLevel();
      } else {
        chimera::log::ChimeraLogger::fatal(
            "Could not create the resources directory.");
        return 1; // Error
      }
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// The command for this source file is ready, can perform FrontendAction
    if (optShowFunDef) {
      std::cout << "Function Definitions found : " << std::endl;
      return ::chimera::functionDefAction(llvm::outs(), command, sourcePath);
    }
///////////////////////////////////////////////////////////////////////////////

#ifdef _CHIMERA_DEBUG_
    chimera::cd_utils::dump(std::cout, command);
#endif
    chimera::MutationTemplate t(command, sourcePath,
                                outputPath + chimera::fs::pathSep + "mutants");

    // Loop on registered operators
    const chimera::MutationOperatorPtrMap &map = this->registeredOperatorMap;
    for (auto it = map.begin(); it != map.end(); ++it) {
      t.loadOperator(it->second.get());
    }

    // Set if generate the mutatns or only the report
    t.setGenerateMutants(optGenerateMutants);
    t.setGenerateMutantsReport(!optNotGenerateReport);
    // Analyze template
    if (optFunOpConfFile != "") {
      t.analyze(confMap);
    } else {
      t.analyze();
    }
  }
  return 0;
}
