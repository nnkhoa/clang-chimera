//===- Operators.cpp -----------------------------------------------*- C++ -*-===//
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
/// \file Operators.cpp
/// \author Mario Barbareschi
/// \brief This file contains operators implementation for VPA
//===----------------------------------------------------------------------===//

#include "Operators/VPA/Operator.h"
#include "Operators/VPA/Mutators.h"

std::unique_ptr<::chimera::m_operator::MutationOperator> 
chimera::vpamutator::getVPAOperator() {
  std::unique_ptr<::chimera::m_operator::MutationOperator> Op(
      new ::chimera::m_operator::MutationOperator(
        "VPAOperator",
        "IIDEAA Code Instrumentation",
        true)
      );

  // Add mutators to the current operator
  Op->addMutator(
      ::chimera::m_operator::MutatorPtr(new ::chimera::vpamutator::VPAFloatOperationMutator()));

  return Op;
}
