#ifndef WINDOWS_PIPE_PROCESS_H
#define WINDOWS_PIPE_PROCESS_H

//--------------------------------------------------------------------------------

#include <string>

#include "windows_process.hpp"

namespace proc 
{
	class PipeWindowsProcess : public WindowsProcess
	{
	public:
		PipeWindowsProcess() noexcept;
		virtual ~PipeWindowsProcess();
        NOC__NOD_FUNCs(PipeWindowsProcess)

		void readPipe(std::string& result) noexcept;
		void writePipe(std::string& aMessage, PypeType aType = Process::PypeType::ZERO) noexcept;

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

#endif // !WINDOWS_PIPE_PROCESS_H