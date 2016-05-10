//===- Utils.h ----------------------------------------------------*- C++ -*-===//
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
/// \file Utils.h
/// \author Federico Iannucci
/// \brief This file contains a utility functions
//===----------------------------------------------------------------------===//

#ifndef SRC_INCLUDE_UTILS_H_
#define SRC_INCLUDE_UTILS_H_

#include "llvm/ADT/Twine.h"
#include "llvm/Support/raw_os_ostream.h"

#include <map>
#include <vector>
#include <string>

namespace chimera {
namespace conf {

/// @brief For each function name contains the vector of operator to apply.
/// The FunOp configuration file is a csv file, in which the first element of
/// every row is the
/// name of a function that should be present in the source code, the others
/// comma-separated
/// value in the row are the operator's identifier for the correspondent
/// operator to use.
///
/// If the first element of the first row is empty, it's possible to specify
/// only the operators
/// that will be applied to all functions found.
///
/// To comment a configuration entry use -> //.
using FunOpConfMap = std::map<std::string, std::vector<std::string>>;

/// @brief Reaf a functions/operators configuration file and create a
/// FunOpConfMap.
/// @retval bool If some error is encountered.
bool readFunctionsOperatorsConfFile(const std::string &filename,
                                    FunOpConfMap &fileMap);

} // End chimera::conf namespace

///////////////////////////////////////////////////////////////////////////////
/// @brief Filesystem functions
namespace fs {

extern const char pathSep;
/// @return The os-specific path separator
char getPathSeparator();

/// @brief Get the parent directory of a path
/// @param
/// @return
std::string getParentPath(const std::string &);

/// @brief Create directories e sub-directories
/// @param path The directory to create
/// @param ignoreExisting If it has to be ignored path previous existence.
/// @return If the directory is successfully created.
bool createDirectories(const llvm::Twine &path, bool ignoreExisting = true);

void deleteDirectory(const llvm::Twine &path);

} // End chimera::fs namespace

///////////////////////////////////////////////////////////////////////////////
/// @brief Syntax checker
namespace syntax {

// bool checkSyntax(std::string sourcePath, const
// clang::tooling::CompilationDatabase&);

} // end chimera::syntax namespace
} // End chimera namespace

#endif /* SRC_INCLUDE_UTILS_H_ */
