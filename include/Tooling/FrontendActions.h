/******************************************************************************
 * @file   FrontendActions.h
 * @author Federico Iannucci
 * @date   01 dic 2015
 * @brief  FrontendAction child classed and utility functions
 * @details This file provides some FrontendAction implementation, each with an
 *          help function to call it easily
 ******************************************************************************/
#ifndef INCLUDE_FRONTENDACTIONS_H_
#define INCLUDE_FRONTENDACTIONS_H_

#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/raw_ostream.h"

namespace chimera {

///////////////////////////////////////////////////////////////////////////////
/// @brief Check the syntax of the source file
/// @param Output stream
/// @param The compile command for the source file
/// @param sourceFilePath The path to the source file
int checkSyntaxAction(const ::clang::tooling::CompileCommand&,
                      const std::string& sourceFilePath);

/// @brief Put on an raw_ostream the function definitions in the sourceFilePath
/// @param Output stream
/// @param The compile command for the source file
/// @param sourceFilePath The path to the source file
int functionDefAction(llvm::raw_ostream&,
                      const ::clang::tooling::CompileCommand&,
                      const std::string& sourceFilePath);
///////////////////////////////////////////////////////////////////////////////
/// @brief Preprocess the input file and put the result on a raw_ostream
class PreprocessIncludeAction : public clang::InitOnlyAction {
 public:
  /// @brief Ctor
  /// @param o Output stream
  PreprocessIncludeAction(llvm::raw_ostream& o = llvm::outs())
      : raw_out(o) {
  }
  /// @brief Put on raw_out stream the preprocessed file.
  void EndSourceFileAction() override;
 private:
  llvm::raw_ostream& raw_out;
};
/// @brief Apply the PreprocessIncludeAction to sourceFilePath
/// @param Output stream
/// @param The compile command for the source file
/// @param sourceFilePath The path to the source file
int preprocessIncludeAction(llvm::raw_ostream&,
                            const ::clang::tooling::CompileCommand&,
                            const std::string& sourceFilePath);
///////////////////////////////////////////////////////////////////////////////
/// @brief Expand the macros in the input file and put the result on a raw_ostream
class ExpandMacroAction : public clang::PreprocessOnlyAction {
 public:
  /// @brief Ctor
  /// @param o Output stream
  ExpandMacroAction(llvm::raw_ostream& o = llvm::outs())
      : raw_out(o) {
  }
  /// @brief Put on raw_out stream the macro-expanded file.
  void EndSourceFileAction() override;
 private:
  llvm::raw_ostream& raw_out;
};
/// @brief Apply the MacroExpansionAction to sourceFilePath
/// @param Output stream
/// @param The compile command for the source file
/// @param sourceFilePath The path to the source file
int expandMacrosAction(llvm::raw_ostream&,
                       const ::clang::tooling::CompileCommand&,
                       const std::string& sourceFilePath);
///////////////////////////////////////////////////////////////////////////////
/// @brief Create an HTML version of the source file and put it on a raw_ostream
class HTMLPrinterAction : public clang::ASTFrontendAction {
 public:
  /// @brief Ctor
  /// @param o Output stream
  HTMLPrinterAction(llvm::raw_ostream& o = llvm::outs()): raw_out(o) {
  }

  ::std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance&, llvm::StringRef);
 private:
  llvm::raw_ostream& raw_out;
};
/// @brief Apply the HTMLPrinterAction to sourceFilePath
/// @param Output stream
/// @param The compile command for the source file
/// @param sourceFilePath The path to the source file
int htmlPrintAction(llvm::raw_ostream&,
                    const ::clang::tooling::CompileCommand&,
                    const std::string& sourceFilePath);
///////////////////////////////////////////////////////////////////////////////
/// @brief Reformat the code and put it on a raw_ostream
class ReformatAction : public clang::PreprocessOnlyAction {
 public:
  /// @brief Ctor
  /// @param o Output stream
  ReformatAction(llvm::raw_ostream& o = llvm::outs())
      : raw_out(o) {
  }
  /// @brief Put on raw_out stream the macro-expanded file.
  void EndSourceFileAction() override;
 private:
  llvm::raw_ostream& raw_out;
};
/// @brief Apply the MacroExpansionAction to sourceFilePath
/// @param Output stream
/// @param The compile command for the source file
/// @param sourceFilePath The path to the source file
int reformatAction(llvm::raw_ostream&,
                       const ::clang::tooling::CompileCommand&,
                       const std::string& sourceFilePath);
/// @brief Reformat the source code \p code streaming the output into \p o
/// @param Output stream
/// @param Source code
void reformatSourceCode(llvm::raw_ostream& o, ::llvm::StringRef code);

}  // end chimera namespace
#endif /* INCLUDE_FRONTENDACTIONS_H_ */
