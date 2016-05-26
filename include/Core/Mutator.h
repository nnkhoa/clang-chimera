//===- Mutator.h ------------------------------------------------*- C++ -*-===//
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
/// \file Mutator.h
/// \author Federico Iannucci
/// \date 14 ott 2015
/// \brief This file contains the abstract class Mutator
//===----------------------------------------------------------------------===//

#ifndef INCLUDE_MUTATOR_H_
#define INCLUDE_MUTATOR_H_

#include "clang/AST/ASTTypeTraits.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"

#include <string>
#include <vector>

namespace chimera
{
namespace mutator
{

// Exported types
using NodeType = clang::ast_matchers::MatchFinder::MatchResult;
using MutatorType = unsigned short int;
///< Mutator's mutation type

/// @brief Mutator's match type
enum MatcherType {
    DeclarationMatcherType,
    StatementMatcherType,
    TypeMatcherType,
    TypeLocMatcherType,
    NestedNameSpecifierMatcherType,
    NestedNameSpecifierLocMatcherType
};

/**
 * @brief   Mutator Class, mutate the source code (translation unit) when it's
 * compliant to the matching rules.
 *          To mutate the code it's used a Rewrite object, that represents the
 * AST of a translation unit.
 *          The mutator can be a First Order Mutation (FOM) or High Order (HOM).
 * The default is First Order.
 * @details Some times the mutation operation to be synactically valid requires additional
 * directive to the compiler, there
 *          is a method to accomplish that.
 *          FOM - A FOM mutator implies that each modification (a call to
 * mutate) is isolated, i.e. when it is valid
 *          it generates, for each mutation type, a mutant.
 *          HOM - A HOM mutator implies that each modification (a call to
 * mutate) is accumulated, and if ONE of them
 *          is valid, it generates a mutant with all the modifications.
 *          An HOM mutator has mutation type equals to 0. If mutation types are
 * needed, multiple mutators
 *          have to be created, one for each of them.
 */
class Mutator
{
public:
    /// @brief Constructor
    /// @param matcherType The type of the matcher of this mutator.
    explicit Mutator ( MatcherType matcherType, std::string identifier = "",
                       std::string description = "", MutatorType types = 0,
                       bool isHOM = false )
        : matcherType ( matcherType ), isHOM ( isHOM ), identifier ( identifier ),
          description ( description ), types ( types ) {
        assert ( ( !isHOM || types == 1 ) &&
                 "HOM mutator MUST have zero mutation types" );
    }

    // Virtual destructor
    virtual ~Mutator() {}

    // Getter and Setter
    /**
     * @{
     */
    const std::string &getDescription() const {
        return description;
    }

    void setDescription ( const std::string &description ) {
        this->description = description;
    }

    const std::string &getIdentifier() const {
        return identifier;
    }

    void setIdentifier ( const std::string &identifier ) {
        this->identifier = identifier;
    }

    MutatorType getTypes() const {
        return types;
    }

    void setTypes ( MutatorType types ) {
        this->types = types;
    }

    const MatcherType getMatcherType() const {
        return matcherType;
    }

    const bool isHom() const {
        return isHOM;
    }

    /// @brief Return the local additional compile commands
    /// @return string Containing valid clang compile commands
    const ::std::vector<::std::string> &getAdditionalCompileCommands() {
        return this->additionalCompileCommands;
    }

    /**
     * @}
     */

    /**
     * @{
     */
    /**
     * @brief Get the matcher. Default anything
     */
    virtual clang::ast_matchers::StatementMatcher getStatementMatcher() {
        return clang::ast_matchers::anything();
    }
    virtual clang::ast_matchers::DeclarationMatcher getDeclarationMatcher() {
        return clang::ast_matchers::anything();
    }
    virtual clang::ast_matchers::TypeMatcher getTypeMatcher() {
        return clang::ast_matchers::anything();
    }
    virtual clang::ast_matchers::TypeLocMatcher getTypeLocMatcher() {
        return clang::ast_matchers::anything();
    }
    virtual clang::ast_matchers::NestedNameSpecifierMatcher
    getNestedNameSpecifierMatcher() {
        return clang::ast_matchers::anything();
    }
    virtual clang::ast_matchers::NestedNameSpecifierLocMatcher
    getNestedNameSpecifierLocMatcher() {
        return clang::ast_matchers::anything();
    }

    /**
     * @brief If the matcher doesn't cover the matching logic. Fine grained
     * matching logic.
     * @retval bool The node is matched or less. Default true
     */
    virtual bool match ( const NodeType &node ) {
        return true;
    }
    /**
     * @}
     */

    /**
     * @{
     */
    /**
     * @brief  Mutate a node with type
     * @param node The node to mutate
     * @param type The type of mutation
     * @retval Rewriter& The Rewriter obj with code modification
     */
    virtual clang::Rewriter &mutate ( const NodeType &node, MutatorType type,
                                      clang::Rewriter &rw ) = 0;

    /**
     * @brief Clean a previous mutation
     * @param node The node to mutate
     * @param type The type of mutation
     * @retval none
     */
    virtual void clean ( const NodeType &node, MutatorType type ) {}

    /// @brief Get the matched DynTyped Node
    /// @details This function abstract the binding information of the MatchResult
    /// that are mutator specific.
    ///          Example :
    ///           const BinaryOperator* assignOp =
    ///           node.Nodes.getNodeAs<BinaryOperator>(
    ///           "assignment_op");
    ///           if (assignOp != nullptr) {
    ///             dynNode = ast_type_traits::DynTypedNode::create(*assignOp);
    ///             return true;
    ///           } else
    ///             return false;
    /// TODO: Create a helper macro?
    /// @return If true the MatchResult contained a valid matched node, if false
    /// DynTypedNode isn't modified.
    virtual bool getMatchedNode ( const NodeType &,
                                  clang::ast_type_traits::DynTypedNode & ) = 0;

    ///
    /// @{
    // Callbacks

    /// @brief Called when a mutant has been successfully created applying the
    /// mutate method.
    ///        For a FOM mutator is called after each mutate call.
    ///        For a HOM mutator is called after all the mutate calls for a
    ///        mutant.
    /// @param mutantPath
    virtual void onCreatedMutant ( const ::std::string &mutantPath ) {}

    /// @brief It is called at the end of the translation unit, after all the
    /// mutants creation
    /// @param dirPath Path to the directory in which artifacts can be saved
    virtual void onEndOfTranslationUnit ( const ::std::string &dirPath ) {}
    ///@}

    /**
     * @}
     */
protected:
    ::std::vector<::std::string> additionalCompileCommands;

private:
    const MatcherType matcherType; ///< Mutator matcher type
    const bool isHOM; ///< If this mutator is an High Order Mutator, i.e
    /// accumulate each mutation
    ::std::string identifier;  ///< Mutator identifier
    ::std::string description; ///< Mutator description
    MutatorType types;         ///< Number of mutations supported
};
} // End mutator namespace
} // End chimera namespace

#endif /* INCLUDE_MUTATOR_H_ */
