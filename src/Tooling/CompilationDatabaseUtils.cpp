//===- CompilationDatabaseUtils.cpp -----------------------------*- C++ -*-===//
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
/// \file CompilationDatabaseUtils.cpp
/// \author Federico Iannucci
/// \brief This file provides functions and classes to manage compilation 
///        database
//===----------------------------------------------------------------------===//

#include "Log.h"
#include "Utils.h"
#include "Tooling/CompilationDatabaseUtils.h"

#include "llvm/Support/Path.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/ADT/Twine.h"

using namespace chimera::log;
using namespace llvm;
using namespace clang::tooling;

void chimera::cd_utils::dump(
    std::ostream &out, const clang::tooling::CompileCommand &compileCommand) {
  out << "Command directory: " << compileCommand.Directory << std::endl;
  out << "Command line: ";
  for (auto c = compileCommand.CommandLine.begin();
       c != compileCommand.CommandLine.end(); ++c) {
    out << *c << " ";
  }
  out << std::endl;
}
void chimera::cd_utils::dump(std::ostream &out,
                             const CompileCommandVector &commands) {
  // Loop on compile commad
  for (auto compileCommand = commands.begin(); compileCommand != commands.end();
       ++compileCommand) {
    dump(out, *compileCommand);
  }
}

/// @brief Dump a CompilationDatabase
/// @param
/// @param
void chimera::cd_utils::dump(
    std::ostream &out, const clang::tooling::CompilationDatabase &database) {
  const CompileCommandVector commands = database.getAllCompileCommands();
  if (commands.empty()) {
    // Maybe a FixedCompilationDatabase
    dump(out, database.getCompileCommands("filepath"));
  } else
    dump(out, database.getAllCompileCommands());
}

chimera::cd_utils::CompileCommandVector
chimera::cd_utils::getCompileCommandsByFilePath(
    const CompilationDatabase &database, StringRef filename) {
  // First search for the correct "file"
  Twine targetFilePath = Twine(filename);
  std::string foundFilePath;
  ChimeraLogger::verboseAndIncr("Retrieving compileCommands for " +
                                targetFilePath.str());
  // Find the foundFilename to access specific compileCommands
  // Get all "file" field of the compilation database
  auto compileFile = database.getAllFiles();
  // Check if it's empty
  if (!compileFile.empty()) {
    // This isn't a FixedCompilationDatabase
    for (auto file = compileFile.begin(); file != compileFile.end(); ++file) {
      // Compare with targetFilename
      Twine filePathToCompare(*file);
      ChimeraLogger::verbose("Comparing with File: " + *file);
      // llvm::sys::fs::equivalent to test the really equivalence
      if (llvm::sys::fs::equivalent(targetFilePath, filePathToCompare)) {
        ChimeraLogger::verbose("Successful. Match found!");
        foundFilePath = *file;
        break;
      }
    }
  } else {
    // This is a FixedCompilationDatabase
    ChimeraLogger::verbose(
        "CompilationDatabase with an empty filelist. Maybe provided by hand");
    foundFilePath = targetFilePath.str();
  }
  ChimeraLogger::decrActualVLevel();
  // Return compileCommands
  return database.getCompileCommands(foundFilePath);
}

bool chimera::cd_utils::changeCompileCommandTarget(
    ::clang::tooling::CompileCommand &command, ::llvm::StringRef oldTarget,
    ::std::string newTarget, bool suppressWarnings) {
  ::chimera::log::ChimeraLogger::verboseAndIncr(
      "[ RUN  ] Adapting compile command");
  bool commandChanged = false;

  // Modify the target of the compile command and add the -I parent
  for (auto commandLineElem = command.CommandLine.begin();
       commandLineElem != command.CommandLine.end(); ++commandLineElem) {
    // This check is safe, because in this case same name means same file (a
    // filtering has been done before),
    // also because in the command the relativeness of the paths are different.
    if (::llvm::sys::path::filename(*commandLineElem) ==
        ::llvm::sys::path::filename(oldTarget)) {
      //::chimera::log::ChimeraLogger::verbose("Target found: " +
      //*commandLineElem + " by " + oldTarget.str());
      // Element found.
      commandChanged = true;
      *commandLineElem = newTarget; // Change the compile command target
    }
  }
  // Add the target parent directory as -I parameter (to resolve local
  // includes).
  command.CommandLine.push_back("-I" + ::chimera::fs::getParentPath(oldTarget));

  if (suppressWarnings) {
    command.CommandLine.push_back("-w");
  }
  ::chimera::log::ChimeraLogger::verbosePreDecr(
      "[ DONE ] Adapting compile command");
  return commandChanged;
}
