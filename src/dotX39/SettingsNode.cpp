#include "SettingsNode.h"

/*
import java.util.ArrayList;
import java.util.List;
*/

	
	SettingsNode::SettingsNode(void) {
		boundOptions = ::std::vector<SettingsOption*>();
		childs = ::std::vector<SettingsNode*>();
		_name = "";
	}
	SettingsNode::SettingsNode(char* s) {
		boundOptions = ::std::vector<SettingsOption*>();
		childs = ::std::vector<SettingsNode*>();
		_name = s;
	}
	SettingsNode::SettingsNode(std::string& s) {
		boundOptions = ::std::vector<SettingsOption*>();
		childs = ::std::vector<SettingsNode*>();
		_name = new char[s.size()];
		strcpy(_name, s.c_str());
	}
	char* SettingsNode::getName(void){
		return _name;
	}
	void SettingsNode::setName(char* s){
		_name = s;
	}