///////////////////////////////////////////////////////////////////////////
// Converter.cpp : implements and provides test stub for Converter.h     //
// ver 1.1                                                               //
//                                                                       // 
//  Platform:      Dell Alienware 17R5, Windows 10                       //
//  Application:   Prototype for CSE687 - OOD Projects                   //
//  Author:        Xiao Chen                                             //
//  Source: Converter.h from TA Ammar Salman                             //
///////////////////////////////////////////////////////////////////////////

#include "Converter.h"
#include <vector>
#include <algorithm>
#include "../Logger/Logger.h"
#include "../FileSystem/FileSystem.h"

using namespace FileSystem;
using namespace Logging;

// -----< default ctor >--------------------------------------------------
CodeConverter::CodeConverter()
{
}

// -----< ctor to set dep table >-----------------------------------------
CodeConverter::CodeConverter(const DependencyTable & dt) : dt_(dt)
{
	LoggerDebug::write("\n  CodeConverter initialized with DependencyTable\n");
}

// -----< set dependency table function >---------------------------------
void CodeConverter::setDepTable(const DependencyTable & dt)
{
	dt_ = dt;
}

// -----< convert pre-set dependency table >------------------------------
std::vector<std::string> CodeConverter::convert()
{
	if (!createOutpurDir()) {
		LoggerDebug::write("\n\n  Failed to start conversion due to inability to create output directory");
		LoggerDebug::write("\n  -- Output Directory: " + Path::getFullFileSpec(outputDir_));
		return std::vector<std::string>();
	}

	LoggerDemo::write("\n\n  Converting files to webpages");
	LoggerDebug::write("\n\n  Converting files to webpages");

	LoggerDebug::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));
	LoggerDemo::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));

	for (auto entry : dt_) {
		if (convertFile(entry.first)) {
			LoggerDemo::write("\n  -- Converted: ");
			LoggerDebug::write("\n  -- Converted: ");
		}
		else {
			LoggerDemo::write("\n  -- Failed:    ");
			LoggerDebug::write("\n  -- Failed:    ");
		}

		std::string filename = Path::getName(entry.first);
		LoggerDebug::write(filename);
		LoggerDemo::write(filename);
	}
	LoggerDebug::write("\n");
	LoggerDemo::write("\n");
	return convertedFiles_;
}

// -----< convert single file given path >----------------------------------
std::string CodeConverter::convert(const std::string & filepath)
{
	if (!createOutpurDir()) {
		LoggerDebug::write("\n\n  Failed to start conversion due to inability to create output directory");
		LoggerDebug::write("\n  -- Output Directory: " + Path::getFullFileSpec(outputDir_));
		return "";
	}

	LoggerDemo::write("\n\n  Converting files to webpages");
	LoggerDebug::write("\n\n  Converting files to webpages");

	LoggerDebug::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));
	LoggerDemo::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));

	if (convertFile(filepath)) {
		LoggerDemo::write("\n  -- Converted: ");
		LoggerDebug::write("\n  -- Converted: ");
	}
	else {
		LoggerDemo::write("\n  -- Failed:    ");
		LoggerDebug::write("\n  -- Failed:    ");
	}

	std::string filename = Path::getName(filepath);
	LoggerDebug::write(filename);
	LoggerDemo::write(filename);

	return filename + ".html";
}

std::vector<std::string> CodeConverter::convert(const std::vector<std::string>& files)
{
	clear();
	dt_ = DependencyTable(files);
	return convert();
}

// -----< get set output directory >----------------------------------------
const std::string & CodeConverter::outputDir() const
{
	return outputDir_;
}

// -----< set output directory >--------------------------------------------
void CodeConverter::outputDir(const std::string & dir)
{
	outputDir_ = dir;
}

// -----< create output directory >-----------------------------------------
/* returns success of creation. it could fail if dir is sys protected */
bool CodeConverter::createOutpurDir()
{
	if (!Directory::exists(outputDir_)) {
		LoggerDebug::write("\n  Creating output directory. Path: " + Path::getFullFileSpec(outputDir_));
		return Directory::create(outputDir_);
	}
	LoggerDebug::write("\n  Output directory already exists. Proceeding...");
	return true;
}


// -----< get list of converted files >-------------------------------------
const std::vector<std::string> CodeConverter::convertedFiles() const
{
	return convertedFiles_;
}

void CodeConverter::clear()
{
	convertedFiles_.clear();
	dt_.clear();
}

