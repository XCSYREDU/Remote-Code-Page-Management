///////////////////////////////////////////////////////////////////////////
// executive.cpp : defines facade/executive for project 2 of OOD         //
// ver 1.0                                                               //
//                                                                       // 
//  Platform:      Dell Alienware 17R5, Windows 10                       //
//  Application:   Project for CSE687 - OOD Projects                     //
//  Author:        Xiao Chen                                             //
///////////////////////////////////////////////////////////////////////////

#include "Executive.h"
#include "../Logger/Logger.h"

using namespace Utilities;
using namespace Logging;
using namespace FileSystem;

ProcessCmdLine::Usage customUsage();
// -----< Default constructor >--------------------------

Execut::Execut() {}

// -----< Process command line >--------------------------

bool Execut::processCommandLineArgs(int argc, char ** argv) {
	pcl_ = new ProcessCmdLine(argc, argv);
	pcl_->usage(customUsage());
	if (pcl_->parseError())
	{
		pcl_->usage();
		std::cout << "\n\n";
		return false;
	}
	dirIn_ = pcl_->path();
	return true;
}
// -----< return pointer of processcmdline to be used by other package >--------------------------

Utilities::ProcessCmdLine* Execut::return_pc() {
	return pcl_;
}

ProcessCmdLine::Usage customUsage()
{
	std::string usage;
	usage += "\n  Command Line: path [/option]* [/pattern]* [/regex]*";
	usage += "\n    path is relative or absolute path where processing begins";
	usage += "\n    [/option]* are one or more options of the form:";
	usage += "\n      /s     - walk directory recursively";
	usage += "\n      /demo  - run in demonstration mode (cannot coexist with /debug)";
	usage += "\n      /debug - run in debug mode (cannot coexist with /demo)";
	//usage += "\n      /h - hide empty directories";
	//usage += "\n      /a - on stopping, show all files in current directory";
	usage += "\n    [pattern]* are one or more pattern strings of the form:";
	usage += "\n      *.h *.cpp *.cs *.txt or *.*";
	usage += "\n    [regex] regular expression(s), i.e. [A-B](.*)";
	usage += "\n";
	return usage;
}
// -----< with getfile from loader extract files from cmdline >--------------------------

bool Execut::extractFiles()
{
	// std::cout << "loader" << std::endl;
	Loader ld;
	bool rf = ld.getFiles(pcl_);
	files_ = ld.r_fileset();
	return rf;
}


// -----< return output directory >--------------------------------------
const std::string & Execut::outputDirectory() const
{
	return dirOut_;
}

// -----< return input directory - from PCL >----------------------------
const std::string & Execut::workingDirectory() const
{
	return dirIn_;
}

// -----< set output directory >-----------------------------------------
void Execut::outputDirectory(const std::string & dir)
{
	dirOut_ = dir;
	//cconv_.outputDir(dir);
}
// -----< return vector to be used by other package >--------------------------

std::vector<std::string> Execut::r_files() {
	return files_;
}


#ifdef TEST_EXE

#include <iostream>

// -----< NORMAL USE MAIN > ------------------------------------------
int main(int argc, char ** argv) {
	Execut ex;
	if (!ex.processCommandLineArgs(argc, argv)) {
		std::cout << "\n  Failed to process command line arguments. Terminating\n\n";
		getchar();
		return 0;
	}
	ex.extractFiles();
	getchar();
	return 0;
}

#endif