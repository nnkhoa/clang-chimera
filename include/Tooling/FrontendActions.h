//===- FrontendActions.h ----------------------------------------*- C++ -*-===//
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
/// \file FrontendActions.h
/// \author Federico Iannucci
/// \brief  This file contains FrontendActions classes
/// \details This file provides some FrontendAction implementation, each with an
///          help function to call it easily
//===----------------------------------------------------------------------===//

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
  // Qui abbiamo l'errore nel compiling conl a nuova llvm la funzione perché nel .cpp 
  // questo viene passato a la funzione clang::createHTMLPrinter che ora non accetta piú raw_ostream ma std::unique_ptr 
  HTMLPrinterAction(::std::unique_ptr<llvm::raw_ostream> o){  raw_out = std::move(o);}

  ::std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance&, llvm::StringRef);
 private:
  ::std::unique_ptr<llvm::raw_ostream> raw_out;
};
/// @brief Apply the HTMLPrinterAction to sourceFilePath
/// @param Output stream
/// @param The compile command for the source file
/// @param sourceFilePath The path to the source file
int htmlPrintAction(::std::unique_ptr<llvm::raw_ostream>,
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
