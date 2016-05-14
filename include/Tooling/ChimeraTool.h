//===- ChimeraTool.h --------------------------------------------*- C++ -*-===//
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
/// \file ChimeraTool.h
/// \author Federico Iannucci
/// \brief This file contains the class ChimeraTool
//===----------------------------------------------------------------------===//

#ifndef SRC_INCLUDE_CHIMERA_H_
#define SRC_INCLUDE_CHIMERA_H_

#include "Core/MutationOperator.h"

#include "llvm/ADT/StringMap.h"

// Forward declarations
namespace clang { namespace tooling { class CompilationDatabase; } }


namespace chimera
{
// Typedefs
using MutationOperatorPtrMap =
    ::llvm::StringMap<m_operator::MutationOperatorPtr>;
///< Map of MutationOperator ptr

struct SourcePreprocessingOptions {
    bool Preprocess : 1; ///< Preprocess the input (equivalent to -E), include
    /// expansion and reformatting
    bool ExpandMacros : 1; ///< Expand the macros in the input, include
    /// reformatting
    bool Reformat : 1; ///< Reformat the input

    SourcePreprocessingOptions() : Preprocess ( 0 ), ExpandMacros ( 0 ), Reformat ( 1 ) {}
};

/// \brief Chimera Class
/// TODO: Make singleton with helper DEFINE to add operators
/// \details The main class that exposes all functionalities
class ChimeraTool
{
public:
    /// \brief Ctor
    ChimeraTool() : compilationDatabasePtr ( nullptr ) {}

    // MutationOperator management methods
    /// \brief Register a mutation operator
    /// \param The mutation operator to register
    /// \return If succeeded, if the operator's identifier already exists the
    /// operation fails.
    bool registerMutationOperator ( m_operator::MutationOperatorPtr );

    /// \brief Unregister a mutation operator
    /// \param The identifier of the mutation operator
    /// \return If succeeded, id est the operator was registered
    bool unregisterMutationOperator ( const m_operator::IdType & );

    const MutationOperatorPtrMap &getRegisteredMutOperators();

    /// \brief Run the ChimeraTool
    /// \param argc Argument counter
    /// \param argv Arguments as passed to the main function
    /// \return status
    int run ( int argc, const char **argv );

private:
    ::clang::tooling::CompilationDatabase *compilationDatabasePtr;
    MutationOperatorPtrMap registeredOperatorMap;
};
} // End chimera namespace

#endif /* SRC_INCLUDE_CHIMERA_H_ */
