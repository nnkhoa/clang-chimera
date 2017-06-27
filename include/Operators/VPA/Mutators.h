//===- Mutators.h -----------------------------------------------*- C++ -*-===//
//
//  Copyright (C) 2017  Mario Barbareschi (mario.barbareschi@unina.it)
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
/// \author Mario Barbareschi
/// \brief This file contains mutators definition for VPA
//===----------------------------------------------------------------------===//

#ifndef INCLUDE_OPERATORS_VPA_MUTATORS_H
#define INCLUDE_OPERATORS_VPA_MUTATORS_H

#include "Core/Mutator.h"


namespace chimera
{
  namespace vpamutator
  {

    /// @brief VPA Operation mutator
    class VPAFloatOperationMutator : public ::chimera::mutator::Mutator {
      ::clang::BinaryOperatorKind NoOp = ::clang::BinaryOperatorKind::BO_Comma;
      struct MutationInfo {
        ::std::string opId;  ///< Operation Identifier
        unsigned line;  ///< Occurrence line
        ::std::string opRetTy;  ///< Operation Return Type
        ::clang::BinaryOperatorKind opTy;  ///< Operation Type
        ::std::string op1;  ///< Operand 1
        ::clang::BinaryOperatorKind op1OpTy;  ///< It is != NoOp if operand 1 is a binary operation
        ::std::string op2;  ///< Operand 2
        ::clang::BinaryOperatorKind op2OpTy;  ///< It is != NoOp if operand 2 is a binary operation
        ::std::string retOp;  ///< Operand which eventually is returned
      };
     public:
      VPAFloatOperationMutator()
          : Mutator(::chimera::mutator::StatementMatcherType,
                    "mutator_vpa_operation", "Instruments the code for IIDEAA.", 1,
                    true),
            operationCounter(0) {
      }
      /// @brief Matching rules :
      ///        -
      /// @return
      virtual ::clang::ast_matchers::StatementMatcher getStatementMatcher()
          override;
      virtual bool match(const ::chimera::mutator::NodeType& node) override;
      virtual bool getMatchedNode(const ::chimera::mutator::NodeType&,
                                  clang::ast_type_traits::DynTypedNode&) override;
      virtual ::clang::Rewriter& mutate(const ::chimera::mutator::NodeType& node,
                                        ::chimera::mutator::MutatorType type,
                                        clang::Rewriter& rw) override;
      virtual void onCreatedMutant(const ::std::string&) override;

     private:
      unsigned int operationCounter;  ///< Counter to keep tracks of done mutations
      ::std::vector<MutationInfo> mutationsInfo;  ///< It maintains info about mutations, in order to be saved
    };


  } // end namespace vpa 

} // end namespace chimera

#endif /* INCLUDE_OPERATORS_VPA_MUTATORS_H */
