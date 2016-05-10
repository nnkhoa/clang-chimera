//===- Operators.h ----------------------------------------------*- C++ -*-===//
//
//  Copyright (C) 2015, 2016  Federico Iannucci (fed.iannucci@gmail.com)
//  All Rights Reserved
//
//  This file is part of Clang-Chimera.
//
//  Unauthorized copying of this file, via any medium is strictly prohibited
//  Proprietary and confidential
//
//===----------------------------------------------------------------------===//
/// \file Operators.h
/// \author Federico Iannucci
/// \brief This files contains FLAP Operators
//===----------------------------------------------------------------------===//

#ifndef INCLUDE_IIDEAA_FLAPOPERATOR_H_
#define INCLUDE_IIDEAA_FLAPOPERATOR_H_

#include "Core/MutationOperator.h"

namespace iideaa
{
///// @brief FLAP VarDecl mutator
//class FLAPVarDecl : public ::chimera::mutator::Mutator {
// public:
//  FLAPVarDecl()
//      : Mutator(::chimera::mutator::DeclarationMatcherType, "mutator_IIDEAA",
//                "Instruments the code for IIDEAA.", 1, true) {
//  }
//  /// @brief Matching rules :
//  ///        -
//  /// @return
//  virtual ::clang::ast_matchers::DeclarationMatcher getDeclarationMatcher()
//      override;
//  virtual bool match(const ::chimera::mutator::NodeType& node) override;
//  virtual bool getMatchedNode(const ::chimera::mutator::NodeType&,
//                              clang::ast_type_traits::DynTypedNode&) override;
//  virtual ::clang::Rewriter& mutate(const ::chimera::mutator::NodeType& node,
//                                    ::chimera::mutator::MutatorType type,
//                                    clang::Rewriter& rw) override;
// private:
//  ::std::vector<::clang::SourceLocation> modifiedLocations;
//};

/// @brief FLAP Operation mutator
class FLAPFloatOperationMutator : public ::chimera::mutator::Mutator
{
protected:
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
    FLAPFloatOperationMutator()
        : Mutator ( ::chimera::mutator::StatementMatcherType,
                    "mutator_flap_operation", "Instruments the code for IIDEAA.", 1,
                    true ),
        operationCounter ( 0 ) {
    }
    /// @brief Matching rules :
    ///        -
    /// @return
    virtual ::clang::ast_matchers::StatementMatcher getStatementMatcher()
    override;
    virtual bool match ( const ::chimera::mutator::NodeType& node ) override;
    virtual bool getMatchedNode ( const ::chimera::mutator::NodeType&,
                                  clang::ast_type_traits::DynTypedNode& ) override;
    virtual ::clang::Rewriter& mutate ( const ::chimera::mutator::NodeType& node,
                                        ::chimera::mutator::MutatorType type,
                                        clang::Rewriter& rw ) override;
    virtual void onCreatedMutant ( const ::std::string& ) override;

protected:
    unsigned int operationCounter;  ///< Counter to keep tracks of done mutations
    ::std::vector<MutationInfo> mutationsInfo;  ///< It maintains info about mutations, in order to be saved
};

class FLAPVivadoFloatOperationMutator : public FLAPFloatOperationMutator
{
public:
    FLAPVivadoFloatOperationMutator() : FLAPFloatOperationMutator() {}
    virtual ::clang::Rewriter& mutate ( const ::chimera::mutator::NodeType& node,
                                        ::chimera::mutator::MutatorType type,
                                        clang::Rewriter& rw ) override;
};

/// @brief Create and return the IIDEAA operator
/// @return the IIDEAA operator
std::unique_ptr<::chimera::m_operator::MutationOperator> getFLAPOperator();

/// @brief Create and return the FLAP Operator for Vivado
/// @return the IIDEAA operator
std::unique_ptr<::chimera::m_operator::MutationOperator> getFLAPVivadoOperator();
}  // End namespace iideaa
#endif /* INCLUDE_IIDEAA_FLAPOPERATOR_H_ */
