#pragma once
///////////////////////////////////////////////////////////////////////////
// Cdinterface.h : interface to Translate managed calls into native calls//
// ver 1.0                                                               //
//                                                                       // 
//  Platform:      Dell Alienware 17R5, Windows 10                       //
//  Application:   Project for CSE687 - OOD Projects                     //
//  Author:        Xiao Chen                                             //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package provides an interface for the Cdinterface class
*  and a declaration of an object factory function. Using these
*  isolates the user from all implementation details in Cdinterface.
*
*  Required Files:
* =======================
*  Display.h Display.cpp 
*  Dependency.h Dependency.cpp
*  Executive.h Executive.cpp
*
*  Maintainence History:
* =======================
*  ver 1.0 - 8 April 2019
*  - first release
*/

#include <string>
#include <vector>

struct ICdinterface
{
	virtual ~ICdinterface() {}
	virtual void convertfiles(const std::string& path) = 0;
	virtual void display(const std::string& path) = 0;
	virtual void pcfiles(int argc, std::vector<std::string> argv) = 0;
};

ICdinterface* createCdinterface();