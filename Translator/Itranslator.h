#pragma once
using namespace System;
using namespace System::Collections::Generic;

public ref class Itranslator abstract {
public:
	virtual void convertfiles(String^ path) = 0;
	virtual void display(String^ path) = 0;
	
};

public ref class TraslatorFactory {
public:
	static Itranslator^ createTranslator();
};