//===- MutationOperator.h ---------------------------------------*- C++ -*-===//
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
/// \file MutationOperator.h
/// \author Federico Iannucci
/// \date 14 ott 2015 
/// \brief This file contains the class Mutation Operator
//===----------------------------------------------------------------------===//

#ifndef INCLUDE_MUTATION_OPERATOR_H_
#define INCLUDE_MUTATION_OPERATOR_H_

#include "Core/Mutator.h"

#include <memory>
#include <string>
#include <vector>

namespace chimera {
namespace m_operator {
// Forward declarations
class MutationOperator;

// Typedefs
using IdType = ::std::string;
using MutationOperatorPtr = std::unique_ptr<m_operator::MutationOperator>;
///< Mutation Operator id type
using MutatorPtr = ::std::shared_ptr<::chimera::mutator::Mutator>;
using MutatorPtrVector = ::std::vector<MutatorPtr>;
///< MutatorPtr vector, returned by some MutationOperator's methods

/// @brief    Mutation Operator class, it groups mutators. It is used to
/// localize inside the functions
///           the modifications.
///           As for the mutator can be of FOM or HOM type.
/// @details  FOM - It is the default type, it localizes the modifications of
/// its mutators inside a function,
///           specified by a configuration file. A FOM operator can contain HOM
///           mutator.
///           TODO: HOM mutators inside a FOM operator are accumulated.
///
///           HOM - A HOM operator widens the modifications among all the
///           functions for which is specified.
///           It MUST have all HOM mutators (a FOM mutator would not make
///           sense), those mutators that generally
///           would generate a mutant for each of them, they are going to create
///           an overall mutant, which is
///           associated with the operator itself.
///
///           If an operator has all HOM mutators DOES NOT imply that is a HOM
///           operator, it is an imposed property.
class MutationOperator {
public:
  // Ctor
  MutationOperator(IdType id, std::string descr = "", bool isHOM = false);

  // Getter and Setter
  const std::string &getDescription() const { return description; }

  void setDescription(const std::string &description) {
    this->description = description;
  }

  const IdType &getIdentifier() const { return identifier; }

  const MutatorPtrVector &getMutators() const { return mutators; }

  const bool isHom() const { return this->isHOM; }

  /**
   * @brief Add a mutator to the operator
   * @param mutator The mutator to add
   */
  void addMutator(MutatorPtr mutator) {
    this->mutators.push_back(mutator);
    // Check the mutator type
    assert((!this->isHOM || mutator->isHom()) &&
           "An HOM operator MUST have HOM mutators");
  }

private:
  const bool isHOM;
  const IdType identifier;   ///< Identifier
  std::string description;   ///< Description
  MutatorPtrVector mutators; ///< Mutators of this operator
};
} // End chimera::m_operator namespace
} // End chimera namespace

#endif /* INCLUDE_MUTATION_OPERATOR_H_ */
