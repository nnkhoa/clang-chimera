

#ifndef INCLUDE_OPERATORS_PERFORATION2_OPERATORS_H
#define INCLUDE_OPERATORS_PERFORATION2_OPERATORS_H

#include "Core/MutationOperator.h"

namespace chimera
{
namespace perforation 
{

// @brief Create and return the Loop Perforation Operator
::std::unique_ptr<::chimera::m_operator::MutationOperator> getPerforationSecondOperator();


} // end namespace chimera::perforation
} // end namespace chimera

#endif /* INCLUDE_OPERATORS_PERFORATION_OPERATORS_H */

