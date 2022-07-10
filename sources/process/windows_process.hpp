#ifndef WINDOWS_PROCESS_H
#define WINDOWS_PROCESS_H

//--------------------------------------------------------------------------------
//				CHILD PROCESS CREATION IMPLIMENTATION 
//--------------------------------------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <vector>
#include <iostream>

#include "domain/type.hpp"
#include "domain/string.hpp"
#include "domain/error_message.hpp"
#include "domain/path.hpp"
#include "domain/pair.hpp"

#include "process.hpp"

//--------------------------------------------------------------------------------


#define _CRT_SECURE_NO_WARNINGS
#include <future>
#include <windows.h>
#include <userenv.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <atlconv.h>
#include <atlalloc.h>
#include <shlwapi.h>
#include <cstdint>

#include "CPUTime/getCPUTime.hpp"

//--------------------------------------------------------------------------------

namespace proc
{
	class WindowsProcess : public Process
	{
	public:
		/*
		\brief Base process constructor that initialize time and memory 
		usage limits for child process.
		(all child process have max time and memory usage limits
		that defined in MAX_TIME_LIMIT and MAX_MEMORY_LIMIT)
		\param aParameters Child process parameters for execution.
		\param aTimeLimit Child process maximum time usage.
		\param aMemoryLimit Child process maximum memory usage.
		*/
		WindowsProcess() noexcept;
		virtual ~WindowsProcess() = default;

        virtual void setComand(const dom::String& aParameters) noexcept;
        /*
		\brief Create a child process with the specified parameters.
		\param aParameters Child process parameters for execution.
		*/
		virtual void create() noexcept;

		/*
		\brief Executing the child process without time and memory 
		usage evaluation.
		\return Returns true if the process is completed successfully.
		*/
		virtual bool run() noexcept;

		/*
		\brief Executing the child process with time and memory 
		usage evaluation.
		\return Returns the time and memory used by the process.
		*/
		virtual dom::Pair<uint_64> runWithLimits() noexcept;

	protected:
		/*
		\brief Redirecting input and output streams for a child process.
		*/
		virtual void IORedirection() noexcept = 0;

		/*
		\brief Closes the input and output handler for the child process.
		(finishing of communication with the child process)
		*/
    
		virtual void closeHandles() noexcept = 0;

	protected:
		STARTUPINFOA mStartupInfo;
		PROCESS_INFORMATION mProcessInfo;

	private:
		std::future<long long> mFuture;

		long long getMillisecondsNow() noexcept;

		long long getCurrentMemoryUsage(HANDLE&) noexcept;
		long long getMaxMemoryUsage(PROCESS_INFORMATION&, long long) noexcept;

		DWORD getExitCode(HANDLE&) noexcept;
		bool killProcess(PROCESS_INFORMATION&) noexcept;

	private:
		uint_64 mTimeLimit;
		uint_64 mMemoryLimit;

        dom::CharArray mProcessName;
        dom::CharArray mProcessArgs;
	};

}

//--------------------------------------------------------------------------------

#endif // WINDOWS_PROCESS_H