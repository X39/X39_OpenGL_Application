#pragma once

#include <string>

class Model
{
	//enums, Structures
	private:
		enum FileType
		{
			FILETYPE_OBJ
		};

	//Variables
	private:
		std::string path;
	private:
		void init(const std::string s);
		void loadModelFromDrive(void);
		void loader_dotObj(void);

	public:
		Model(const char* s);
		Model(const ::std::string s);
		~Model(void);
		void loadToGpu(void);
		void unloadFromGpu(void);
		void render(void);
};

