//===- FrontendActions.cpp --------------------------------------*- C++ -*-===//
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
/// \file FrontendActions.cpp
/// \author Federico Iannucci
/// \brief  This file implements FrontendActions classes
//===----------------------------------------------------------------------===//

#include "Tooling/CompilationDatabaseUtils.h"
#include "Tooling/FrontendActions.h"

#include "clang/AST/Decl.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/Utils.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Frontend/PreprocessorOutputOptions.h"
#include "clang/Format/Format.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "clang/Rewrite/Frontend/ASTConsumers.h"

using namespace clang::ast_matchers;
using namespace clang::tooling;

///////////////////////////////////////////////////////////////////////////////
/// @brief MatchCallback child : The callback called to show functions definition
class FunctionDefCallback :
    public clang::ast_matchers::MatchFinder::MatchCallback {
 public:
  FunctionDefCallback(llvm::raw_ostream& out)
      : raw_out(out) {
  }
  /**
   * @brief Run implementation.
   */
  virtual void run(
      const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    // The Result should contains a FunctioNDecl binded with the "functionDecl" refId
    const clang::FunctionDecl* functionDecl = Result.Nodes
        .getNodeAs<clang::FunctionDecl>("functionDecl");
    // Print to the ostream
    raw_out << functionDecl->getNameAsString() << " at "
            << functionDecl->getLocStart().printToString(*Result.SourceManager)
            << '\n';
  }
 private:
  llvm::raw_ostream& raw_out;
};

int chimera::functionDefAction(llvm::raw_ostream& out,
                               const CompileCommand& command,
                               const ::std::string& sourceFilePath) {
  MatchFinder functionDeclFinder;
  FunctionDefCallback* callback = new FunctionDefCallback(out);
  DeclarationMatcher matcher = functionDecl(isDefinition()).bind(
      "functionDecl");
  functionDeclFinder.addMatcher(matcher, callback);
  return (ClangTool(::chimera::cd_utils::FlexibleCompilationDatabase(command),
                    sourceFilePath)).run(
      newFrontendActionFactory(&functionDeclFinder).get());
}
///////////////////////////////////////////////////////////////////////////////

int chimera::checkSyntaxAction(const ::clang::tooling::CompileCommand& c,
                               const ::std::string& sourceFilePath) {
  // Run the ClangTool with proper FrontendClass
  return (ClangTool(::chimera::cd_utils::FlexibleCompilationDatabase(c),
                    sourceFilePath)).run(
      newFrontendActionFactory<clang::SyntaxOnlyAction>().get());
}

///////////////////////////////////////////////////////////////////////////////

void chimera::PreprocessIncludeAction::EndSourceFileAction() {
  // Create the output options for the preprocessor
  clang::PreprocessorOutputOptions poo = clang::PreprocessorOutputOptions();
  poo.ShowCPP = 1;
  clang::DoPrintPreprocessedInput(this->getCompilerInstance().getPreprocessor(),
                                  &this->raw_out, poo);
}

int chimera::preprocessIncludeAction(llvm::raw_ostream& out,
                                     const CompileCommand& c,
                                     const ::std::string& sourceFilePath) {
  // Create temp FrontendActionFactory class
  class SimpleFrontendActionFactory : public FrontendActionFactory {
   public:
    SimpleFrontendActionFactory(llvm::raw_ostream& o)
        : raw_out(o) {
    }
    clang::FrontendAction *create() override {
      return new PreprocessIncludeAction(this->raw_out);
    }
   private:
    llvm::raw_ostream& raw_out;
  };

// Run the ClangTool with proper FrontendClass
  return (ClangTool(::chimera::cd_utils::FlexibleCompilationDatabase(c),
                    sourceFilePath)).run(new SimpleFrontendActionFactory(out));
}

///////////////////////////////////////////////////////////////////////////////

