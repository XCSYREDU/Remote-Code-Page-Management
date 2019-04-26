///////////////////////////////////////////////////////////////////////////
// loader.cpp : define the cunction in loader.h                          //
// ver 1.0                                                               //
//                                                                       // 
//  Platform:      Dell Alienware 17R5, Windows 10                       //
//  Application:   Project for CSE687 - OOD Projects                     //
//  Author:        Xiao Chen                                             //
///////////////////////////////////////////////////////////////////////////
#include "Loader.h"

using namespace Utilities;
using namespace FileSystem;

// -----< default ctor >--------------------------------------------------
Loader::Loader() {

}

// -----< getFiles from certain processcmdline >----------------------------
bool Loader::getFiles(Utilities::ProcessCmdLine * pcl_)
{
	DirExplorerN de(pcl_->path());

	for (auto patt : pcl_->patterns())
	{
		de.addPattern(patt);
	}

	if (pcl_->hasOption("s"))
	{
		de.recurse();
	}

	bool aa = de.search() && de.match_regexes(pcl_->regexes());
	de.showStats();

	filesets = de.filesList();

	return aa;
}

// -----< return vector to be used by other package >--------------------------
std::vector<std::string> Loader::r_fileset() {
	return filesets;
}