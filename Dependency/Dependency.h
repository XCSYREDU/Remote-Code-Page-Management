#pragma once
///////////////////////////////////////////////////////////////////////////
// Dependency.h deinfine functions to get dependency relationships       //
// ver 1.0                                                               //
//                                                                       // 
//  Platform:      Dell Alienware 17R5, Windows 10                       //
//  Application:   Project for CSE687 - OOD Projects                     //
//  Author:        Xiao Chen                                             //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines Depen with conponents to get deendency relationship 
*  between files.adddependency provide function to add dependency relationship 
*  to dependency table addDependencyLinks provide function to add 
*  dependented files link to .html files. adddepen using regex to select .h and .cpp
*  files from a file sets. parserDoing process parser to get dependency relationship
*  recursively.

*
*
*  Required Files:
* =======================
*  Semi.h Semi.cpp ITokenCollection.h Dependency.h
*  Logger.h DependencyTable.h Convert.h Display.h
*  Logger.cpp DependencyTable.cpp Convert.cpp Display.cpp
*
*  Maintainence History:
* =======================
*  ver 1.0 - 4 Mar 2019
*  - first release
*/




#include <iostream>
#include <string>
#include <fstream>
#include "../SemiExpression/Semi.h"
#include "../Logger/Logger.h"
#include "../DependencyTable/DependencyTable.h"
#include "../Converter/Converter.h"
#include "../Display/Display.h"

namespace depend {
	class Depen {
	public:
		Depen();
		~Depen();
		void adddependency(std::string& fileSpec,std::vector<std::string>& temp);
		void addDependencyLinks(std::string file, DependencyTable dt_, std::ofstream& out_);
		void adddepen(std::vector<Lexer::ITokenCollection*> a);
		std::vector<std::string> doparser();
		void parserDoing(std::string path);
		std::vector<std::string> r_converted_f();
	private:
		std::vector<std::string> depelem;
		std::ofstream out_;
		std::vector<std::string> converted_f;
	};

}
