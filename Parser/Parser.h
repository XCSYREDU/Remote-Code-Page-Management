#ifndef PARSER_H
#define PARSER_H
/////////////////////////////////////////////////////////////////////
//  Parser.h - Analyzes C++ and C# language constructs             //
//  ver 1.1                                                        //
//  Language:      Visual C++, Visual Studio 2017                  //
//  Platform:      Dell Alienware 17R5, Windows 10                 //
//  Application:   Porject for CSE687 - OOD Projects               //
//  Author:        Xiao Chen                                       //
//  Source: Parser.h from Professor Jim Fawcett                    //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================
  This module defines a Parser class.  Its instances collect 
  semi-expressions from one or more files for analysis.  
  Analysis consists of applying a set of rules to the semi-expression, 
  and for each rule that matches, invoking a set of one or more actions.

  Build Process:
  ==============
  Required files
    - Parser.h, Parser.cpp, 
    - ITokenCollection.h, 
    - Semi.h, Semi.cpp,
    - toker.h, toker.cpp,
    - ActionsAndRules.h, ActionsAndRules.cpp,
    - GrammarHelpers.h, GrammarHelpers.cpp,
    - AbstrSynTree.h, AbstrSynTree.cpp,
    - ScopeStack.h, ScopeStack.cpp
    - ConfigureParser.h, ConfigureParser.cpp,
    - FileSystem.h, FileSystem.cpp,
    - Logger.h, Logger.cpp,
    - Utilities.h, Utilities.cpp

  Maintenance History:
  ===================
  ver 1.1 : 3 Mar 2019
  - based on parser.h from professor Jim Fawcett
  - add a default constructor for Class Parser
  - add a function to judge whether it meet Class or method
  - add four functions to transfer private vector to other package
  - add four private vector<string>

  ver 1.0 : 11 Feb 2019
  - first release


*/

#include <string>
#include <iostream>
#include <vector>
#include "../SemiExpression/ITokenCollection.h"
#include "../AbstractSyntaxTree/AbstrSynTree.h"


namespace CodeAnalysis
{
  ///////////////////////////////////////////////////////////////
  // abstract base class for parsing actions
  //   - when a rule succeeds, it invokes any registered action

  class Parser;

  class IBuilder
  {
  public:
    virtual ~IBuilder() {}
    virtual Parser* Build() = 0;
  };

  ///////////////////////////////////////////////////////////////
  // abstract base class for parsing actions
  //   - when a rule succeeds, it invokes any registered action

  class IAction
  {
  public:
    virtual ~IAction() {}
    virtual void doAction(const Lexer::ITokenCollection* pTc) = 0;
  };

  ///////////////////////////////////////////////////////////////
  // abstract base class for parser language construct detections
  //   - rules are registered with the parser for use

  class IRule
  {
  public:
    static const bool Continue = true;
    static const bool Stop = false;
    virtual ~IRule() {}
    void addAction(IAction* pAction);
    void doActions(const Lexer::ITokenCollection* pTc);
    virtual bool doTest(const Lexer::ITokenCollection* pTc) = 0;
  protected:
    std::vector<IAction*> actions;
  };

  class Parser
  {
  public:
    Parser(Lexer::ITokenCollection* pTokCollection);
	Parser() ;
    ~Parser();
    void addRule(IRule* pRule);
    bool parse();
    bool next();

	void findClassAndFunction(ASTNode* pGlobalScope);
	std::vector<int> pcs();
	std::vector<int> pce();
	std::vector<int> pms();
	std::vector<int> pme();
  private:
    Lexer::ITokenCollection* pTokColl;
    std::vector<IRule*> rules;
	
	std::vector<int> class_start;
	std::vector<int> class_end;
	std::vector<int> method_start;
	std::vector<int> method_end;
  };

  inline Parser::Parser(Lexer::ITokenCollection* pTokCollection) : pTokColl(pTokCollection) {}
  inline Parser::Parser(){}
  inline Parser::~Parser() {}
}
#endif
