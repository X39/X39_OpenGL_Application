#pragma once

#include "SettingsNode.h"
#include <vector>

class SOFTUO_SettingsNodeCollection {

private:
	int _validIndex;
	int _currentIndex;
	::std::vector<SettingsNode*> _list;
	
public:
	SOFTUO_SettingsNodeCollection(void);
	void up(int i);
	void down(SettingsNode* node);
	bool down(void);
	int getCurrentIndex(void);
	SettingsNode* getNode(void);
};