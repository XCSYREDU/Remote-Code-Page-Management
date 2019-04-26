///////////////////////////////////////////////////////////////////////////
// Display.cpp   : defines webpage display using browser functions       //
// ver 1.1                                                               //
//																		 //
//  Platform:      Dell Alienware 17R5, Windows 10                       //
//  Application:   Project for CSE687 - OOD Projects                     //
//  Author:        Xiao Chen                                             //
//  Source: Display.h from TA Ammar Salman                                //
///////////////////////////////////////////////////////////////////////////

#include "Display.h"
#include "../Process/Process.h"
#include "../Logger/Logger.h"
#include "../FileSystem/FileSystem.h"
#include "../Parser/Parser.h"
#include "../Parser/ConfigureParser.h"

using namespace FileSystem;
using namespace CodeAnalysis;
using namespace Logging;

// -----< default ctor >--------------------------------------------------
Display::Display() { }

// -----< display single file >-------------------------------------------
void Display::display(const std::string & file)
{
  LoggerDebug::write("\n  Displaying file: " + FileSystem::Path::getName(file) + " in browser");
  LoggerDemo::write("\n  Displaying file: " + FileSystem::Path::getName(file) + " in browser");

  CBP callback = []() {
    LoggerDebug::write("\n  --- child browser exited with this message ---");
    LoggerDemo::write("\n  --- child browser exited with this message ---");
  };

  Process p;
  p.application("C:\\Windows\\System32\\cmd.exe");
  std::string cmd = "/C " + FileSystem::Path::getFullFileSpec(file);

  p.commandLine(cmd);
  p.create();
  p.setCallBackProcessing(callback);
  p.registerCallback();
  WaitForSingleObject(p.getProcessHandle(), INFINITE);
}

// -----< find sigleline comment and process it >--------------------------
bool Display::findsinglecomment(std::string & line, std::ofstream& out_) {
	size_t pos = line.npos;
	if ((pos = line.find("//")) != line.npos) {
		if (pos_mark_mult != 0) {
			return TRUE;
		}
		if (pos_mark == 0) {
			line.insert(pos, "<div class=\"toggleSingCommentDisplay\">");
			commtbuffer_.push_back(line);
			pos_mark++;
			return FALSE;
		}
		else {
			if (*(line.begin()) != '/' /*&& commtbuffer_[0].find("//") != pos*/) {
				commtbuffer_[pos_mark - 1].append("</div>");
				for (size_t i = 0; i < commtbuffer_.size(); i++) {
					out_ << commtbuffer_[i] << std::endl;
				}
				line.insert(pos, "<div class=\"toggleSingCommentDisplay\">");
				line.append("</div>");
				commtbuffer_.clear();
				pos_mark = 0;
				return TRUE;
			}
			pos_mark++;
			commtbuffer_.push_back(line);
			return FALSE;
		}
	}
	if ((pos = line.find("//")) == line.npos && pos_mark != 0) {
		commtbuffer_[pos_mark - 1].append("</div>");
		for (size_t i = 0; i < commtbuffer_.size(); i++) {
			out_ << commtbuffer_[i] << std::endl;
		}
		commtbuffer_.clear();
		pos_mark = 0;
		return TRUE;
	}
	else {
		return TRUE;
	}
}

// -----< return vector to be used by other package >--------------------------
std::vector<int> Display::r_cs() {
	return class_start;
}

// -----< return vector to be used by other package >--------------------------
std::vector<int> Display::r_ce() {
	return class_end;
}

// -----< return vector to be used by other package >--------------------------
std::vector<int> Display::r_ms() {
	return method_start;
}

// -----< return vector to be used by other package >--------------------------
std::vector<int> Display::r_me() {
	return method_end;
}

// -----< find multi line comment and process it >--------------------------
bool Display::findmultComment(std::string & line, std::ofstream& out_) {
	size_t pos = line.npos;
	size_t pos_2 = line.npos;
	if (pos_mark_mult == 0) {
		if ((pos = line.find("/*")) != line.npos && (pos_2 = line.find("*/")) != line.npos) {
			line.insert(pos, "<div class=\"toggleMultCommentDisplay\">");
			line.append("</div>");
			return TRUE;
		}
		else if ((pos = line.find("/*")) != line.npos) {
			line.insert(pos, "<div class=\"toggleMultCommentDisplay\">");
			mulcommbuffer_.push_back(line);
			pos_mark_mult++;
			return FALSE;
		}
		else {
			return TRUE;
		}
	}
	else {
		if ((pos = line.find("*/")) != line.npos) {
			for (size_t i = 0; i < mulcommbuffer_.size(); i++) {
				out_ << mulcommbuffer_[i] << std::endl;
			}
			mulcommbuffer_.clear();
			line = line + "</div>";
			pos_mark_mult = 0;
			return TRUE;
		}
		else {
			pos_mark_mult++;
			mulcommbuffer_.push_back(line);
			return FALSE;
		}
	}
}

// -----< find class and function line  >--------------------------
void Display::findaf(std::string path) {
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
		ASTNode* pGlobalScope = pRepo->getGlobalScope();
		complexityEval(pGlobalScope);
		//pParser->findClassAndFunction(pGlobalScope);
		pParser->findClassAndFunction(pGlobalScope);
		class_start = pParser->pcs();
		class_end = pParser->pce();
		method_start = pParser->pms();
		method_end = pParser->pme();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
		std::cout << "\n  exception caught at line " << __LINE__ << " ";
		std::cout << "\n  in package \"" << name << "\"";
	}
	std::cout << "\n";
}

// -----< process class and function .html line >--------------------------
void Display::addclassandFunc(std::vector<int> cs, std::vector<int> ce, std::vector<int> ms, std::vector<int> me, int line_num, std::string& line) {
	auto iter1 = find(cs.begin(), cs.end(), line_num);
	if (iter1 != cs.end()) {
		size_t pos1 = line.npos;
		if ((pos1 = line.find("{")) != line.npos) {
			line.insert(pos1, "<div class=\"toggleClassDisplay\">");
		}
	}
	auto iter2 = find(ce.begin(), ce.end(), line_num - 1);
	if (iter2 != ce.end()) {
		line = "</div>" + line;
	}
	auto iter3 = find(ms.begin(), ms.end(), line_num);
	if (iter3 != ms.end()) {
		size_t pos = line.npos;
		if ((pos = line.find("{")) != line.npos) {
			line.insert(pos, "<div class=\"toggleMethodDisplay\">");
		}
	}
	auto iter4 = find(me.begin(), me.end(), line_num);
	if (iter4 != me.end()) {
			line = line + "</div>";
	}
}

// -----< display multiple files  >---------------------------------------
void Display::display(const std::vector<std::string>& files)
{
  for (auto file : files) 
    display(file);
}

#ifdef TEST_DISPLAY

#include <iostream>

int main() {
  LoggerDebug::attach(&std::cout);
  LoggerDebug::start();

  std::vector<std::string> files;
  files.push_back("..\\ConvertedWebpages\\Converter.h.html");
  files.push_back("..\\ConvertedWebpages\\Converter.cpp.html");
  std::string path = "C:/Users/xiaoc/Desktop/687_ pj2/CppParser/CppProperties/Properties.h";
  Display d;
  d.display(files);
  d.findaf(path);
  LoggerDebug::stop();
}

#endif