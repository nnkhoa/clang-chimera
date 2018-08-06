//===- Mutators.cpp -----------------------------------------------*- C++ -*-===//
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
/// \file Mutators.cpp
/// \author Mario Barbareschi
/// \brief This file contains mutators implemenration for VPA
//===----------------------------------------------------------------------===//

#include "Operators/VPA/Operator.h"
#include "Operators/VPA/Mutators.h"

#include "Log.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include <iostream>

#define DEBUG_TYPE "mutator_iideaa"

using namespace clang;
using namespace clang::ast_matchers;
using namespace chimera::log;
using namespace std;
using namespace chimera;
using namespace chimera::mutator;
///////////////////////////////////////////////////////////////////////////////
// Vpa operation mutator

#define XHS_INTERNAL_MATCHER(id)                                               \
  ignoringParenImpCasts(expr().bind(                                           \
      id)) //< x hand side matcher without parenthesis and implicit casts

// Retrieve hs without implicit cast and parenthesis,
// also bypassing explicit casting, this is done arriving to the explicit cast
// and then going on the child (has), where the above rule is applied again
#define XHS_MATCHER(type, id)                                                  \
  allOf(hasType(qualType(hasCanonicalType(asString(type)))),                   \
        anyOf(XHS_INTERNAL_MATCHER(id),                                        \
              ignoringParenImpCasts(                                           \
                  castExpr(has(expr(XHS_INTERNAL_MATCHER(id)))))))

static ::std::string mapOpCode(::clang::BinaryOperator::Opcode code) {
  ::std::string retString = "";
  switch (code) {
  case BO_Add:
  case BO_AddAssign:
    retString = "ADD";
    break;
  case BO_Sub:
  case BO_SubAssign:
    retString = "SUB";
    break;
  case BO_Mul:
  case BO_MulAssign:
    retString = "MUL";
    break;
  case BO_Div:
  case BO_DivAssign:
    retString = "DIV";
    break;
  default:
    llvm_unreachable("OpCode unsupported");
  }
  return retString;
}

static ::std::string castVpaFloat(const ::std::string &xhs, const ::std::string &precId) {
  return "::vpa::VPA(" + xhs + ", " + precId + ")";
}

/// @brief Apply the casting logic on a hand side
/// @param rw Rewriter
/// @param xhs Hand side to cast
/// @param type Cast type
/// @param precId VPA specific parameter
static void castVpaFloat(Rewriter &rw, const Expr *xhs,
                          const ::std::string &precId) {
  SourceRange range = xhs->getSourceRange();
    rw.InsertTextBefore(range.getBegin(), "::vpa::VPA(");
    rw.InsertTextAfterToken(range.getEnd(), ", " + precId + ")");
}

::clang::ast_matchers::StatementMatcher
chimera::vpamutator::VPAFloatOperationMutator::getStatementMatcher() {
  // Match any float/double binary operation, leave to the match fine grain the
  // specific operation types
  return stmt(
      // Match the Bop
      anyOf(binaryOperator(hasLHS(XHS_MATCHER("float", "lhs")),
                           hasRHS(XHS_MATCHER("float", "rhs")))
                .bind("floatOp"),
            binaryOperator(hasLHS(XHS_MATCHER("double", "lhs")),
                           hasRHS(XHS_MATCHER("double", "rhs")))
                .bind("doubleOp"),
                binaryOperator(hasLHS(XHS_MATCHER("double", "lhs")),
                               hasRHS(XHS_MATCHER("float", "rhs")))
                .bind("doubleOp"),
                binaryOperator(hasLHS(XHS_MATCHER("float", "lhs")),
                               hasRHS(XHS_MATCHER("double", "rhs")))
                .bind("doubleOp")),
      // The form anyOf(,anything()) allows to TRY a match with a matcher
      // anything() put as last allows to try each matcher without the need to
      // match any of them
      // Retrieve a possible assignment which the bop is RHS
      anyOf(hasAncestor(
                binaryOperator(hasOperatorName("=")).bind("externalAssignOp")),
            anything()),
      anyOf(hasAncestor(
                varDecl().bind("varDeclAssign")),
            anything()),
      //          ignoringParenImpCasts(
      //              hasParent(
      //                  castExpr(
      //                      hasParent(
      //                          binaryOperator(hasOperatorName("=")).bind(
      //                              "externalAssignOp")))))),),
      anyOf(
          // Retrieve possible controlStmts in the surroundings
          hasAncestor(forStmt().bind("forStmt")),
          hasAncestor(whileStmt().bind("whileStmt")),
          hasAncestor(doStmt().bind("doStmt")),
          hasAncestor(ifStmt().bind("ifStmt")), anything()),
      // Unless it is:
      //  - Inside a function call
      //  - Inside an arraySubscript
      unless(
          anyOf(hasAncestor(callExpr()), hasAncestor(arraySubscriptExpr()))));
}

