//===- Operator.h -----------------------------------------------*- C++ -*-===//
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
/// \file Operator.h
/// \author Mario Barbareschi
/// \brief This file contains operators definition for VPA
//===----------------------------------------------------------------------===//

#ifndef INCLUDE_IIDEAA_VPANOPERATOR_H_
#define INCLUDE_IIDEAA_VPANOPERATOR_H_

#include "Core/MutationOperator.h"

namespace chimera {
namespace vpa_nmutator{

  /// @brief VPAN Operation mutator
  /// @brief Create and return the IIDEAA operator
  /// @return the IIDEAA operator
  std::unique_ptr<::chimera::m_operator::MutationOperator> getVPANOperator();

}  // End namespace vpan
}  // End namespace chimera
#endif /* INCLUDE_IIDEAA_VPANOPERATOR_H_ */
