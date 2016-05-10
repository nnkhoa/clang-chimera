//===- CompilationDatabaseUtils.h -------------------------------*- C++ -*-===//
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
/// \file CompilationDatabaseUtils.h
/// \author Federico Iannucci
/// \brief This file provides functions and classes to manage compilation 
///        database
//===----------------------------------------------------------------------===//

#ifndef SRC_INCLUDE_COMPILATIONDATABASEUTILS_H_
#define SRC_INCLUDE_COMPILATIONDATABASEUTILS_H_

#include "clang/Tooling/CompilationDatabase.h"
#include "llvm/ADT/StringRef.h"

#include <string>
#include <vector>
#include <ostream>

namespace chimera {
namespace cd_utils {
// Typedefs
using CompileCommandVector = std::vector<clang::tooling::CompileCommand>;

/// @brief Dump Functions
void dump(std::ostream&, const ::clang::tooling::CompileCommand&);
void dump(std::ostream&, const CompileCommandVector&);
void dump(std::ostream&, const ::clang::tooling::CompilationDatabase&);

/// @brief Retrieve the compile commands from a compilation database given a filepath.
/// @param database The CompilationDabatase.
/// @param filepath The filepath to use.
/// @return The vector of compile command
CompileCommandVector getCompileCommandsByFilePath(
    const clang::tooling::CompilationDatabase&, llvm::StringRef filepath);

/// @brief Take a compile command \p command and change the target \p oldTarget to the new target \p newTarget
/// @param c Compile Command
/// @param oldTarget The old target path
/// @param newTarget The new target path
/// @param suppressWarning If add -w
/// @return true If \p command is changed
bool changeCompileCommandTarget(::clang::tooling::CompileCommand& command,
                                ::llvm::StringRef oldTarget,
                                ::std::string newTarget,
                                 bool suppressWarning = false);

/// @brief Flexible CompilationDatabase class, it's more flexible than the FixedCompilationDatabase class
/// @details Using this CompilationDatabase it's irrelevant passing a good StringRef as sourcePath during the
///          'run' call of ClangTool. As FixedCompilationDatabase it always returns on getCompileCommands(StringRef)
///          all commands.
class FlexibleCompilationDatabase : public clang::tooling::CompilationDatabase {
 public:
  /// @brief Empty constructor
  FlexibleCompilationDatabase() {
  }

  /// @brief Build a compilationDatabase from a single file and compileCommands
  FlexibleCompilationDatabase(const std::string& file,
                              const CompileCommandVector& compileCommands)
      : compileCommands(compileCommands) {
    this->files.push_back(file);
  }

  /// @brief Build a CompilationDatabase from compileCommands
  FlexibleCompilationDatabase(const CompileCommandVector& compileCommands)
      : compileCommands(compileCommands) {
  }

  /// @brief Build a CompilationDatabase from a single compileCommand
  FlexibleCompilationDatabase(
      const clang::tooling::CompileCommand& compileCommand) {
    this->compileCommands.push_back(compileCommand);
  }

  /// @brief Build a CompilationDatabase from another one
  FlexibleCompilationDatabase(
      const clang::tooling::CompilationDatabase& database) {
    this->files = database.getAllFiles();
    this->compileCommands = database.getAllCompileCommands();
  }
  /// \brief Returns all compile commands in which the specified file was
  /// compiled.
  ///
  /// This includes compile commands that span multiple source files.
  /// For example, consider a project with the following compilations:
  /// $ clang++ -o test a.cc b.cc t.cc
  /// $ clang++ -o production a.cc b.cc -DPRODUCTION
  /// A compilation database representing the project would return both command
  /// lines for a.cc and b.cc and only the first command line for t.cc.
  virtual CompileCommandVector getCompileCommands(
      llvm::StringRef FilePath) const {
    return this->compileCommands;
  }

  /// \brief Returns the list of all files available in the compilation database.
  virtual std::vector<std::string> getAllFiles() const {
    return this->files;
  }

  /// \brief Returns all compile commands for all the files in the compilation
  /// database.
  ///
  /// FIXME: Add a layer in Tooling that provides an interface to run a tool
  /// over all files in a compilation database. Not all build systems have the
  /// ability to provide a feasible implementation for \c getAllCompileCommands.
  virtual CompileCommandVector getAllCompileCommands() const {
    return CompileCommandVector();
  }
 private:
  std::vector<std::string> files;
  CompileCommandVector compileCommands;
};
}  // end chimera::cd_utils namespace
}  // end chimera namespace

#endif /* SRC_INCLUDE_COMPILATIONDATABASEUTILS_H_ */
