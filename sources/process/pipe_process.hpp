#ifndef PIPE_PROCESS_H
#define PIPE_PROCESS_H

//--------------------------------------------------------------------------------

#include <string>
#include <cstring>

#include "my_process.hpp"

#ifdef LINUS_LINUX
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#endif

class PipeProcess : public MyProcess
{
public:
	enum PypeType { ZERO = 0, NO_ZERO = 1 };


	PipeProcess
	(
		const std::vector<char*>& aParameters,
		uint_64 aTimeLimit = 1e18,
		uint_64 aMemoryLimit = 1e18
	);
	virtual ~PipeProcess();

	void readPipe(str_ref result);
	void writePipe(str_ref aMessage, PypeType aType = ZERO);

private:
	virtual void IORedirection();

#if		defined(BILL_WINDOWS)
	virtual void closeHandles();

	HANDLE mThisSTDIN;
	HANDLE mThisSTDOUT;
	HANDLE mChildSTDIN;
	HANDLE mChildSTDOUT;
#elif	defined(LINUS_LINUX)
	void create(const std::vector<char*>& aParameters);

    // int mParentPipes[2];
    // int mChildPipes[2];


    int mPipeA[2];
    int mPipeB[2];
#endif // BILL_WINDOWS
};

//--------------------------------------------------------------------------------

#endif // PIPE_PROCESS_H