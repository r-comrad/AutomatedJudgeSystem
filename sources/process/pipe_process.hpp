#ifndef PIPE_PROCESS_H
#define PIPE_PROCESS_H

//--------------------------------------------------------------------------------

#include <string>

#include "process/my_process.hpp"

namespace proc {
	class PipeProcess : public Process
	{
	public:
		enum PypeType { ZERO = 0, NO_ZERO = 1 };


		PipeProcess
		(
			const StringTable& aParameters,
			uint_64 aTimeLimit = 1e18,
			uint_64 aMemoryLimit = 1e18
		);
		virtual ~PipeProcess();

		void readPipe(std::string& result);
		void writePipe(std::string& aMessage, PypeType aType = ZERO);

	private:
		virtual void IORedirection();

	#if		defined(BILL_WINDOWS)
		virtual void closeHandles();

		HANDLE mThisSTDIN;
		HANDLE mThisSTDOUT;
		HANDLE mChildSTDIN;
		HANDLE mChildSTDOUT;
	#elif	defined(LINUS_LINUX)
		int mParentPipes[2];
		int mChildPipes[2];
	#endif // BILL_WINDOWS
	};
}

//--------------------------------------------------------------------------------

#endif // PIPE_PROCESS_H