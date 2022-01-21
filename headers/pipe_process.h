#ifndef PIPE_PROCESS_H
#define PIPE_PROCESS_H

//--------------------------------------------------------------------------------

#include <string>

#include "my_process.h"

class PipeProcess : public MyProcess
{
public:
	enum PypeType { ZERO = 0, NO_ZERO = 1 };

	PipeProcess(const std::string& aName, const std::string& aParameters);
	virtual ~PipeProcess();

	void readPipe(std::string& result);
	void writePipe(std::string& aMessage, PypeType aType = ZERO);

private:
	virtual void IORedirection();
	virtual void closeHandles();

#ifdef BILL_WINDOWS
	HANDLE mThisSTDIN;
	HANDLE mThisSTDOUT;
	HANDLE mChildSTDIN;
	HANDLE mChildSTDOUT;
#else
int mPipefd[2];
#endif // BILL_WINDOWS
};

//--------------------------------------------------------------------------------

#endif // PIPE_PROCESS_H