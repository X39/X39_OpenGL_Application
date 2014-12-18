#include "ISettingsFileHandler.h"
#if __ISettingsFileHandler_UseParserVersion_1_0__
#include <stdio.h>
/*
import java.io.FILE;
import java.io.FILEInputStream;
import java.io.FILEOutputStream;
import java.io.PrintStream;
import java.util.ArrayList;
*/
	
	
	
	ISettingsFileHandler::ISettingsFileHandler(char* file) {
		_file = "";
		_dir = "";
		setFile(file);
	}
	char* ISettingsFileHandler::getVersion(void) {
		return "1.0";
	}

	void ISettingsFileHandler::setFile(char* str){
		std::string s = str;
		char c = '/';
		if(s.find("\\") != std::string::npos)
			c = '\\';
		_file = s;
		_dir = s.substr(0, s.length() - s.rfind(c));
		
	}
	//char* ISettingsFileHandler::split(char* regex, char splitChar){
	//	ArrayList<char*> arr = new ArrayList<char*>();
	//	char* s = new char*();
	//	
	//	for(char c : regex.toCharArray()){
	//		if(c == splitChar){
	//			arr.add(s);
	//			s = new char*();
	//			continue;
	//		}
	//		s += c;
	//	}
	//	arr.add(s);
	//	char* array[] = {""};
	//	return arr.toArray(array);
	//}

	SettingsDocument* ISettingsFileHandler::load(void){
		std::string s = std::string();
		std::string version = std::string();
		std::string Value = std::string();
		bool isCommentActive = false;
		bool isValueActive = false;
		bool checkForStar = false;
		bool expectingSpecialCharAsNext = false;
		bool backSlash = false;
		char expectedChar = 0x00;
		std::ifstream fIn (_file, std::ifstream::in);
		if(!fIn)
		{
			return NULL;
		}
		int i = 0;
		SOFTUO_SettingsNodeCollection nodes = SOFTUO_SettingsNodeCollection();
		int charCounter = 0;
		int lineCounter = 0;
		i = 0;
		while(!fIn.eof()){
			i = fIn.peek();
			fIn.get();
			charCounter++;
			if(i == 0x09|| i == 0x0A)
				continue;
			if(i == 0x0D) //0x0D == CR
			{
				lineCounter++;
				charCounter = 0;
			}
			if(i == 0x20)
			{
				if(expectingSpecialCharAsNext)
					continue;
				if(!isValueActive)
					continue;
			}
			if(expectingSpecialCharAsNext)
				if(((char) i) == expectedChar){
					expectingSpecialCharAsNext = false;
					continue;
				}
				else
				{
					fIn.close();
					//throw new ExceptionSettingsParsing("Parse Error at line '" + lineCounter + "' char '" + charCounter + "'");
				}
			
			if (isValueActive) {
				bool flag = true;
				if(!backSlash)
				{
					if(i == 0x22){ //0x22 == '"'
						isValueActive = false;
						nodes.getNode()->boundOptions.push_back(new SettingsOption(s, Value));
						s = "";
						Value = "";
						continue;
					}
					if(i == 0x5C) //0x5C == '\'
					{
						backSlash = true;
						flag = false;
					}
				}
				if(flag)
				{
					if(backSlash && (i == 0x4E || i == 0x6E)) //0x4E == 'N' - 0x6E == 'n'
					{
						Value.append("\n");
					}else if(backSlash && (i == 0x54 || i == 0x74)) //0x54 == 'T' - 0x74 == 't'
					{
						Value.append("\t");
					}else if(backSlash && (i == 0x52 || i == 0x72)) //0x52 == 'R' - 0x72 == 'r'
					{
						Value.append("\r");
					}else if(backSlash && i == 0x5C) //0x5C == '\'
					{
						Value.append("\\");
					}else if(backSlash && i == 0x22 ) //0x4E == '"'
					{
						Value.append("\"");
					}else
					{
						char c[] = {(char)i, 0x00};
						Value.append(c);
					}
					backSlash = false;
				}
				continue;
			}else{
				
				//COMMENT HANDLING SECTION-------------------------------------------------------------------
				if(isCommentActive){
					if (checkForStar){ 				// If checkforstar
						if (i == 0x2F) 				// then check i for being '/'
							isCommentActive = false; // if yes the comment line starts HERE
						else						// else
							checkForStar = false; 	// reset checkForStar
					}
					if (i == 0x2A) {//0x2F == '*'
						checkForStar = true;
					}
					continue;
				}else{
					if (checkForStar){ 				// If checkforstar
						if (i == 0x2A){ 			// then check i for being '*'
							isCommentActive = true; // if yes the comment line starts HERE
							s = "";
							continue;
						}else{						// else
							checkForStar = false; 	// reset checkForStar
//							s += (char)0x2F;
						}
					}
					if (i == 0x2F) { //0x2F == '/'
						checkForStar = true;
					}
				}
				//"NODE WITH ARGS", "NODE NO ARGS", "OPTION" Differer-----------------------------------------------------------------------
				if(i == 0x3D){ //0x3D == '=' //OPTION
					expectingSpecialCharAsNext = true;
					expectedChar = 0x22; // 0x22 == '"'
					isValueActive = true;
					continue;
				}else if(i == 0x7B){ //0x7B == '{' //NODE NO ARGS
					nodes.down(new SettingsNode(s));
					s = "";
					continue;
				}else if(i == 0x7D){ //0x7D == '}' //NODE NO ARGS
					nodes.up(1);
					continue;
				}/*	else if(i == 0x28){ //0x28 == '(' //NODE WITH ARGS
					NOT IN THIS VERSION
				}*/
					s += (char)i;
			}
		}
		nodes.up(-1);
		fIn.close();
		char* str = new char[_file.size()];
		strcpy(str, _file.c_str());
		return new SettingsDocument(str, *nodes.getNode());
		
	}

	void ISettingsFileHandler::save(SettingsDocument* doc){
		//SOFTUO_SettingsNodeCollection nodes = SOFTUO_SettingsNodeCollection();
		//nodes.down(doc->getRootNode());
		std::fstream fs (_file, std::fstream::out);
		//PrintStream out = new PrintStream(new FILEOutputStream(new FILE(_file.getAbsolutePath())));
		fs << "<VERSION-" << getVersion() << ">\n";
		save_helper_rekursive(0, doc->getRootNode(), &fs);
		fs.flush();
		fs.close();
	}
	void ISettingsFileHandler::save_helper_rekursive(int index, SettingsNode* root, std::fstream* out)
	{
		std::string spacer = "";
		spacer.append(index, '\t');
		*out << spacer << root->getName() << "{\n";
		if(!root->boundOptions.empty())
		{
			for(int i = 0; i < root->boundOptions.size(); i++)
			{
				SettingsOption* option = root->boundOptions[i];
				*out << spacer << "\t" << option->getName() << " = \"" << save_parseSyntax(option->getValue()) << "\"\n";
			}
		}
		if(!root->childs.empty())
			for(int i = 0; i < root->childs.size(); i++)
			{
				SettingsNode* n = root->childs[i];
				save_helper_rekursive(index + 1, n, out);
			}
		*out << spacer << "}\n";
//		nodes->up(1);
	}
	char* ISettingsFileHandler::save_parseSyntax(char* s)
	{
		std::string out = std::string();
		for(int i = 0; s[i] != 0x00; i++)
		{
			char c = s[i];
			if(c == '\\')
			{
				out += '\\';
				out += '\\';
				continue;
			}else if(c == '\"')
			{
				out += '\\';
				out += '\"';
				continue;
			}else if(c == '\t')
			{
				out += '\\';
				out += 't';
				continue;
			}else if(c == '\n')
			{
				out += '\\';
				out += 'n';
				continue;
			}else if(c == '\r')
			{
				out += '\\';
				out += 'r';
				continue;
			}
			out += c;
		}
		char* cstr = new char[out.size()];
		strcpy(cstr, out.c_str());
		return cstr;
	}
#endif