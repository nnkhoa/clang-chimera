/******************************************************************************
 * @file   Mutators.h
 * @author Federico Iannucci
 * @date   16 ott 2015
 * @brief  Chimera predefined mutators
 ******************************************************************************/
#ifndef SRC_INCLUDE_CHIMERAMUTATORS_H_
#define SRC_INCLUDE_CHIMERAMUTATORS_H_

#include "Core/Mutator.h"

namespace chimera {

/**
 * @brief Mutator the changes in two way the if condition : true and false.
 */
class MutatorIfConditionTrueFalse : public chimera::mutator::Mutator {
public:
  /**
   * @brief Constuctor
   */
  MutatorIfConditionTrueFalse()
      : Mutator(mutator::StatementMatcherType,
                "mutator_if_condition_true_false", "Take a condition of an "
                                                   "IfStmt and changes it in "
                                                   "two way : true and false.",
                2) {}
  virtual clang::ast_matchers::StatementMatcher getStatementMatcher() override;
  virtual bool getMatchedNode(const chimera::mutator::NodeType &,
                              clang::ast_type_traits::DynTypedNode &) override;
  virtual clang::Rewriter &mutate(const chimera::mutator::NodeType &node,
                                  mutator::MutatorType type,
                                  clang::Rewriter &rw) override;
};

/**
 * @brief Mutator the delete an assignment instruction
 */
class MutatorAssignmentDeletion : public ::chimera::mutator::Mutator {
public:
  /**
   * @brief Constuctor
   */
  MutatorAssignmentDeletion()
      : Mutator(mutator::StatementMatcherType, "mutator_assignment_deletion",
                "Take an assignment operation and delete it", 1) {}
  virtual clang::ast_matchers::StatementMatcher getStatementMatcher() override;
  virtual bool getMatchedNode(const chimera::mutator::NodeType &,
                              clang::ast_type_traits::DynTypedNode &) override;
  virtual clang::Rewriter &mutate(const chimera::mutator::NodeType &node,
                                  mutator::MutatorType type,
                                  clang::Rewriter &rw) override;
};

/**
 * @brief Mutator that deletes the clause of a switch case
 */
class MutatorClauseDeletion : public chimera::mutator::Mutator {
public:
  /**
   * @brief Constuctor
   */
  MutatorClauseDeletion()
      : Mutator(mutator::StatementMatcherType, "mutator_clause_deletion",
                "Take a switch case and delete one by one each clause", 1) {}
  virtual clang::ast_matchers::StatementMatcher getStatementMatcher() override;
  virtual bool getMatchedNode(const chimera::mutator::NodeType &,
                              clang::ast_type_traits::DynTypedNode &) override;
  virtual clang::Rewriter &mutate(const chimera::mutator::NodeType &node,
                                  mutator::MutatorType type,
                                  clang::Rewriter &rw) override;
};

/**
 * @brief Mutator take a for-loop and forces it to execute only one time.
 */
class MutatorForSingleExecution : public chimera::mutator::Mutator {
public:
  /**
   * @brief Constuctor
   */
  MutatorForSingleExecution()
      : Mutator(mutator::StatementMatcherType, "mutator_for_single_execution",
                "Take a for-loop and forces it to execute only one time.", 1) {}
  virtual clang::ast_matchers::StatementMatcher getStatementMatcher() override;
  virtual bool getMatchedNode(const chimera::mutator::NodeType &,
                              clang::ast_type_traits::DynTypedNode &) override;
  virtual clang::Rewriter &mutate(const chimera::mutator::NodeType &node,
                                  mutator::MutatorType type,
                                  clang::Rewriter &rw) override;
};

/**
 * @brief Mutator take a while-loop and forces it to execute only one time.
 */
class MutatorWhileSingleExecution : public chimera::mutator::Mutator {
public:
  /**
   * @brief Constuctor
   */
  MutatorWhileSingleExecution()
      : Mutator(mutator::StatementMatcherType, "mutator_while_single_execution",
                "Take a while-loop and forces it to execute only one time.",
                1) {}
  virtual clang::ast_matchers::StatementMatcher getStatementMatcher() override;
  virtual bool getMatchedNode(const chimera::mutator::NodeType &,
                              clang::ast_type_traits::DynTypedNode &) override;
  virtual clang::Rewriter &mutate(const chimera::mutator::NodeType &node,
                                  mutator::MutatorType type,
                                  clang::Rewriter &rw) override;
};

/**
 * @brief Mutator take a do-while-loop and forces it to execute only one time.
 */
class MutatorDoWhileSingleExecution : public chimera::mutator::Mutator {
public:
  /**
   * @brief Constuctor
   */
  MutatorDoWhileSingleExecution()
      : Mutator(chimera::mutator::StatementMatcherType,
                "mutator_do_while_single_execution",
                "Take a do-while-loop and forces it to execute only one time.",
                1) {}
  virtual clang::ast_matchers::StatementMatcher getStatementMatcher() override;
  virtual bool getMatchedNode(const chimera::mutator::NodeType &,
                              clang::ast_type_traits::DynTypedNode &) override;
  virtual clang::Rewriter &mutate(const chimera::mutator::NodeType &node,
                                  mutator::MutatorType type,
                                  clang::Rewriter &rw) override;
};

/*************************************************************************************************/
/* Micro Operation Mutators */
/* Arithmetics */
/**
 * @brief Mutator take an addition operation and mutate it.
 */
class MutatorAdditionReplacement : public chimera::mutator::Mutator {
public:
  /**
   * @brief Constuctor
   */
  MutatorAdditionReplacement()
      : Mutator(mutator::StatementMatcherType, "mutator_addition_replacement",
                "Take a take an addition operation and mutate it.", 1) {}
  virtual clang::ast_matchers::StatementMatcher getStatementMatcher() override;
  virtual bool getMatchedNode(const chimera::mutator::NodeType &,
                              clang::ast_type_traits::DynTypedNode &) override;
  virtual clang::Rewriter &mutate(const chimera::mutator::NodeType &node,
                                  mutator::MutatorType type,
                                  clang::Rewriter &rw) override;
};

/**
 * @brief Mutator take a compound addition operation and mutate it.
 */
class MutatorCompoundAdditionReplacement : public chimera::mutator::Mutator {
public:
  /**
   * @brief Constuctor
   */
  MutatorCompoundAdditionReplacement()
      : Mutator(mutator::StatementMatcherType,
                "mutator_compound_addition_replacement",
                "Take a take a compound addition operation and mutate it.", 1) {
  }
  virtual clang::ast_matchers::StatementMatcher getStatementMatcher() override;
  virtual bool getMatchedNode(const chimera::mutator::NodeType &,
                              clang::ast_type_traits::DynTypedNode &) override;
  virtual clang::Rewriter &mutate(const chimera::mutator::NodeType &node,
                                  mutator::MutatorType type,
                                  clang::Rewriter &rw) override;
};

/**
 * @brief Mutator take an subtraction operation and mutate it.
 */
class MutatorSubtractionReplacement : public chimera::mutator::Mutator {
public:
  /**
   * @brief Constuctor
   */
  MutatorSubtractionReplacement()
      : Mutator(mutator::StatementMatcherType,
                "mutator_subtraction_replacement",
                "Take a take an subtract operation and mutate it.", 1) {}
  virtual clang::ast_matchers::StatementMatcher getStatementMatcher() override;
  virtual bool getMatchedNode(const chimera::mutator::NodeType &,
                              clang::ast_type_traits::DynTypedNode &) override;
  virtual clang::Rewriter &mutate(const chimera::mutator::NodeType &node,
                                  mutator::MutatorType type,
                                  clang::Rewriter &rw) override;
};

/**
 * @brief Mutator take an subtraction operation and mutate it.
 */
class MutatorCompoundSubtractionReplacement : public chimera::mutator::Mutator {
public:
  /**
   * @brief Constuctor
   */
  MutatorCompoundSubtractionReplacement()
      : Mutator(mutator::StatementMatcherType,
                "mutator_compound_subtraction_replacement",
                "Take a take a compound subtract operation and mutate it.", 1) {
  }
  virtual clang::ast_matchers::StatementMatcher getStatementMatcher() override;
  virtual bool getMatchedNode(const chimera::mutator::NodeType &,
                              clang::ast_type_traits::DynTypedNode &) override;
  virtual clang::Rewriter &mutate(const chimera::mutator::NodeType &node,
                                  mutator::MutatorType type,
                                  clang::Rewriter &rw) override;
};

/* Logic */
/**
 * @brief Mutator take an not logical operation and mutate it.
 */
class MutatorLogicalNotReplacement : public chimera::mutator::Mutator {
public:
  /**
   * @brief Constuctor
   */
  MutatorLogicalNotReplacement()
      : Mutator(mutator::StatementMatcherType,
                "mutator_logical_not_replacement",
                "Take a take an not logical operation and mutate it.", 1) {}
  virtual clang::ast_matchers::StatementMatcher getStatementMatcher() override;
  virtual bool getMatchedNode(const chimera::mutator::NodeType &,
                              clang::ast_type_traits::DynTypedNode &) override;
  virtual clang::Rewriter &mutate(const chimera::mutator::NodeType &node,
                                  mutator::MutatorType type,
                                  clang::Rewriter &rw) override;
};

/**
 * @brief Mutator take an and logical operation and mutate it.
 */
class MutatorLogicalAndReplacement : public chimera::mutator::Mutator {
public:
  /**
   * @brief Constuctor
   */
  MutatorLogicalAndReplacement()
      : Mutator(mutator::StatementMatcherType,
                "mutator_logical_and_replacement",
                "Take a take an and logical operation and mutate it.", 1) {}
  virtual clang::ast_matchers::StatementMatcher getStatementMatcher() override;
  virtual bool getMatchedNode(const chimera::mutator::NodeType &,
                              clang::ast_type_traits::DynTypedNode &) override;
  virtual clang::Rewriter &mutate(const chimera::mutator::NodeType &node,
                                  mutator::MutatorType type,
                                  clang::Rewriter &rw) override;
};

/**
 * @brief Mutator take an or logical operation and mutate it.
 */
class MutatorLogicalOrReplacement : public chimera::mutator::Mutator {
public:
  /**
   * @brief Constuctor
   */
  MutatorLogicalOrReplacement()
      : Mutator(mutator::StatementMatcherType, "mutator_logical_or_replacement",
                "Take a take an or logical operation and mutate it.", 1) {}
  virtual clang::ast_matchers::StatementMatcher getStatementMatcher() override;
  virtual bool getMatchedNode(const chimera::mutator::NodeType &,
                              clang::ast_type_traits::DynTypedNode &) override;
  virtual clang::Rewriter &mutate(const chimera::mutator::NodeType &node,
                                  mutator::MutatorType type,
                                  clang::Rewriter &rw) override;
};

/* Bitwise */
/**
 * @brief Mutator take an not logical operation and mutate it.
 */
class MutatorBitwiseNotReplacement : public chimera::mutator::Mutator {
public:
  /**
   * @brief Constuctor
   */
  MutatorBitwiseNotReplacement()
      : Mutator(mutator::StatementMatcherType,
                "mutator_bitwise_not_replacement",
                "Take a take an not bitwise operation and mutate it.", 1) {}
  virtual clang::ast_matchers::StatementMatcher getStatementMatcher() override;
  virtual bool getMatchedNode(const chimera::mutator::NodeType &,
                              clang::ast_type_traits::DynTypedNode &) override;
  virtual clang::Rewriter &mutate(const chimera::mutator::NodeType &node,
                                  mutator::MutatorType type,
                                  clang::Rewriter &rw) override;
};

/**
 * @brief Mutator take a bitwise and operation and mutate it.
 */
class MutatorBitwiseAndReplacement : public chimera::mutator::Mutator {
public:
  /**
   * @brief Constuctor
   */
  MutatorBitwiseAndReplacement()
      : Mutator(mutator::StatementMatcherType,
                "mutator_bitwise_and_replacement",
                "Take a take an and bitwise operation and mutate it.", 1) {}
  virtual clang::ast_matchers::StatementMatcher getStatementMatcher() override;
  virtual bool getMatchedNode(const chimera::mutator::NodeType &,
                              clang::ast_type_traits::DynTypedNode &) override;
  virtual clang::Rewriter &mutate(const chimera::mutator::NodeType &node,
                                  mutator::MutatorType type,
                                  clang::Rewriter &rw) override;
};

/**
 * @brief Mutator take a bitwise and operation and mutate it.
 */
class MutatorCompoundBitwiseAndReplacement : public chimera::mutator::Mutator {
public:
  /**
   * @brief Constuctor
   */
  MutatorCompoundBitwiseAndReplacement()
      : Mutator(mutator::StatementMatcherType,
                "mutator_compound_bitwise_and_replacement",
                "Take a take a compound and bitwise operation and mutate it.",
                1) {}
  virtual clang::ast_matchers::StatementMatcher getStatementMatcher() override;
  virtual bool getMatchedNode(const chimera::mutator::NodeType &,
                              clang::ast_type_traits::DynTypedNode &) override;
  virtual clang::Rewriter &mutate(const chimera::mutator::NodeType &node,
                                  mutator::MutatorType type,
                                  clang::Rewriter &rw) override;
};

/**
 * @brief Mutator take an or bitwise operation and mutate it.
 */
class MutatorBitwiseOrReplacement : public chimera::mutator::Mutator {
public:
  /**
   * @brief Constuctor
   */
  MutatorBitwiseOrReplacement()
      : Mutator(mutator::StatementMatcherType, "mutator_bitwise_or_replacement",
                "Take a take an or bitwise operation and mutate it.", 1) {}
  virtual clang::ast_matchers::StatementMatcher getStatementMatcher() override;
  virtual bool getMatchedNode(const chimera::mutator::NodeType &,
                              clang::ast_type_traits::DynTypedNode &) override;
  virtual clang::Rewriter &mutate(const chimera::mutator::NodeType &node,
                                  mutator::MutatorType type,
                                  clang::Rewriter &rw) override;
};

/**
 * @brief Mutator take an or bitwise operation and mutate it.
 */
class MutatorCompoundBitwiseOrReplacement : public chimera::mutator::Mutator {
public:
  /**
   * @brief Constuctor
   */
  MutatorCompoundBitwiseOrReplacement()
      : Mutator(mutator::StatementMatcherType,
                "mutator_compound_bitwise_or_replacement",
                "Take a take a compound or bitwise operation and mutate it.",
                1) {}
  virtual clang::ast_matchers::StatementMatcher getStatementMatcher() override;
  virtual bool getMatchedNode(const chimera::mutator::NodeType &,
                              clang::ast_type_traits::DynTypedNode &) override;
  virtual clang::Rewriter &mutate(const chimera::mutator::NodeType &node,
                                  mutator::MutatorType type,
                                  clang::Rewriter &rw) override;
};

/*************************************************************************************************/
/* Local Stuck Data Mutators */
/**
 * @brief Mutator take a DeclRef of Integer types and mutate it.
 * TODO : Support float types.
 */
class MutatorIntegerRefStuckData : public chimera::mutator::Mutator {
public:
  /**
   * @brief Constuctor
   */
  MutatorIntegerRefStuckData()
      : Mutator(
            mutator::StatementMatcherType, "mutator_int_ref_stuck_data",
            "Take a take a DeclRef of Integer or Float types and mutate it.",
            2) {}
  virtual bool match(const chimera::mutator::NodeType &node) override;
  virtual clang::ast_matchers::StatementMatcher getStatementMatcher() override;
  virtual bool getMatchedNode(const chimera::mutator::NodeType &,
                              clang::ast_type_traits::DynTypedNode &) override;
  virtual clang::Rewriter &mutate(const chimera::mutator::NodeType &node,
                                  mutator::MutatorType type,
                                  clang::Rewriter &rw) override;
};

/**
 * @brief Mutator take a reference to array of integer type.
 * TODO : Support others types
 */
class MutatorArrayRefStuckData : public chimera::mutator::Mutator {
public:
  /**
   * @brief Constuctor
   */
  MutatorArrayRefStuckData()
      : Mutator(mutator::StatementMatcherType, "mutator_array_ref_stuck_data",
                "Take a take a DeclRef of array type and mutate it.", 2) {}
  virtual bool match(const chimera::mutator::NodeType &node) override;
  virtual clang::ast_matchers::StatementMatcher getStatementMatcher() override;
  virtual bool getMatchedNode(const chimera::mutator::NodeType &,
                              clang::ast_type_traits::DynTypedNode &) override;
  virtual clang::Rewriter &mutate(const chimera::mutator::NodeType &node,
                                  mutator::MutatorType type,
                                  clang::Rewriter &rw) override;
};

/**
 * @brief Mutator take a reference to an integer struct/class member and mutate
 * it.
 * TODO : Support others types
 */
class MutatorMemberExprStuckData : public chimera::mutator::Mutator {
public:
  /**
   * @brief Constuctor
   */
  MutatorMemberExprStuckData()
      : Mutator(mutator::StatementMatcherType, "mutator_member_expr_stuck_data",
                "Take a take a MemberExpr and mutate it.", 2) {}
  virtual bool match(const chimera::mutator::NodeType &node) override;
  virtual clang::ast_matchers::StatementMatcher getStatementMatcher() override;
  virtual bool getMatchedNode(const chimera::mutator::NodeType &,
                              clang::ast_type_traits::DynTypedNode &) override;
  virtual clang::Rewriter &mutate(const chimera::mutator::NodeType &node,
                                  mutator::MutatorType type,
                                  clang::Rewriter &rw) override;
};
/*************************************************************************************************/
/* Global Stuck Data Mutators */
/**
 * @brief Mutator takes a variable (through its variable declaration) and make
 * not changing its value.
 */
class MutatorVariableLatch : public chimera::mutator::Mutator {
public:
  /**
   * @brief Constuctor
   */
  MutatorVariableLatch()
      : Mutator(
            mutator::DeclarationMatcherType, "mutator_variable_latch",
            "Take a take a DeclRef of Integer or Float types and mutate it.",
            1) {}
  virtual clang::ast_matchers::DeclarationMatcher
  getDeclarationMatcher() override;
  virtual bool getMatchedNode(const chimera::mutator::NodeType &,
                              clang::ast_type_traits::DynTypedNode &) override;
  virtual clang::Rewriter &mutate(const chimera::mutator::NodeType &node,
                                  mutator::MutatorType type,
                                  clang::Rewriter &rw) override;
};
/*************************************************************************************************/

} // End chimera namespace

#endif /* SRC_INCLUDE_CHIMERAMUTATORS_H_ */
