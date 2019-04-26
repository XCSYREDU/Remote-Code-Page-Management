///////////////////////////////////////////////////////////////////////////
// Dependency.cpp functions body in dependency.h and test tub            //
// ver 1.0                                                               //
//                                                                       // 
//  Platform:      Dell Alienware 17R5, Windows 10                       //
//  Application:   Project for CSE687 - OOD Projects                     //
//  Author:        Xiao Chen                                             //
///////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <regex>
#include "Dependency.h"
#include "../FileSystem/FileSystem.h"
#include "../Parser/Parser.h"
#include "../Parser/ConfigureParser.h"

using namespace Lexer;
using namespace depend;
using namespace Logging;
using namespace FileSystem;
using namespace CodeAnalysis;
// -----< default ctor >--------------------------------------------------

Depen::Depen(){
}
// -----< default distructor >--------------------------------------------------

Depen::~Depen() {

}
// -----< function to add dependency relation to dependency table >--------------

void Depen::adddependency(std::string& fileSpec, std::vector<std::string>& temp) {

	DependencyTable dt;
	std::cout << "Current file: " << fileSpec << std::endl;
	for (size_t i = 0; i < temp.size(); i++) {
		dt.addDependency(fileSpec, temp[i]);
	}
	LoggerDebug::attach(&std::cout);
	LoggerDebug::start();
	dt.display();
	LoggerDebug::stop();
	if (temp.size() == 0) {
		CodeConverter cc;
		cc.convert(fileSpec);
		std::vector<std::string>fileset = cc.r_convertedFiles_();
		for (auto i : fileset) {
			converted_f.push_back(i);
		}
		/*
		for (auto i : fileset) {
			Display dis;
			dis.display(i);
		}*/
	}
	else {
		CodeConverter cc(dt);
		cc.convert();
		std::vector<std::string>fileset = cc.r_convertedFiles_();
		for (auto i : fileset) {
			converted_f.push_back(i);
		}
		/*
		for (auto i : fileset) {
			Display dis;
			dis.display(i);
		}*/
	}

	//LoggerDebug::stop();
}

// -----< add depedency links markup code >--------------------------------
void Depen::addDependencyLinks(std::string file, DependencyTable dt_, std::ofstream& out_)
{
	std::string filename = Path::getName(file);
	if (!dt_.has(file)) { // in case of single file conversion
		std::cout << "\n    No entry found in DependencyTable for [" + filename + "]. Skipping dependency links.." << std::endl;
		return;
	}

	if (dt_[file].size() == 0) { // in case the file has no dependencies
		std::cout << "\n    No dependencies found for [" + filename + "]. Skipping dependency links.." << std::endl;
		return;
	}

	out_ << "    <h3>Dependencies: " << std::endl;
	for (auto dep : dt_[file]) {
		out_ << "      <a href=\"" << Path::getName(dep) << ".html\">" << Path::getName(dep) << "</a>" << std::endl;
	}
	out_ << "    </h3>";
}
// -----< select .cpp and .h from #include semi >--------------

void Depen::adddepen(std::vector<Lexer::ITokenCollection*> a) {
	auto it1 = a.begin();
	while (it1 != a.end()) {
		auto it2 = (*it1)->begin();
		while (it2 != (*it1)->end()) {
			if (regex_search((*it2), std::regex("\\.cpp")) || regex_search((*it2), std::regex("\\.h"))) {
				int temp = (*it2).length() - 2;
				(*it2) = (*it2).substr(1, temp);
				depelem.push_back(*it2);
			}
			it2++;
		}
		it1++;
	}
}
// -----< return file_converted>------------------------------
std::vector<std::string> Depen::r_converted_f() {
	return converted_f;
}

// -----< pass private vector to other package >--------------

std::vector<std::string> Depen::doparser() {
	return depelem;
}
// -----< function to parser the file and find dependency related files >--------------

void Depen::parserDoing(std::string path) {
	std::cout << "done with dependency" << std::endl;
	std::string fileSpec = FileSystem::Path::getFullFileSpec(path);
	ConfigParseForCodeAnal configure;
	Parser* pParser = configure.Build();
	std::string name;
	try
	{
		if (pParser)
		{
			name = FileSystem::Path::getName(path);
			if (!configure.Attach(fileSpec))
			{
				std::cout << "\n  could not open file " << name << std::endl;
				return;
			}
		}
		else
		{
			std::cout << "\n\n  Parser not built\n\n";
			return;
		}
		// save current package name
		Repository* pRepo = Repository::getInstance();
		pRepo->package() = name;
		// parse the package
		while (pParser->next())
		{
			pParser->parse();
		}
		std::vector<std::string> temp; //= pParser->doparser();
		ASTNode* pGlobalScope = pRepo->getGlobalScope();
		complexityEval(pGlobalScope);
		adddepen(pGlobalScope->statements_);
		temp = doparser();
		std::cout << "\n";
		adddependency(fileSpec, temp);
	}

	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
		std::cout << "\n  exception caught at line " << __LINE__ << " ";
		std::cout << "\n  in package \"" << name << "\"";
	}
	std::cout << "\n";
}

#ifdef TEST_DEPEN

#include "../FileSystem/FileSystem.h"
#include <queue>
#include <string>
#define Util StringHelper

using namespace CodeAnalysis;

int main(int argc, char* argv[])
{

	if (argc < 2)
	{
		std::cout
			<< "\n  please enter name of file to process on command line\n\n";
		return 1;
	}

	std::string fileSpec;

	for (int i = 1; i < argc; ++i)  // iterate over files
	{
		Depen de;
		de.parserDoing(argv[i]);
	}
	getchar();
	std::cout << "\n";
}

#endif