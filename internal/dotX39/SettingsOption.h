#pragma once

#include <string>

class SettingsOption {
	private:
		char* _value;
		char* _name;

	public:
		SettingsOption(char* s, char* value);
		SettingsOption(std::string& s, std::string& value);

		char* getName();
		char* getValue();
		void setName(char* s);
		void setValue(char* s);
};