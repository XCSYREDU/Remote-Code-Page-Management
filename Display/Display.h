#pragma once
///////////////////////////////////////////////////////////////////////////
// Display.h     : defines webpage display using browser functions       //
// ver 1.1                                                               //
//                                                                       // 
//  Platform:      Dell Alienware 17R5, Windows 10                       //
//  Application:   Project for CSE687 - OOD Projects                     //
//  Author:        Xiao Chen                                             //
//  Source: Display.h from TA Ammar Salman                                //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines Display class which accepts a list of files as a
*  vector<string> and uses the default internet browser to display them
*  one by one. Please note that the functionality has limiations:
*   1) Opera/MS Edge: will pop-up all tabs instantly. 
*   2) Chrome/Firefox: will pop-up windows separately only if no
*      already existing Chrome/Firefox window is opened (all must be
*      closed before running this). 
*  
*  By ver 1.1 the package add a function to find and add html script to hide
*  all of the sigle line comment and the multi line function and a function to find 
*  and hide all of the functions and classes inside the script
*
*  Required Files:
* =======================
*  Display.h Display.cpp Process.h Process.cpp
*  Logger.h Logger.cpp FileSystem.h FileSystem.cpp
*  Parser.h Parser.cpp ConfigureParser.cpp ConfigureParser.h
*
*  Maintainence History:
* =======================
*  ver 1.1 - 3 Mar 2019
*  - add function findsinglecomment(std::string& line, std::ofstream& out_);
*  - add function findmultComment(std::string& line, std::ofstream& out_);
*  - add function findmultComment(std::string& line, std::ofstream& out_);
*  - add function addclassandFunc(std::vector<int> cs, std::vector<int> ce, std::vector<int> ms, std::vector<int> me, int line_num, std::string& line);
*  - add four functions to transfer private vector to other package 
*  - add fout vector to store line count of class and function
* 
*  ver 1.0 - 14 Feb 2019
*  - first release
*/


#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>


class Display
{
public:
  Display();
  void display(const std::string& file);
  void display(const std::vector<std::string>& files);
  bool findsinglecomment(std::string& line, std::ofstream& out_);
  bool findmultComment(std::string& line, std::ofstream& out_);
  void addclassandFunc(std::vector<int> cs, std::vector<int> ce, std::vector<int> ms, std::vector<int> me, int line_num, std::string& line);
  void findaf(std::string path);
  std::vector<int> r_cs();
  std::vector<int> r_ce();
  std::vector<int> r_ms();
  std::vector<int> r_me();

private:
	int pos_mark = 0;
	int pos_mark_mult = 0;
	std::vector<std::string> commtbuffer_;
	std::vector<std::string> mulcommbuffer_;
	std::vector<int> class_start;
	std::vector<int> class_end;
	std::vector<int> method_start;
	std::vector<int> method_end;
	
};