static const BinaryOperator *getBop(const ::chimera::mutator::NodeType &node) {
  const BinaryOperator *bop = node.Nodes.getNodeAs<BinaryOperator>("floatOp");
  if (bop == nullptr) {
    bop = node.Nodes.getNodeAs<BinaryOperator>("doubleOp");
  }
  assert(bop && "BinaryOperator is nullptr");
  return bop;
}

bool chimera::vpamutator::VPAFloatOperationMutator::match(
    const ::chimera::mutator::NodeType &node) {
  // Remove all unsupported binary operation
  // Retrieve the node
  const BinaryOperator *bop = getBop(node);

  if (bop->getOpcode() == BO_Add || bop->getOpcode() == BO_AddAssign ||
      bop->getOpcode() == BO_Sub || bop->getOpcode() == BO_SubAssign ||
      bop->getOpcode() == BO_Mul || bop->getOpcode() == BO_MulAssign ||
      bop->getOpcode() == BO_Div || bop->getOpcode() == BO_DivAssign) {
    // Consider the control stmts in the surroundings, the operation could be
    // deep in the condition expression
    // Simply control if the bop DOES NOT START within the body of the control
    // smts,
    // that is is in the "control" part
    SourceRange bopRange = bop->getSourceRange();
    // IF a construct has been matched
    // If stmt
    const IfStmt *ifStmt = node.Nodes.getNodeAs<IfStmt>("ifStmt");
    if (ifStmt != nullptr) {
      if (bopRange.getBegin() < ifStmt->getCond()->getSourceRange().getEnd()) {
        return false;
      }
    }
    return true;
  } else {
    return false;
  }
}

bool chimera::vpamutator::VPAFloatOperationMutator::getMatchedNode(
    const ::chimera::mutator::NodeType &node,
    clang::ast_type_traits::DynTypedNode &dynNode) {
  const BinaryOperator *bop = getBop(node);
  if (bop != nullptr) {
    dynNode = ast_type_traits::DynTypedNode::create(*bop);
    return true;
  } else
    return false;
}