void chimera::ExpandMacroAction::EndSourceFileAction() {
  ::std::string macro_expanded_code;
  ::llvm::raw_string_ostream raw_string_out(macro_expanded_code);
  clang::RewriteMacrosInInput(this->getCompilerInstance().getPreprocessor(),
                              &raw_string_out);
  reformatSourceCode(this->raw_out, macro_expanded_code);
}

int chimera::expandMacrosAction(llvm::raw_ostream& out,
                                const ::clang::tooling::CompileCommand& c,
                                const ::std::string& sourceFilePath) {
// Create temp FrontendActionFactory class
  class SimpleFrontendActionFactory : public FrontendActionFactory {
   public:
    SimpleFrontendActionFactory(llvm::raw_ostream& o)
        : raw_out(o) {
    }
    clang::FrontendAction *create() override {
      return new ExpandMacroAction(this->raw_out);
    }
   private:
    llvm::raw_ostream& raw_out;
  };

// Run the ClangTool with proper FrontendClass
  return (ClangTool(::chimera::cd_utils::FlexibleCompilationDatabase(c),
                    sourceFilePath)).run(new SimpleFrontendActionFactory(out));
}

///////////////////////////////////////////////////////////////////////////////

::std::unique_ptr<clang::ASTConsumer> chimera::HTMLPrinterAction::CreateASTConsumer(
    ::clang::CompilerInstance& CI, ::llvm::StringRef sourcePath) {
  return ::clang::CreateHTMLPrinter(std::move(this->raw_out), CI.getPreprocessor());
}

int chimera::htmlPrintAction(::std::unique_ptr<llvm::raw_ostream> out,
                             const ::clang::tooling::CompileCommand& c,
                             const ::std::string& sourceFilePath) {
// Create temp FrontendActionFactory class
  class SimpleFrontendActionFactory : public FrontendActionFactory {
   public:
    SimpleFrontendActionFactory(::std::unique_ptr<llvm::raw_ostream> o): raw_out(std::move(o)){//qua ho fatto un cambio dicui non sono certo
    }
    clang::FrontendAction *create() override {
      return new HTMLPrinterAction(std::move(this->raw_out));
    }
   private:
    ::std::unique_ptr<llvm::raw_ostream> raw_out;
  };

// Run the ClangTool with proper FrontendClass
  return (ClangTool(::chimera::cd_utils::FlexibleCompilationDatabase(c),
                    sourceFilePath)).run(new SimpleFrontendActionFactory(std::move(out)));
}

///////////////////////////////////////////////////////////////////////////////

void chimera::reformatSourceCode(llvm::raw_ostream& out,
                                 ::llvm::StringRef code) {
// Create a vector of Range
  ::std::vector<::clang::tooling::Range> rangeVector = { Range(0, code.size()) };

// Apply reformat with LLVM style
  ::clang::tooling::Replacements rs = ::clang::format::reformat(
      ::clang::format::getLLVMStyle(), code, rangeVector);
  out << ::clang::tooling::applyAllReplacements(code, rs);
}

void chimera::ReformatAction::EndSourceFileAction() {
// Get the source manager
  ::clang::SourceManager& sourceManager = this->getCompilerInstance()
      .getSourceManager();
  reformatSourceCode(
      this->raw_out,
      sourceManager.getBufferData(sourceManager.getMainFileID()));
}

int chimera::reformatAction(llvm::raw_ostream& out,
                            const ::clang::tooling::CompileCommand& c,
                            const std::string& sourceFilePath) {
// Create temp FrontendActionFactory class
  class SimpleFrontendActionFactory : public FrontendActionFactory {
   public:
    SimpleFrontendActionFactory(llvm::raw_ostream& o)
        : raw_out(o) {
    }
    clang::FrontendAction *create() override {
      return new ReformatAction(this->raw_out);
    }
   private:
    llvm::raw_ostream& raw_out;
  };

// Run the ClangTool with proper FrontendClass
  return (ClangTool(::chimera::cd_utils::FlexibleCompilationDatabase(c),
                    sourceFilePath)).run(new SimpleFrontendActionFactory(out));
}