// -----< private - read file and create webpage >--------------------------
bool CodeConverter::convertFile(std::string file)
{
	std::ifstream in(file);
	if (!in.is_open() || !in.good()) {
		LoggerDebug::write("Error -- unable to read file, path may be invalid.");
		in.close();
		return false;
	}
	std::string filename = Path::getName(file);
	std::string outputPath = outputDir_ + filename + ".html";
	out_.open(outputPath, std::ofstream::out);
	if (!out_.is_open() || !out_.good()) {
		LoggerDebug::write("Error -- unable to open output file for writing.");
		in.close();
		return false;
	}
	executive(in, filename, file);
	addClosingTags();
	out_.close();
	convertedFiles_.push_back(outputPath);
	return true;
}

// -----< execute the whole html file processing function >--------------------------
void CodeConverter::executive(std::ifstream& in, std::string& filename, std::string& file) {
	// write everything to file 
	Display dis;
	dis.findaf(file);
	std::vector<int> cs = dis.r_cs();
	std::vector<int> ce = dis.r_ce();
	std::vector<int> ms = dis.r_ms();
	std::vector<int> me = dis.r_me();
	addPreCodeHTML(filename);
	addButtonbody();
	addButttonScript();
	depend::Depen de;
	de.addDependencyLinks(file, dt_, out_);
	addButton();
	addPreTag();
	while (in.good()) {
		std::string line;
		int line_num = 1;
		while (std::getline(in, line)) {
			skipSpecialChars(line);
			dis.addclassandFunc(cs, ce, ms, me, line_num, line);
			if (dis.findsinglecomment(line, out_) == FALSE) {
				line_num++;
				continue;
			}
			if (dis.findmultComment(line, out_) == FALSE) {
				line_num++;
				continue;
			}
			out_ << line << std::endl;
			line_num++;
		}
	}
}

// -----< private - add generic HTML preliminary markup >-------------------
void CodeConverter::addPreCodeHTML(const std::string& title)
{
	out_ << "<DOCTYPE !HTML>" << std::endl;
	out_ << "<html>" << std::endl;
	out_ << "  <head>" << std::endl;
	out_ << "    <Title>" << title << "</Title>" << std::endl;
	out_ << "    <style>" << std::endl;
	out_ << "      body {" << std::endl;
	out_ << "        padding:15px 40px;" << std::endl;
	out_ << "        font-family: Consolas;" << std::endl;
	out_ << "        font-size: 1.25em;" << std::endl;
	out_ << "        font-weight: normal;" << std::endl;
	out_ << "      }" << std::endl;
	out_ << "      span {" << std::endl;
	out_ << "        display:inline;" << std::endl;
	out_ << "      }" << std::endl;

}

// -----< add button script to .html files >--------------------------
void CodeConverter::addButtonbody() {
	out_ << "      .toggleClassDisplay {  }" << std::endl;
	out_ << "      .toggleMethodDisplay{ }" << std::endl;
	out_ << "      .toggleSingCommentDisplay{ }" << std::endl;
	out_ << "      .toggleMultCommentDisplay{ }" << std::endl;
	out_ << "		.button1{" << std::endl;
	out_ << "		position: fixed;" << std::endl;
	out_ << "		top: 50px;" << std::endl;
	out_ << "		right: 50px;" << std::endl;
	out_ << "		width: 230px;" << std::endl;
	out_ << "		}" << std::endl;
	out_ << "		.button2{" << std::endl;
	out_ << "		position: fixed;" << std::endl;
	out_ << "		top: 80px;" << std::endl;
	out_ << "		right: 50px;" << std::endl;
	out_ << "		width: 230px;" << std::endl;
	out_ << "		}" << std::endl;
	out_ << "		.button3{" << std::endl;
	out_ << "		position: fixed;" << std::endl;
	out_ << "		top:110px;" << std::endl;
	out_ << "		right: 50px;" << std::endl;
	out_ << "		width: 230px;" << std::endl;
	out_ << "		}" << std::endl;
	out_ << "		.button4{" << std::endl;
	out_ << "		position: fixed;" << std::endl;
	out_ << "		top: 140px;" << std::endl;
	out_ << "		right: 50px;" << std::endl;
	out_ << "		width: 230px;" << std::endl;
	out_ << "		}" << std::endl;
	out_ << "      </style>" << std::endl;
}

