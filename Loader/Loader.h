#pragma once
///////////////////////////////////////////////////////////////////////////
// loader.h : define function to get file path from a certain commandline//
// ver 1.1                                                               //
//                                                                       // 
//  Platform:      Dell Alienware 17R5, Windows 10                       //
//  Application:   Project for CSE687 - OOD Projects                     //
//  Author:        Xiao Chen                                             //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines Loader with conponents of getFiles
*  to get files from a certain path given by command line
*  and r_filesets to return path of files
*  with package DirExploreN it can process pattern from command line 
*  and get the useful path we want
*
*
*  Required Files:
* =======================
*  DirExplorerN.h DirExplorerN.cpp CodeUtilities.h
*  Loader.h
*
*  Maintainence History:
* =======================
*  ver 1.0 - 4 Mar 2019
*  - first release
*/


#include <string>
#include <vector>
#include "../DirExplorer-Naive/DirExplorerN.h"
#include "../Utilities/CodeUtilities/CodeUtilities.h"

class Loader {
public:
	Loader();
	bool getFiles(Utilities::ProcessCmdLine *pcl_);
	std::vector<std::string> r_fileset();
private:
	std::vector<std::string> filesets;
};