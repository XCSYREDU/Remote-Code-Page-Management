#include "Itranslator.h"
#include "../CdPubInter/Cdinterface.h"

using namespace System;
using namespace System::Text;


public ref class Translator : Itranslator {
public:
	Translator() {
		pCdinterface_ = createCdinterface();
	}
	~Translator() {
		delete pCdinterface_;
		pCdinterface_ = nullptr;
	}
	void convertfiles(String^ path) override {
		pCdinterface_->convertfiles(mToN(path));
	}
	void display(String^ path) override {
		pCdinterface_->display(mToN(path));
	}

private:
	std::string mToN(String^ str);
	String^ nToM(const std::string& str);
	std::vector<double> mToN(List<double>^ listDbls);
	List<double>^ nToM(const std::vector<double>& vecDbls);
	ICdinterface* pCdinterface_ = nullptr;
	std::vector<std::string> sToV(array<String^>^ cmd);
};

Itranslator^ TraslatorFactory::createTranslator()
{
	return gcnew Translator();
}

std::string Translator::mToN(String^ str)
{
	std::string temp;
	for (int i = 0; i < str->Length; ++i)
		temp += char(str[i]);
	return temp;
}

std::vector<std::string> Translator::sToV(array<String^>^ cmd) {
	std::vector<std::string> temp;

	for (int i = 0; i < cmd->Length; i++) {
		temp.push_back(mToN(cmd[i]));
	}
	return temp;
}


String^ Translator::nToM(const std::string& str)
{
	StringBuilder^ sb = gcnew StringBuilder();
	for (size_t i = 0; i < str.size(); ++i)
		sb->Append((wchar_t)str[i]);
	return sb->ToString();
}

std::vector<double> Translator::mToN(List<double>^ dbls)
{
	std::vector<double> temp;
	for (int i = 0; i < dbls->Count; ++i)
		temp.push_back(dbls[i]);
	return temp;
}

List<double>^ Translator::nToM(const std::vector<double>& dbls)
{
	List<double>^ temp = gcnew List<double>();
	for (size_t i = 0; i < dbls.size(); ++i)
		temp->Add(dbls[i]);
	return temp;
}

