//===- ObjcObfuscator.cpp ---------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Example clang plugin which simply prints the names of all the top-level decls
// in the input file.
//
//===----------------------------------------------------------------------===//
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Sema/Sema.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/AST/Type.h"
#include "clang/Lex/Lexer.h"
#include "clang/Lex/Lexer.h"
#include "llvm/Support/RandomNumberGenerator.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <random>
#include <time.h>
#include "./Strings.h"
//Setup Random Name Array
std::string arrayString[] = ObfuscateStringList;
//End


using namespace clang;
using namespace std;
class ObjcObfuscatorVisitor
  : public RecursiveASTVisitor<ObjcObfuscatorVisitor> {
public:
  ObjcObfuscatorVisitor(ASTContext *Context,SourceManager* Manager)
    : Context(Context),Manager(Manager),_rewriter(*Manager,Context->getLangOpts ()){

    }
  void HandleTranslationUnit(clang::ASTContext &context)
    {
        TraverseDecl(context.getTranslationUnitDecl());

    }
  bool VisitObjCInterfaceDecl(ObjCInterfaceDecl *declaration)
    {
      if(Manager->isInMainFile(declaration->getSourceRange ().getBegin () )){
            llvm::errs()<<"Objc Interface:"<<declaration->getObjCRuntimeNameAsString ()<<"\n";
            std::cout<<"#define "<<declaration->getNameAsString ()<<" "<<RandomString()<<"\n";
        }
            return true;
    }
  bool VisitObjCMethodDecl(ObjCMethodDecl *declaration)
    {
      if(Manager->isInMainFile(declaration->getSourceRange ().getBegin () )){
        //Need To Break Down The Selector Into Parts
          llvm::errs()<<"ObjcMethod:"<<declaration->getNameAsString ()<<"\n";
          SplitSelector(declaration->getNameAsString ());
        }
            return true;
    }
  bool VisitObjCIvarDecl(ObjCIvarDecl *declaration)
    {
      if(Manager->isInMainFile(declaration->getSourceRange ().getBegin () )){
              llvm::errs()<<"ObjcIvar:"<<declaration->getNameAsString ()<<"\n";

              std::cout<<"#define "<<declaration->getNameAsString ()<<" "<<RandomString()<<"\n";
        }
            return true;
    }
  bool VisitObjCPropertyDecl(ObjCPropertyDecl *declaration)
    {
      if(Manager->isInMainFile(declaration->getSourceRange ().getBegin () )){
              llvm::errs()<<"ObjCPropertyDecl:"<<declaration->getNameAsString ()<<"\n";
              std::cout<<"#define "<<declaration->getNameAsString ()<<" "<<RandomString()<<"\n";
        }
            return true;
    }
  bool VisitObjCCategoryDecl(ObjCCategoryDecl *declaration)
    {
      if(Manager->isInMainFile(declaration->getSourceRange ().getBegin () )){
            llvm::errs()<<"ObjCCategoryDecl:"<<declaration->getNameAsString ()<<"\n";
            std::cout<<"#define "<<declaration->getNameAsString ()<<" "<<RandomString()<<"\n";
        }
            return true;
    }
  static std::string RandomString(){
    std::string ReturnString;
    for(int i=0;i<72;i++){
        std::string string=arrayString[rand() % ObfuscateListSize];
        ReturnString+=string;
    }
    return ReturnString;

  }
  static void SplitSelector(std::string str)
{
    for (int i=0; i<str.length(); i++)
    {
    if (str[i] == ':')
        str[i] = ' ';
    }

    vector<std::string> array;
    stringstream ss(str);
    std::string temp;
    while (ss >> temp)
      array.push_back(temp); 
    for(int i=0; i<array.size(); i++){
      std::cout<<"#define "<<array[i]<<" "<<RandomString()<<"\n";

    }
}
  private:
  ASTContext *Context;
  SourceManager* Manager;
  typedef clang::RecursiveASTVisitor<ObjcObfuscatorVisitor> Base;
  clang::Rewriter _rewriter;

  };
class ObjcObfuscatorConsumer : public clang::ASTConsumer {
public:
    explicit ObjcObfuscatorConsumer(ASTContext *Context,SourceManager* Manager): Visitor(Context,Manager) {}
    virtual bool HandleTopLevelDecl(DeclGroupRef DG) {
        // a DeclGroupRef may have multiple Decls, so we iterate through each one
        for (DeclGroupRef::iterator i = DG.begin(), e = DG.end(); i != e; i++) {
            Decl *D = *i;
            Visitor.TraverseDecl(D); // recursively visit each AST node in Decl "D"
        }
        return true;
            }
private:
  SourceManager *Manager;
  ObjcObfuscatorVisitor Visitor;
};


class ObjcObfuscatorAction : public PluginASTAction {
protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 llvm::StringRef) override {
    llvm::errs()<<"Loading ObjcClassGuard\n";
    return llvm::make_unique<ObjcObfuscatorConsumer>(&CI.getASTContext(),&CI.getSourceManager ());
  }

  bool ParseArgs(const CompilerInstance &CI,
                 const std::vector<std::string> &args) override {

    return true;
  }
  void PrintHelp(llvm::raw_ostream& ros) {
    ros << "Add -ObjcClassGuard Obfuscate Objc Class & Method Names";
  }

};


static FrontendPluginRegistry::Add<ObjcObfuscatorAction> X("-ObjcClassGuard", "Obfuscate Objc Class & Method Names");






