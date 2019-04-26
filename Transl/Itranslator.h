#pragma once
///////////////////////////////////////////////////////////////////////////
// itranslator.h : Translates managed calls into native calls            //
// ver 1.0                                                               //
//                                                                       // 
//  Platform:      Dell Alienware 17R5, Windows 10                       //
//  Application:   Project for CSE687 - OOD Projects                     //
//  Author:        Xiao Chen                                             //
//  Source: itranslator.h from prof Jim Fawcett                          //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package is responsible for translating managed types into
 * native types and for delegating managed calls to native calls.
 *
 * This file declares an interface and object factory for C# clients
 * to use, isolating them from the Translator implementation.
 * 
 * For C# clients to use this package it needs to be built as a dll,
 * and clients need to make a reference to the Translator project.
 *
 * In order to create a C++\CLI project you need to have installed
 * C++\CLI support.  See CSE687\Presentations\CreatingCppCli.pdf.
*
*  Required Files:
* =======================
*  Cdinterface.h Cdinterface.cpp
*
*  Maintainence History:
* =======================
*  ver 1.0 - 8 April 2019
*  - first release
*/

using namespace System;
using namespace System::Collections::Generic;

public ref class Itranslator abstract {
public:
	virtual void convertfiles(String^ path) = 0;
	virtual void display(String^ path) = 0;
	virtual void pcfile(int argc, List<String^>^ argv) = 0;
};

public ref class TraslatorFactory {
public:
	static Itranslator^ createTranslator();
};