/******************************************************************************
 * @file   MutationOperator.cpp
 * @author Federico Iannucci
 * @date   30 nov 2015
 * @brief  Mutation Operator Implementation File
 ******************************************************************************/

#include "Core/MutationOperator.h"

using namespace llvm;
using namespace chimera::m_operator;

chimera::m_operator::MutationOperator::MutationOperator(IdType id,
                                                        std::string descr,
                                                        bool isHOM)
    : isHOM(isHOM), identifier(id), description(descr) {}
