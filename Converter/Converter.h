#pragma once
///////////////////////////////////////////////////////////////////////////
// Converter.h   : defines source code conversion to webpage functions   //
// ver 1.1                                                               //
//                                                                       // 
//  Platform:      Dell Alienware 17R5, Windows 10                       //
//  Application:   Prototype for CSE687 - OOD Projects                   //
//  Author:        Xiao Chen                                             //
//  Source: Converter.h from TA Ammar Salman                             //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines CodeConverter class which accepts DependencyTable
*  and uses its relationships to create linked webpages that point to each
*  other based on dependencies. It can also convert single files if needed.
*  The conversion process filters HTML special characters before printing
*  them into output files. The resulting output of this converter is a list
*  of files (vector<string>) of the created webpages.
*
*  Required Files:
* =======================
*  Converter.h Converter.cpp DependencyTable.h DependencyTable.cpp
*  Logger.h Logger.cpp FileSystem.h FileSystem.cpp
*  Dependency.h Dependency.cpp
*  Display.h Display.cpp
*
*  Maintainence History:
* =======================
*  ver 1.1 - 4 Mar 2019
*  add button script for each .html files
*  add hiding function for each .html files
*
*
*  ver 1.0 - 11 Feb 2019
*  - first release
*/

#include "../DependencyTable/DependencyTable.h"
#include"../Dependency/Dependency.h"
#include "../Display/Display.h"
#include <vector>
#include <string>
#include <fstream>

class CodeConverter
{
public:
	CodeConverter();
	CodeConverter(const DependencyTable& dt);

	const std::string& outputDir() const;
	void outputDir(const std::string& dir);
	bool createOutpurDir();

	void setDepTable(const DependencyTable& dt);

	std::vector<std::string> convert();
	std::string convert(const std::string& filepath);
	std::vector<std::string> convert(const std::vector<std::string>& files);

	const std::vector<std::string> convertedFiles() const;
	std::vector<std::string> r_convertedFiles_();
	void clear();

private:
	bool convertFile(std::string file);
	void addPreCodeHTML(const std::string& title);
	void addButtonbody();
	void addButttonScript();
	void addButton();
	void addPreTag();
	void addClosingTags();
	void skipSpecialChars(std::string& line);
	//bool findClass(std::string& line);
	//bool findMethod(std::string& line);
	void executive(std::ifstream& in, std::string& filename, std::string& file);
	
private:
	//int class_mark = 0;
	//int method_mark = 0;
	DependencyTable dt_;
	std::string outputDir_ = "..\\ConvertedWebpages\\";
	std::vector<std::string> convertedFiles_;
	std::ofstream out_;
	//std::vector<std::string> classbuffer_;
	//std::vector<std::string> methodbuffer_;
};

