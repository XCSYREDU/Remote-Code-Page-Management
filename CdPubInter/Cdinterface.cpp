///////////////////////////////////////////////////////////////////////////
// Cdinterface.cpp : Translates managed calls into native calls          //
// ver 1.0                                                               //
//                                                                       // 
//  Platform:      Dell Alienware 17R5, Windows 10                       //
//  Application:   Project for CSE687 - OOD Projects                     //
//  Author:        Xiao Chen                                             //
///////////////////////////////////////////////////////////////////////////
#include "../Dependency/Dependency.h"
#include "../Display/Display.h"
#include "../Executive/Executive.h"
#include "Cdinterface.h"
#include <iostream>

class Cdinterface : public ICdinterface {
public:
	//----< invoke convert process >--------------------------------------
	virtual void convertfiles(const std::string& path) override {
		
		depend::Depen de;
		de.parserDoing(path);
	}

	//----< invoke display process >--------------------------------------
	virtual void display(const std::string& path) override {
		
		Display dis;
		dis.display(path);
	}

	//----< invoke process command line process and convert file >--------
	virtual void pcfiles(int argc, std::vector<std::string> argv) override {
		int size = argv.size();
		char** cmd = new char*[size];
		for (int i = 0; i < size; i++) {
			cmd[i] = (char*)argv[i].c_str();
		}
		Execut ex;
		if (!ex.processCommandLineArgs(argc, cmd)) {
			std::cout << "\n  Failed to process command line arguments. Terminating\n\n";
			getchar();
			return;
		}
		ex.extractFiles();
		std::vector<std::string> temp = ex.r_files();
		for (auto i : temp) {
			convertfiles(i);
		}
	}
};

//-----< object factory >----------------------------------------------------
ICdinterface* createCdinterface() 
{
	return new Cdinterface();
}