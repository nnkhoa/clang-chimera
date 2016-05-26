//===- MutatorsTesting.h ----------------------------------------*- C++ -*-===//
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
/// \file MutatorsTesting.h
/// \author Federico Iannucci
/// \brief This file is used to test mutators.
///	   It is recommended to insert here the calls to the testing framework
//===----------------------------------------------------------------------===//

#ifndef INCLUDE_TESTING_MUTATORS_TESTING_H_
#define INCLUDE_TESTING_MUTATORS_TESTING_H_

#include "Testing/ChimeraTest.h"

// Include the header in which mutators are defined
#include "Operators/Examples/Mutators.h"

/// \addtogroup MUTATORS_TESTING Test cases for the Sample Mutators
/// \{
// Test mutators
CHIMERA_MUTATOR_MATCH_TEST ( ::chimera::examples::MutatorGreaterOpReplacement,mutator_greater_op_replacement );
/// \}

#endif /* INCLUDE_TESTING_MUTATORS_TESTING_H_ */