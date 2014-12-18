#include "SOFTUO_SettingsNodeCollection.h"

/*
import java.util.ArrayList;
import java.util.List;
*/
	
	SOFTUO_SettingsNodeCollection::SOFTUO_SettingsNodeCollection(void){
		_validIndex = -1;
		_currentIndex = -1;
		_list = ::std::vector<SettingsNode*>();
		
	}
	
	void SOFTUO_SettingsNodeCollection::up(int i){
		_currentIndex -= i;
		if(_currentIndex < 0 || i < 0)
			_currentIndex = 0;
	}
	void SOFTUO_SettingsNodeCollection::down(SettingsNode* node){
		if(_validIndex > -1)
			_list[_currentIndex]->childs.push_back(node);
		_currentIndex++;
		_validIndex = _currentIndex;
		if(_list.size() < _validIndex + 1)
			_list.push_back(node);
		else
			_list[_currentIndex] = node;
	}
	bool SOFTUO_SettingsNodeCollection::down(void){
		if(_currentIndex == _validIndex)
			return false;
		_currentIndex--;
		return true;
	}
	int SOFTUO_SettingsNodeCollection::getCurrentIndex(void){
		return _currentIndex;
	}
	
	SettingsNode* SOFTUO_SettingsNodeCollection::getNode(void){
		if(_validIndex <= -1)
			return NULL;
		return _list[_currentIndex];
	}