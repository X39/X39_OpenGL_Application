#pragma once
namespace X39 {
	namespace Threading {
		class BaseTask {
			public:
				virtual void execute(void) = 0;
		};
	}
}