// -----< add script for buttons in .html files >--------------------------
void CodeConverter::addButttonScript() {
	out_ << "      <script>" << std::endl;
	out_ << "			function showHideClass(className) {" << std::endl;
	out_ << "				let elems = document.getElementsByClassName(className);" << std::endl;
	out_ << "				if (elems === null) {" << std::endl;
	out_ << "					return;" << std::endl;
	out_ << "				}" << std::endl;
	out_ << "				for (let i = 0; i < elems.length; ++i) {" << std::endl;
	out_ << "					let displayProp = elems[i].style.display;" << std::endl;
	out_ << "					if (displayProp === 'none')" << std::endl;
	out_ << "						elems[i].style['display'] = \"inline\";" << std::endl;
	out_ << "					else" << std::endl;
	out_ << "						elems[i].style['display'] = \"none\";" << std::endl;
	out_ << "				}" << std::endl;
	out_ << "			}" << std::endl;
	out_ << "      </script>" << std::endl;
	out_ << "  </head>" << std::endl << std::endl;
	out_ << "  <body>" << std::endl;
}

void CodeConverter::addButton() {
	out_ << "  <div>" << std::endl;
	//out_ << "  <button class = \"button1\" onclick=\"showHideClass('toggleClassDisplay')\">Show/Hide Class Contents" << std::endl;
	out_ << "  <label><input type=\"checkbox\" id =\"hideC\" value=\"hidefuncs\" checked=\"true\" onclick=\"showHideClass('toggleClassDisplay')\">Show/Hide Class Contents</label><br>" << std::endl;
	//out_ << "  < / button>" << std::endl;
	//out_ << "  <button class = \"button2\" onclick=\"showHideClass('toggleMethodDisplay')\">Show/Hide Method Contents" << std::endl;
	out_ << "  <label><input type=\"checkbox\" id =\"hideM\" value=\"hidefuncs\" checked=\"true\" onclick=\"showHideClass('toggleMethodDisplay')\">Show/Hide Method Contents</label><br>" << std::endl;
	//out_ << "  < / button>" << std::endl;
	//out_ << "  <button class = \"button3\" onclick=\"showHideClass('toggleSingCommentDisplay')\">Show/Hide Single Comments Contents" << std::endl;
	out_ << "  <label><input type=\"checkbox\" id =\"hideSC\" value=\"hidefuncs\" checked=\"true\" onclick=\"showHideClass('toggleSingCommentDisplay')\">Show/Hide Single Comments Contents</label><br>" << std::endl;
	//out_ << "  < / button>" << std::endl;
	//out_ << "  <button class = \"button4\" onclick=\"showHideClass('toggleMultCommentDisplay')\">Show/Hide Multi Comments Contents" << std::endl;
	out_ << "  <label><input type=\"checkbox\" id =\"hideMC\" value=\"hidefuncs\" checked=\"true\" onclick=\"showHideClass('toggleMultCommentDisplay')\">Show/Hide Multi Comments Contents</label><br>" << std::endl;
	//out_ << "  < / button>" << std::endl;
	out_ << "  </div>" << std::endl;
}

// -----< private - add pre tag >------------------------------------------
/* seperated into seperate function to allow for dependencies addition
*  before the actual code of the file */
void CodeConverter::addPreTag()
{
	out_ << "    <pre>" << std::endl;
}


// -----< private - add generic HTML markup closing tags >-----------------
void CodeConverter::addClosingTags()
{
	out_ << "    </pre>" << std::endl;
	out_ << "  </body>" << std::endl;
	out_ << "</html>" << std::endl;
}
// -----< return vector to be used by other package >--------------------------
std::vector<std::string> CodeConverter::r_convertedFiles_() {
	return convertedFiles_;
}

// -----< private - replace HTML special chars >---------------------------
/* note: the function uses while loop to make sure ALL special characters
*  are replaced instead of just the first encounter. */
void CodeConverter::skipSpecialChars(std::string & line)
{
	size_t pos = line.npos;
	while ((pos = line.find('<')) != line.npos)
		line.replace(pos, 1, "&lt;");

	while ((pos = line.find('>')) != line.npos)
		line.replace(pos, 1, "&gt;");
}

#ifdef TEST_CONVERTER

// -----< test stub for converter class >-----------------------------------
int main(int argc, char* argv[]) {
	LoggerDebug::attach(&std::cout);
	LoggerDebug::start();

	LoggerDebug::title("Testing CodeConverter functions");

	LoggerDebug::stop(); // do not log DependencyTable logs



	DependencyTable dt;
	dt.addDependency("Converter.h", "Converter.cpp");
	dt.addDependency("Converter.cpp", "Converter.h");
	dt.addDependency("A.h", "A.h");

	LoggerDebug::start(); // log from now on

	CodeConverter cc(dt);
	dt.display();
	LoggerDebug::write("\n");
	//cc.outputDir("C:\\windows\\system32\\"); // will trigger errors which are handled
	cc.convert();

	LoggerDebug::stop();
	return 0;
}

#endif