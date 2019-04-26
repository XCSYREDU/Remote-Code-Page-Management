///////////////////////////////////////////////////////////////////////////
// translator.cpp : Translates managed calls into native calls           //
// ver 1.0                                                               //
//                                                                       // 
//  Platform:      Dell Alienware 17R5, Windows 10                       //
//  Application:   Project for CSE687 - OOD Projects                     //
//  Author:        Xiao Chen                                             //
//  Source: translator.h from prof Jim Fawcett                           //
///////////////////////////////////////////////////////////////////////////

#include "Itranslator.h"
#include "../CdPubInter/Cdinterface.h"

using namespace System;
using namespace System::Text;


public ref class Translator : Itranslator {
public:
	//----< Constructor of truanslator >--------------------------------------------------

	Translator() {
		pCdinterface_ = createCdinterface();
	}
	//----< destructor of truanslator >----------------------------------------------------

	~Translator() {
		delete pCdinterface_;
		pCdinterface_ = nullptr;
	}
	//----< delegate convert files for code publisher of truanslator >---------------------

	void convertfiles(String^ path) override {
		pCdinterface_->convertfiles(mToN(path));
	}
	//----< delegate disply for code publisher of truanslator >-----------------------------

	void display(String^ path) override {
		pCdinterface_->display(mToN(path));
	}
	//----< delegate process commandline for code publisher of truanslator >----------------

	void pcfile(int argc, List<String^>^ argv) override {
		pCdinterface_->pcfiles(argc,mToN(argv));
	}

private:
	std::string mToN(String^ str);
	String^ nToM(const std::string& str);
	std::vector<std::string> mToN(List<String^>^ listDbls);
	List<double>^ nToM(const std::vector<double>& vecDbls);
	ICdinterface* pCdinterface_ = nullptr;
	std::vector<std::string> sToV(array<String^>^ cmd);
};
//----< object factory >---------------------------------------------------------------------

Itranslator^ TraslatorFactory::createTranslator()
{
	return gcnew Translator();
}
//----< translate string in C# to C++ >-------------------------------------------------------

std::string Translator::mToN(String^ str)
{
	std::string temp;
	for (int i = 0; i < str->Length; ++i)
		temp += char(str[i]);
	return temp;
}
//----< translate array of string in C# to C++ in vector of string >---------------------------

std::vector<std::string> Translator::sToV(array<String^>^ cmd) {
	std::vector<std::string> temp;

	for (int i = 0; i < cmd->Length; i++) {
		temp.push_back(mToN(cmd[i]));
	}
	return temp;
}
//----< translate string in C++ to C# >-------------------------------------------------------

String^ Translator::nToM(const std::string& str)
{
	StringBuilder^ sb = gcnew StringBuilder();
	for (size_t i = 0; i < str.size(); ++i)
		sb->Append((wchar_t)str[i]);
	return sb->ToString();
}
//----< translate list of string in C# to vector of string in  C++ >----------------------------

std::vector<std::string> Translator::mToN(List<String^ >^ dbls)
{
	std::vector<std::string> temp;
	for (int i = 0; i < dbls->Count; ++i)
		temp.push_back(mToN(dbls[i]));
	return temp;
}
//----< translate vector of double  in C++ to List of double in C#>------------------------------

List<double>^ Translator::nToM(const std::vector<double>& dbls)
{
	List<double>^ temp = gcnew List<double>();
	for (size_t i = 0; i < dbls.size(); ++i)
		temp->Add(dbls[i]);
	return temp;
}