::clang::Rewriter &chimera::vpamutator::VPAFloatOperationMutator::mutate(
    const ::chimera::mutator::NodeType &node,
    ::chimera::mutator::MutatorType type, clang::Rewriter &rw) {
  // Common operations
  const FunctionDecl *funDecl =
      node.Nodes.getNodeAs<FunctionDecl>("functionDecl");
  // Set the operation number
  unsigned int bopNum = this->operationCounter++;
  // Local rewriter to holds the original code
  Rewriter oriRw(*(node.SourceManager), node.Context->getLangOpts());

  // Retrieve binary operation, left and right hand side
  ::std::string opRetType = "float";
  const BinaryOperator *bop = node.Nodes.getNodeAs<BinaryOperator>("floatOp");
  if (bop == nullptr) {
    opRetType = "double";
    bop = node.Nodes.getNodeAs<BinaryOperator>("doubleOp");
  }
    
  // bop = (const BinaryOperator*) bop->IgnoreCasts();
  const Expr *internalLhs = node.Nodes.getNodeAs<Expr>("lhs");
  const Expr *internalRhs = node.Nodes.getNodeAs<Expr>("rhs");
  const Expr *lhs = bop->getLHS()->IgnoreCasts();
  const Expr *rhs = bop->getRHS()->IgnoreCasts();

  assert(bop && "BinaryOperator is nullptr");
  assert(internalLhs && "LHS is nullptr");
  assert(internalRhs && "RHS is nullptr");

  ::std::string opId = "OP_" + ::std::to_string(bopNum);
  // TODO: Add operation type
  // Add to the additional compile commands
  //  this->additionalCompileCommands.push_back("-D" + opId);

  ::std::string lhsString = rw.getRewrittenText(lhs->getSourceRange());
  ::std::string rhsString = rw.getRewrittenText(rhs->getSourceRange());

  ////////////////////////////////////////////////////////////////////////////////////////////
  /// Debug
  DEBUG(::llvm::dbgs() << "****************************************************"
                          "****\nDump binary operation:\n");
  // bop->dump();
  DEBUG(::llvm::dbgs() << "Operation: "
                       << rw.getRewrittenText(bop->getSourceRange()) << " ==> ["
                       << bop->getOpcodeStr() << "]\n");
  DEBUG(::llvm::dbgs() << "LHS: " << lhsString << "\n");
  // lhs->dumpColor();
  DEBUG(::llvm::dbgs() << "RHS: " << rhsString << "\n");
  // rhs->dumpColor();
  ////////////////////////////////////////////////////////////////////////////////////////////

  // Create a global var before the function
  if (opRetType == "float") {
    rw.InsertTextBefore(funDecl->getSourceRange().getBegin(),
                        "::vpa::FloatingPointPrecision " + opId + " = ::vpa::float_prec;\n");
  } else {
    rw.InsertTextBefore(funDecl->getSourceRange().getBegin(),
                        "::vpa::FloatingPointPrecision " + opId + " = ::vpa::double_prec;\n");
  }

  bool isLhsBinaryOp = ::llvm::isa<BinaryOperator>(internalLhs);
  bool isRhsBinaryOp = ::llvm::isa<BinaryOperator>(internalRhs);
  ::std::string retVar = "NULL";
    
  // Manage CompoundAssign that are automatically of II type
  if (bop->isCompoundAssignmentOp()) {
    // Expand the compound assignment

    DEBUG(::llvm::dbgs() << "Compound Operation: II Type"
                         << "\n");

    ::std::string op_char;
    switch (bop->getOpcode()) {
    case BO_AddAssign:
      op_char = '+';
      break;
    case BO_SubAssign:
      op_char = '-';
      break;
    case BO_MulAssign:
      op_char = '*';
      break;
    case BO_DivAssign:
      op_char = '/';
      break;
    default:
      llvm_unreachable("OpCode isn't supported");
    }
    
      //castVpaFloat(rw, rhs, opId);
      std::string prova = rw.getRewrittenText(SourceRange(rhs->getSourceRange().getEnd()));
            rw.InsertTextBefore(rhs->getSourceRange().getEnd().getLocWithOffset(prova.size()), ", "+opId+"))");

      rw.InsertTextAfter(rhs->getSourceRange().getBegin(), "::vpa::VPA(");
      
      rw.ReplaceText(bop->getOperatorLoc(), "= ("+ opRetType +")("+castVpaFloat(lhsString, opId)+" "+op_char );//+ " /*CMP*/::vpa::VPA(");

          
      /*
    if (isRhsBinaryOp) {
      bopReplacement += rhsString +")";
    } else {
      bopReplacement += castVpaFloat(rhsString, opId) + ")";
    }*/

    // Apply replacement
    //rw.ReplaceText(bop->getSourceRange(), bopReplacement);

    // In this case the retVar is the LHS
    if (::llvm::isa<DeclRefExpr>(internalLhs)) {
      retVar = ((const DeclRefExpr *)(internalLhs))
                   ->getNameInfo()
                   .getName()
                   .getAsString();
    }
  } else {
    // Characterize the operation: I, II, III level
        if (isLhsBinaryOp == isRhsBinaryOp) {
          // I or III type
          DEBUG(::llvm::dbgs() << "I or III type"
                               << "\n");

          // #AGB start: get cast
          
          const Expr *lhsCast = bop->getLHS();
          const Expr *rhsCast = bop->getRHS();

          const CastExpr *LCE = dyn_cast<CastExpr>(lhsCast);
          const CastExpr *RCE = dyn_cast<CastExpr>(rhsCast);
          
          const char* rhsStmClass = rhs->getStmtClassName();

          if (LCE != nullptr){
            DEBUG(::llvm::dbgs() << "Casting Detected\n"
                                 << "LHS Cast Type: "
                                 << LCE->getCastKindName()
                                 << "\n"
                                 << rw.getRewrittenText(lhsCast->getSourceRange())
                                 << "\n"
                                 << rw.getRewrittenText(rhsCast->getSourceRange())
                                 << "\n");
          }

          if (rhsStmClass != NULL){
            DEBUG(::llvm::dbgs() << "Statement Class: "
                                 << rhsStmClass
                                 << "\n");
          }
          // #AGB end

          // Apply replacements
        //castVpaFloat(rw, lhs, opId);
        
        // #AGB if has cast, replace the text from cast position, otherwise just insert it

        if ((LCE != nullptr) && (strcmp(LCE->getCastKindName(), "NoOp") == 0)){
          DEBUG(::llvm::dbgs() << "NoOp cast \n");
          rw.ReplaceText(lhsCast->getSourceRange().getBegin(), "::vpa::VPA((");
        }else{  
          rw.InsertTextAfter(lhs->getSourceRange().getBegin(), "::vpa::VPA(");
        }

        // #Original
        // rw.InsertTextAfter(lhs->getSourceRange().getBegin(), "::vpa::VPA(");
        
        rw.InsertTextBefore(bop->getOperatorLoc(), ", "+opId+")");
            
            std::string prova = rw.getRewrittenText(SourceRange(rhs->getSourceRange().getEnd()));
            rw.InsertTextBefore(rhs->getSourceRange().getEnd().getLocWithOffset(prova.size()), ", "+opId+")");
        
        // #AGB if there is a Literal of any kind (integer, float, etc.) and it's a Macro on the RHS of a BinOperator, 
          // it will return an empty string when RHS get called
          // Thus, cannot insert anything after that, resulting in missing parenthesis
          // To work around this, a check is made, if RHS has Literal Statement Class and the return value is empty, 
          // a closing parenthesis is 
          // inserted before the BinOperator

          if((strstr(rhsStmClass, "Literal") != NULL) && (rhs->getLocStart().isMacroID() )) {
            DEBUG(::llvm::dbgs() << "Macro Detected\n");            
            rw.InsertTextAfter(bop->getOperatorLoc(), ")");
          }

          // DEBUG(::llvm::dbgs() << "Debug - Literal: \n"
          //                      << "Rhs string's length: "
          //                      << strlen(rhsStmClass)
          //                      << "\n"
          //                      << rw.getRewrittenText(lhs->getSourceRange())
          //                      << "\n"
          //                      << rw.getRewrittenText(rhsCast->getSourceRange())
          //                      << "\n");

          // #AGB if LHS of BinOP has a cast, it must be considered that the 2 variables both are not of cast type
          // hence, for safety measure, add a cast to RHS within VPA

          if ((LCE != nullptr) && (strcmp(LCE->getCastKindName(), "NoOp") == 0)) {
            rw.ReplaceText(rhsCast->getSourceRange().getBegin(), "::vpa::VPA((" + opRetType + ")" + rhsString);
          }else {
            rw.InsertTextAfter(rhs->getSourceRange().getBegin(), "::vpa::VPA(");
          }    

        // #Original    
        // rw.InsertTextAfter(rhs->getSourceRange().getBegin(), "::vpa::VPA(");
        
            
        //castVpaFloat(rw, rhs, opId);
        } else {
          // II level
          DEBUG(::llvm::dbgs() << "II type"
                               << "\n");

          // Apply replacements depending on the hand side types
          //if (!isLhsBinaryOp) {
            rw.InsertTextAfter(bop->getSourceRange().getBegin(), "::vpa::VPA(");
            rw.InsertTextBefore(bop->getOperatorLoc(), ", "+opId+") ");
            //castVpaFloat(rw, lhs, opId);
          //} else {
            
            std::string currentString = rw.getRewrittenText(SourceRange(bop->getSourceRange().getEnd()));
            rw.InsertTextBefore(bop->getSourceRange().getEnd().getLocWithOffset(currentString.size()), ", "+opId+")/*II "+opId+"*/ ");
            rw.InsertTextAfterToken(bop->getOperatorLoc(), "::vpa::VPA(");
            
        }
        //}
  }

    const VarDecl *varDecl =
        node.Nodes.getNodeAs<VarDecl>("varDeclAssign");
    if (varDecl != nullptr){
        const Expr *varDeclExpr = varDecl->getAnyInitializer()->IgnoreCasts();//->IgnoreImpCasts();
        DEBUG(::llvm::dbgs() << "it is an varDeclAssign\n");
    if (varDeclExpr == bop){
        retVar = varDecl->getNameAsString();
        std::string currentStringVarDecl = rw.getRewrittenText(SourceRange(varDeclExpr->getSourceRange().getEnd()));
        rw.InsertTextAfter(varDeclExpr->getSourceRange().getEnd().getLocWithOffset(currentStringVarDecl.size()), ") ");
        rw.InsertTextBefore(varDeclExpr->getSourceRange().getBegin(), "("+ varDecl->getType().getAsString() +")(");
        DEBUG(::llvm::dbgs() << "Var declaration expression: "
                             << rw.getRewrittenText(varDecl->getSourceRange())
                             << "\n");
    }   
    }
    
    // Get return variable name, if exists
    const BinaryOperator *assignOp =
        node.Nodes.getNodeAs<BinaryOperator>("externalAssignOp");
    if (assignOp != nullptr) {
      // Some assign operation has been matched, narrow down to the really
      // interesting
      // The bop MUST be its RHS
        DEBUG(::llvm::dbgs() << "it is an externalAssignOp\n");
      if (assignOp->getRHS()->IgnoreCasts()->IgnoreParenImpCasts() == bop) {
          const Expr *lhsAssign = assignOp->getLHS()->IgnoreCasts();
          const Expr *rhsAssign = assignOp->getRHS()->IgnoreCasts();
          
          ::std::string lhsAssignString = rw.getRewrittenText(lhsAssign->getSourceRange());
          ::std::string rhsAssignString = rw.getRewrittenText(rhsAssign->getSourceRange());
          
            SourceRange rangeRH = rhsAssign->getSourceRange();
            rw.InsertTextAfterToken(assignOp->getOperatorLoc(), "(" + opRetType + ")(");
            rw.InsertTextAfterToken(assignOp->getLocEnd(), ")");
          
          //::std::string bopAssign = lhsAssignString + " = (" + opRetType + ") (" + rhsAssignString +")";
         // rw.ReplaceText(assignOp->getSourceRange(), bopAssign);
        ///////////////////////////////////////////////////////////////////////////////
        /// DEBUG
        DEBUG(::llvm::dbgs() << "External assignment operation: "
                             << rw.getRewrittenText(assignOp->getSourceRange())
                             << "\n");
        ///////////////////////////////////////////////////////////////////////////////
        // Check if it is a DeclRef expression
        if (::llvm::isa<DeclRefExpr>(assignOp->getLHS())) {
          retVar = ((const DeclRefExpr *)(assignOp->getLHS()))
                       ->getNameInfo()
                       .getName()
                       .getAsString();
        }
      }
    }
  //}

  // Store mutations info:
  VPAFloatOperationMutator::MutationInfo mutationInfo;
  // * Operation Identifier
  mutationInfo.opId = opId;
  // * Line location
  FullSourceLoc loc(bop->getSourceRange().getBegin(), *(node.SourceManager));
  mutationInfo.line = loc.getSpellingLineNumber();
  // * Return type
  std::transform(opRetType.begin(), opRetType.end(), opRetType.begin(),::toupper);
  mutationInfo.opRetTy = opRetType;
  // * Operation type
  mutationInfo.opTy = bop->getOpcode();
  // * Information about operands:
  // ** LHS
  ::std::string oriLHS = oriRw.getRewrittenText(internalLhs->getSourceRange());
  ::std::replace(oriLHS.begin(), oriLHS.end(), '\n', ' ');
  oriLHS.erase(remove_if(oriLHS.begin(), oriLHS.end(), ::isspace),
               oriLHS.end());
  mutationInfo.op1 = oriLHS;
  mutationInfo.op1OpTy = NoOp;
  if (isLhsBinaryOp) {
    mutationInfo.op1OpTy = ((const BinaryOperator *)internalLhs)->getOpcode();
  }
  // ** RHS
  ::std::string oriRHS = oriRw.getRewrittenText(internalRhs->getSourceRange());
  ::std::replace(oriRHS.begin(), oriRHS.end(), '\n', ' ');
  oriRHS.erase(remove_if(oriRHS.begin(), oriRHS.end(), ::isspace),
               oriRHS.end());
  mutationInfo.op2 = oriRHS;
  mutationInfo.op2OpTy = NoOp;
  if (isRhsBinaryOp) {
    mutationInfo.op2OpTy = ((const BinaryOperator *)internalRhs)->getOpcode();
  }
  // ** Return variable, if exists
  mutationInfo.retOp = retVar;

  this->mutationsInfo.push_back(mutationInfo);

  DEBUG(::llvm::dbgs() << "Last value: " << rw.getRewrittenText(bop->getSourceRange()) << "\n");
  return rw;
}

