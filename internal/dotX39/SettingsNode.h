#pragma once

#include "SettingsOption.h"
#include <vector>
#include <string>

class SettingsNode {
	private:
		char* _name;
	public:
		::std::vector<SettingsOption*> boundOptions;
		::std::vector<SettingsNode*> childs;
	
	protected:
		SettingsNode();
	public:
		SettingsNode(char* s);
		SettingsNode(std::string& s);
		char* getName();
		void setName(char* s);
};