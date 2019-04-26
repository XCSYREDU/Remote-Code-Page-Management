#pragma once

///////////////////////////////////////////////////////////////////////////
// executive.h : defines facade/executive for project 2 of OOD           //
// ver 1.0                                                               //
//                                                                       // 
//  Platform:      Dell Alienware 17R5, Windows 10                       //
//  Application:   Project for CSE687 - OOD Projects                     //
//  Author:        Xiao Chen                                             //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines Execut with Converter Dependency Loader and other package
*  to demonstrate what I have done with this project
*
*  Required Files:
* =======================
*  Parser.h Parser.cpp CodeUtilities.h
*  Loader.h Loader.cpp DirExplorerN.h DirExplorerN.cpp
*  Display.h Display.cpp Converter.h Converter.cpp
*  Converter.h Converter.cpp Dependency.h Dependency.cpp
*  Executive.h
*  
*  Maintainence History:
* =======================
*  ver 1.0 - 14 Feb 2019
*  - first release
*/

#include <string>
#include <vector>
#include "../Loader/Loader.h"
#include "../DirExplorer-Naive/DirExplorerN.h"
#include "../Utilities/CodeUtilities/CodeUtilities.h"



class Execut {
public:
	Execut();
	bool processCommandLineArgs(int argc, char ** argv);
	bool extractFiles();
	const std::string& workingDirectory() const;
	void outputDirectory(const std::string& dir);
	const std::string& outputDirectory() const;
	Utilities::ProcessCmdLine* return_pc();
	std::vector<std::string> r_files();

private:
	Utilities::ProcessCmdLine *pcl_;
	std::string dirIn_;
	std::vector<std::string> files_;
	std::vector<std::string> c_files_;
	std::string dirOut_;

}; 