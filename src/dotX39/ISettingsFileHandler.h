#pragma once

#include "SettingsDocument.h"
#include "SettingsNode.h"
#include "SettingsOption.h"
#include "SOFTUO_SettingsNodeCollection.h"
#include <fstream>
#include <string>
#define __ISettingsFileHandler_UseParserVersion_1_0__ 1

class ISettingsFileHandler {
	protected:
		std::string _file;
		std::string _dir;
	public:
		ISettingsFileHandler::ISettingsFileHandler(const char* file);
		/**
		 * Used to get the Special SettingsFileHandlerVersion
		 * @return SettingsFileHandlerVersion
		 */
		char* getVersion(void);
		/**
		 * Used to set the specified file you want to write to!<br>
		 * @param file
		 */
		void setFile(const char* s);
		/**
		 * 
		 * @return SettingsDocument of the specified File
		 * @throws Exception
		 */
		SettingsDocument* load(void);
		/**
		 * Open the file that was specified with setFile(File)
		 * @throws Exception
		 */
		void save(SettingsDocument* doc);	
		void save_helper_rekursive(int index, SettingsNode* root, std::fstream* out);
		char* save_parseSyntax(char* s);
};