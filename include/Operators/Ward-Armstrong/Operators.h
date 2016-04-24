/******************************************************************************
 * @file   chimera_operators.h
 * @author Federico Iannucci
 * @date   15 ott 2015
 * @brief  Chimera operators implementation
 ******************************************************************************/
#ifndef INCLUDE_CHIMERA_OPERATORS_H_
#define INCLUDE_CHIMERA_OPERATORS_H_

#include "Core/MutationOperator.h"

namespace chimera {
/**
 * @brief Get StuckIf Operator
 */
m_operator::MutationOperatorPtr getStuckIfOperator();
/**
 * @brief Get Assignment Control Operator
 */
m_operator::MutationOperatorPtr getAssignmentControlOperator();
/**
 * @brief Get Dead Clause Operator
 */
m_operator::MutationOperatorPtr getDeadClauseOperator();
/**
 * @brief Get Dead Process Operator
 */
m_operator::MutationOperatorPtr getDeadProcessOperator();
/**
 * @brief Get Micro operation Operator
 */
m_operator::MutationOperatorPtr getMicroOperationOperator();
/**
 * @brief Get Local Stuck Data
 */
m_operator::MutationOperatorPtr getLocalStuckData();
/**
 * @brief Get Global Stuck Data
 */
m_operator::MutationOperatorPtr getGlobalStuckData();
}

#endif /* INCLUDE_CHIMERA_OPERATORS_H_ */
