#pragma once
#include "BaseTask.h"
namespace X39 {
	class Simulation final
	{
	private:
		void prepareThreadPool();
		void init(void);
		void uninit(void);
		Simulation()
		{
			init();
		}
		~Simulation()
		{
			uninit();
		}
	public:
		static inline Simulation& getInstance()
		{
			static Simulation instance;
			return instance;
		}
		void doRendering();

		Threading::BaseTask* getNextTask(void*);
		void addTask(Threading::BaseTask*);
		void setTaskAsDone(Threading::BaseTask*);
		
	};
}