void chimera::vpamutator::VPAFloatOperationMutator::onCreatedMutant(
    const ::std::string &mDir) {
  // Create a specific report inside the mutant directory
  ::std::error_code error;
  ::llvm::raw_fd_ostream report(mDir + "vpa_float_report.csv", error,
                                ::llvm::sys::fs::OpenFlags::F_Text);
  // Resolve operand/operation information, substituting the binary operator
  // with the code of the I type operation
  // This operation, due to the unknown order of processing, has to be performed
  // here
  // Make a copy to always read the old operand string
  // FIXME: Check also operation type -> store binaryOperator pointer to
  // compare?
  ::std::vector<MutationInfo> cMutationsInfo = this->mutationsInfo;
      for (auto &mI : cMutationsInfo) {
                      DEBUG(::llvm::dbgs() << "Id: " << mI.opId <<"; Op1: " << mI.op1 << ";\t\t Op2: " << mI.op2 << "\n");
      }
 int i = 0, j;
  for (auto &mI : cMutationsInfo) {
    if (mI.op1OpTy != NoOp) {
      // Operand 1 is a binary operation
      // Search in all info
      j = 0;
      for (const auto &mII : this->mutationsInfo) {
        // Check that isn't the same mutationInfo
        if (mII.opId != mI.opId) {
          // Search both operand inside mI.op1, if they are both found AND
          // the operation between them is mII.opTy there is a match.
          // Search from the end of op1 and begin of op2 the OpcodeStr of mII
          auto op1inOp = ::std::search(mI.op1.begin(), mI.op1.end(),
                                       mII.op1.begin(), mII.op1.end());
          auto op2inOp = ::std::search(mI.op1.begin(), mI.op1.end(),
                                       mII.op2.begin(), mII.op2.end());
          if (op1inOp != mI.op1.end() && op2inOp != mI.op1.end() &&
              ::std::find(op1inOp + mII.op1.size() - 1, op2inOp,
                          BinaryOperator::getOpcodeStr(mII.opTy).data()[0]) !=
                  mI.op1.end() && j > i) {
            DEBUG(::llvm::dbgs() << mI.opId << " <--> " << mII.opId <<";\tOperand/operation: " << mI.op1
                                 << " IS Operation: " << mII.opId << "\n");
            mI.op1 = mII.opId; // found the new label
            break;
          }
        }
       ++j;
      }
    }
    if (mI.op2OpTy != NoOp) {
      // Operand 2 is a binary operation
      // Search in all info
        j = 0;
      for (const auto &mII : this->mutationsInfo) {
        // Check that isn't the same mutationInfo
        if (mII.opId != mI.opId) {
          // Search both operand inside mI.op1, if they are both found AND
          // the operation between them is mII.opTy there is a match.
          // Search from the end of op1 and begin of op2 the OpcodeStr of mII
          auto op1inOp = ::std::search(mI.op2.begin(), mI.op2.end(),
                                       mII.op1.begin(), mII.op1.end());
          auto op2inOp = ::std::search(mI.op2.begin(), mI.op2.end(),
                                       mII.op2.begin(), mII.op2.end());
          if (op1inOp != mI.op2.end() && op2inOp != mI.op2.end() &&
              ::std::find(op1inOp + mII.op1.size() - 1, op2inOp,
                          BinaryOperator::getOpcodeStr(mII.opTy).data()[0]) !=
                  mI.op1.end() && j > i) {
            DEBUG(::llvm::dbgs() << mI.opId << " <--> " << mII.opId <<";\tOperand/operation: " << mI.op2
                                 << " IS Operation: " << mII.opId << "\n");
            mI.op2 = mII.opId; // found the new label
            break;
          }
        }
        ++j;
      }
    }
   ++i;
  }

  // Now resolve the retVar, that is where an operation produce a retVar that is
  // used as input in a
  // following operation, the two are dependant. So the input var of the latter
  // operation can be substituted
  // with the operationId of the first.
  // The entries are ordered as location of occurrence, starting from the end it
  // is necessary to see if
  // an operand that is not a binary operation occurrs as retVar of previous
  // operation
  for (auto rIt = cMutationsInfo.rbegin(), rEnd = cMutationsInfo.rend();
       rIt != rEnd; ++rIt) {
    // Operand 1
    if (rIt->op1OpTy == NoOp) {
      auto &localOp = rIt->op1;
      // loop on the remaining operation
      for (auto rIt2 = rIt + 1; rIt2 != rEnd; rIt2++) {
        // Check if operand 1 is a retVar for anyone of them
        if (rIt2->retOp != "NULL" && localOp == rIt2->retOp) {
          DEBUG(::llvm::dbgs() << "Operand: " << localOp
                               << " IS Operation: " << rIt2->opId << "\n");
          localOp = rIt2->opId; // new label
          break;
        }
      }
    }
    if (rIt->op2OpTy == NoOp) {
      // Operand 2
      auto &localOp = rIt->op2;
      // loop on the remaining operation
      for (auto rIt2 = rIt + 1; rIt2 != rEnd; rIt2++) {
        // Check if operand 1 is a retVar for anyone of them
        if (rIt2->retOp != "NULL" && localOp == rIt2->retOp) {
          DEBUG(::llvm::dbgs() << "Operand: " << localOp
                               << " IS Operation: " << rIt2->opId << "\n");
          localOp = rIt2->opId; // new label
          break;
        }
      }
    }
  }

  // for (const auto& mutationInfo : this->mutationsInfo) {
  for (const auto &mutationInfo : cMutationsInfo) {
    report << mutationInfo.opId << "," << mutationInfo.line << ","
           << mutationInfo.opRetTy << "," << mapOpCode(mutationInfo.opTy) << ","
           << "\"" << mutationInfo.op1 << "\","
           << "\"" << mutationInfo.op2 << "\","
           << "\"" << mutationInfo.retOp << "\"\n";
  }
  report.close();
}
