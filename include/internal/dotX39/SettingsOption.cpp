#include "SettingsOption.h"


	SettingsOption::SettingsOption(char* s, char* value) {
		_value = value;
		_name = s;
	}

	SettingsOption::SettingsOption(std::string& s, std::string& value) {
		_value = new char[value.size()];
		_name = new char[s.size()];
		strcpy(_value, value.c_str());
		strcpy(_name, s.c_str());
	}

	char* SettingsOption::getName(void){
		return _name;
	}
	char* SettingsOption::getValue(void){
		return _value;
	}
	void SettingsOption::setName(char* s){
		_name = s;
	}
	void SettingsOption::setValue(char* s){
		_value = s;
	}