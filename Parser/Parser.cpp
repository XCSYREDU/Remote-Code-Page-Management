/////////////////////////////////////////////////////////////////////
//  Parser.cpp - Analyzes C++ and C# language constructs           //
//  ver 1.1                                                        //
//  Language:      Visual C++, Visual Studio 2015                  //
//  Platform:      Dell Alienware 17R5, Windows 10                 //
//  Application:   Project for CSE687 - OOD Projects               //
//  Author:        Xiao Chen                                       //
//  Source: Parser.cpp from Professor Jim Fawcett                  //
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Toker.h"
#include "../SemiExpression/Semi.h"
#include "Parser.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"
#include "../Logger/Logger.h"

using namespace CodeAnalysis;
using namespace depend;
using namespace Lexer;
using namespace Utilities;
using namespace CodeAnalysis;
using namespace Logging;
using Demo = Logging::StaticLogger<1>;

namespace CodeAnalysis {

	//----< register parsing rule >--------------------------------

	void Parser::addRule(IRule* pRule)
	{
		rules.push_back(pRule);
	}
	//----< get next ITokenCollection >------------------------------

	bool Parser::next()
	{
		bool succeeded = pTokColl->get();
		if (!succeeded)
		{
			return false;
		}

		pTokColl->trim();

		return true;
	}

	//----< parse the Semi by applying all rules to it >--------

	bool Parser::parse()
	{
		for (size_t i = 0; i < rules.size(); ++i)
		{
			bool doWhat = rules[i]->doTest(pTokColl);
			if (doWhat == IRule::Stop)
				break;
		}
		return true;
	}
	//----< register action with a rule >--------------------------

	void IRule::addAction(IAction *pAction)
	{
		actions.push_back(pAction);
	}
	//----< invoke all actions associated with a rule >------------

	void IRule::doActions(const Lexer::ITokenCollection* pTokColl)
	{
		if (actions.size() > 0)
			for (size_t i = 0; i < actions.size(); ++i)
				actions[i]->doAction(pTokColl);
	}
	// -----< find class and method line count recursivly >--------------------------------------------------
	void Parser::findClassAndFunction(ASTNode* pGlobalScope) {
		if (pGlobalScope->type_ == "class") {
			class_start.push_back(pGlobalScope->startLineCount_);
			class_end.push_back(pGlobalScope->endLineCount_);
		}
		if (pGlobalScope->type_ == "function") {
			method_start.push_back(pGlobalScope->startLineCount_);
			method_end.push_back(pGlobalScope->endLineCount_);
		}

		auto it1 = pGlobalScope->children_.begin();
		while (it1 != pGlobalScope->children_.end()) {
			findClassAndFunction(*it1);
			it1++;
		}
	}
	// -----< return vector to be used by other package >--------------------------
	std::vector<int> Parser::pcs() {
		return class_start;
	}
	// -----< return vector to be used by other package >--------------------------
	std::vector<int> Parser::pce() {
		return class_end;
	}
	// -----< return vector to be used by other package >--------------------------
	std::vector<int> Parser::pms() {
		return method_start;
	}
	// -----< return vector to be used by other package >--------------------------
	std::vector<int> Parser::pme() {
		return method_end;
	}

	//----< test stub >--------------------------------------------
}

#ifdef TEST_PARSER

#include "../FileSystem/FileSystem.h"
#include <queue>
#include <string>
#define Util StringHelper

using namespace CodeAnalysis;

int main(int argc, char* argv[])
{
	Util::Title("Testing Parser Class");
	putline();


	// Analyzing files, named on the command line

	if (argc < 2)
	{
		std::cout
			<< "\n  please enter name of file to process on command line\n\n";
		return 1;
	}

	std::string fileSpec;

	for (int i = 1; i < argc; ++i)  // iterate over files
	{
		Parser* ps(nullptr);
		ps->parserDoing(argv[i]);
	}
	std::cout << "\n";
}

#endif
