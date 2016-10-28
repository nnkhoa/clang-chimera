//===- Operators.cpp ---------------------------------------------*- C++-*-===//
//
//  Copyright (C) 2015, 2016  Antonio Tammaro (ntonjeta@autistici.org)
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
/// \author Antonio Tammaro  
/// \brief This file contains loop perforation operators
//===----------------------------------------------------------------------===//

#include "Operators/LoopPerforation/Mutators.h"
#include "Operators/LoopPerforation/Operator.h"

::std::unique_ptr<::chimera::m_operator::MutationOperator>
chimera::perforation::getPERFORATIONOperator()
{
  ::std::unique_ptr<::chimera::m_operator::MutationOperator> Op(
      new ::chimera::m_operator::MutationOperator(
          "LoopPerforationOperator",   // Operator identifier
          "Define operator for loop perforation tecnique", // Description
          true)
      );

  // Add mutators to the current operator
  Op->addMutator(
      ::chimera::m_operator::MutatorPtr(new ::chimera::perforation::MutatorLoopPerforation()));

  // Return the operator
  return Op;
}
