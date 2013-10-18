#pragma once

#include "SettingsNode.h"
#include <fstream>

class SettingsDocument{
	private:
		char* _file;
		SettingsNode* _root;
		char* _version;
		void rekursive_deleteNodes(SettingsNode& node);
		void deleteOptions(SettingsNode& node);
	public:
		SettingsDocument(char* file, SettingsNode& root);
		~SettingsDocument();
		SettingsNode* getRootNode();

};