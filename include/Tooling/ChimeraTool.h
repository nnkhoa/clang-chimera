/******************************************************************************
 * @file   Chimera.h
 * @author Federico Iannucci
 * @date   28 ott 2015
 * @brief  ChimeraTool header
 ******************************************************************************/
#ifndef SRC_INCLUDE_CHIMERA_H_
#define SRC_INCLUDE_CHIMERA_H_

#include "Core/MutationOperator.h"

#include "clang/Tooling/CompilationDatabase.h"
#include "llvm/ADT/StringMap.h"

#include <memory>

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
