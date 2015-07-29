#pragma once

#include <string>

namespace X39
{
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
		//Functions
		private:
			void init(const std::string s);
			void loadModelFromDrive(void);

		public:
			Model(const char* s);
			Model(const ::std::string s);
			~Model(void);
			void loadToGpu(void);
			void unloadFromGpu(void);
			void render(void);
			void loader_dotObj(void);
	};
};