#pragma once

#include "globals.h"
#include "glHeaders.h"
#include "Model.h"


namespace X39
{
	namespace Rendering
	{
		class ModelManager
		{
		public:
			ModelManager(void);
			~ModelManager(void);
			void registerModels(void);
			//void bindModelToGraCaBuffer(Model* vbo);
			//void unbindModelFromGraCaBuffer(Model* vbo);
		};
	};
};