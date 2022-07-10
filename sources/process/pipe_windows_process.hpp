#ifndef PIPE_PROCESS_H
#define PIPE_PROCESS_H

//--------------------------------------------------------------------------------

#include <string>

#include "windows_process.hpp"

namespace proc 
{
	class PipeWindowsProcess : public WindowsProcess
	{
	public:
		enum PypeType { ZERO = 0, NO_ZERO = 1 };


		PipeWindowsProcess() noexcept;
		virtual ~PipeWindowsProcess();

		void readPipe(std::string& result) noexcept;
		void writePipe(std::string& aMessage, PypeType aType = ZERO) noexcept;

	private:
		virtual void IORedirection() noexcept;
		virtual void closeHandles() noexcept;

		HANDLE mThisSTDIN;
		HANDLE mThisSTDOUT;
		HANDLE mChildSTDIN;
		HANDLE mChildSTDOUT;
	};
}

//--------------------------------------------------------------------------------

#endif // PIPE_PROCESS_H