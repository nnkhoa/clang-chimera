/******************************************************************************
 * @file   OperatorIIDEAA.cpp
 * @author Federico Iannucci
 * @date   24 gen 2016
 * @brief  TODO
 ******************************************************************************/

#include "Operators/FLAP/Operator.h"
#include "Operators/FLAP/Mutators.h"

std::unique_ptr<::chimera::m_operator::MutationOperator> 
chimera::flapmutator::getFLAPOperator() {
  std::unique_ptr<::chimera::m_operator::MutationOperator> Op(
      new ::chimera::m_operator::MutationOperator(
        "FLAPOperator",
        "IIDEAA Code Instrumentation",
        true)
      );

  // Add mutators to the current operator
  Op->addMutator(
      ::chimera::m_operator::MutatorPtr(new ::chimera::flapmutator::FLAPFloatOperationMutator()));

  return Op;
}
