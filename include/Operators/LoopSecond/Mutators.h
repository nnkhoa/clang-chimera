//===- Mutators.h -----------------------------------------------*- C++ -*-===//
//
//  Copyright (C) 2015, 2016 Antonio Tammaro  (ntonjeta@autistici.org)
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
/// \file Mutators.h
/// \author Antonio Tammaro
///// \brief This file contains loop perforation mutators
//===----------------------------------------------------------------------===//

#ifndef INCLUDE_OPERATORS_PERFORATION_MUTATORS_H
#define INCLUDE_OPERATORS_PERFORATION_MUTATORS_H

#include "Core/Mutator.h"

using namespace clang;
using namespace clang::ast_matchers;

namespace chimera
{
namespace perforation
{

/// \addtogroup OPERATORS_SAMPLE_MUTATORS Sample Mutators
/// \{

/**
 * @brief This mutator try to apply loop perforation technique for approximate computing
 *        others (at the moment -).
 */
class MutatorLoopPerforation2 : public chimera::mutator::Mutator
{
  //TODO ADD report functionality
  struct MutationInfo {
    ::std::string opId;  ///< Operation Identifier
    unsigned line;  ///< Occurrence line
    ::std::string inc;
  };

  public:
    /**
     * @brief Constuctor
     */
    MutatorLoopPerforation2()
        : Mutator ( ::chimera::mutator::StatementMatcherType, // A binary operator is a statement
                    "mutator_loop_perforation_operator", // String identifier
                    "loop perforation", // Description
                    1,
                    true),cond(nullptr),init(nullptr),opId(0) { }
    virtual clang::ast_matchers::StatementMatcher getStatementMatcher() override; // Need to override this method, first part of matching rules
    virtual bool match ( const ::chimera::mutator::NodeType &node ) override; // Also this one, second part of matching rules
    virtual bool getMatchedNode ( const chimera::mutator::NodeType &,
                                  clang::ast_type_traits::DynTypedNode & ) override; // This is pure virtual and must be implemented
    virtual clang::Rewriter &mutate ( const chimera::mutator::NodeType &node,
                                      mutator::MutatorType type,
                                      clang::Rewriter &rw ) override; // mutation rulesi

    virtual void onCreatedMutant(const ::std::string &mutantPath) override;
  private: 
    const ::clang::BinaryOperator *cond; // < Retrive ForStmt condition  
    const ::clang::BinaryOperator *init; 
    unsigned int opId; //< Counter to keep tracks of done mutations
    const ::clang::UnaryOperator *inc; // < Retrive ForStmt increment  
    const ::clang::BinaryOperator *binc; // < Retrive ForStmt increment in case of binary increment
    const ::clang::BinaryOperator *bas; // < Retrive ForStmt increment in case of binary increment
  ::std::vector<MutationInfo> mutationsInfo;  ///< It maintains info about mutations, in order to be saved
};

} // end namespace chimera::perforation
} // end namespace chimera

#endif /* INCLUDE_OPERATORS_PERFORATION_MUTATORS_H */
