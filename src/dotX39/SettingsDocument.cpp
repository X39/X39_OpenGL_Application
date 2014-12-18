#include "SettingsDocument.h"
	

	SettingsDocument::SettingsDocument(char* file, SettingsNode& root){
		_file = file;
		_root = &root;
	}
	void SettingsDocument::deleteOptions(SettingsNode& node)
	{
		for(int i = 0; i < node.boundOptions.size(); i++)
		{
			SettingsOption* opt = node.boundOptions[i];
			delete opt;
		}
	}
	void SettingsDocument::rekursive_deleteNodes(SettingsNode& node)
	{
		for(int i = 0; i < node.childs.size(); i++)
		{
			rekursive_deleteNodes(*node.childs[i]);
		}
		deleteOptions(node);
		delete &node;
	}
	SettingsDocument::~SettingsDocument()
	{
		deleteOptions(*_root);
		rekursive_deleteNodes(*_root);
	}
	
	SettingsNode* SettingsDocument::getRootNode(){
		return _root;
	}